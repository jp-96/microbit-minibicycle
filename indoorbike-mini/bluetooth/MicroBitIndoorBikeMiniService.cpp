/*
MIT License

Copyright (c) 2021 jp-rad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "MicroBitIndoorBikeMiniService.h"
#include "struct.h"

MicroBitIndoorBikeMiniService::MicroBitIndoorBikeMiniService(BLEDevice &_ble, MicroBitIndoorBikeMiniSensor &_sensor, MicroBitIndoorBikeMiniServo &_servo, uint16_t id)
    : ble(_ble), sensor(_sensor), servo(_servo)
{
    this->id = id;
    this->currentTargetResistanceLevel10=0;
    this->lastWindSpeed1000=0;
    this->lastGrade100=0;
    this->lastCrr10000=0;
    this->lastCw100=0;
    this->nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize=0;
    // Caractieristic
    GattCharacteristic  indoorBikeDataCharacteristic(
        UUID(0x2AD2)
        , (uint8_t *)&indoorBikeDataCharacteristicBuffer, 0, sizeof(indoorBikeDataCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    );
    GattCharacteristic  fitnessMachineControlPointCharacteristic(
        UUID(0x2AD9)
        , (uint8_t *)&fitnessMachineControlPointCharacteristicBuffer, 0, sizeof(fitnessMachineControlPointCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE|GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE
    );
    GattCharacteristic  fitnessMachineFeatureCharacteristic(
        UUID(0x2ACC)
        , (uint8_t *)&fitnessMachineFeatureCharacteristicBuffer, 0, sizeof(fitnessMachineFeatureCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
    );
    GattCharacteristic  fitnessMachineStatusCharacteristic(
        UUID(0x2ADA)
        , (uint8_t *)&fitnessMachineStatusCharacteristicBuffer, 0, sizeof(fitnessMachineStatusCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    );
    GattCharacteristic  fitnessTrainingStatusCharacteristic(
        UUID(0x2AD3)
        , (uint8_t *)&fitnessTrainingStatusCharacteristicBuffer, 0, sizeof(fitnessTrainingStatusCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    );
    GattCharacteristic  fitnessSupportedResistanceLevelRangeCharacteristic(
        UUID(0x2AD6)
        , (uint8_t *)&fitnessSupportedResistanceLevelRangeCharacteristicBuffer, 0, sizeof(fitnessSupportedResistanceLevelRangeCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
    );
    
    // Set default security requirements
    indoorBikeDataCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessMachineControlPointCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessMachineFeatureCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessMachineStatusCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessTrainingStatusCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessSupportedResistanceLevelRangeCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    // Service
    GattCharacteristic *characteristics[] = {
        &indoorBikeDataCharacteristic,
        &fitnessMachineControlPointCharacteristic,
        &fitnessMachineFeatureCharacteristic,
        &fitnessMachineStatusCharacteristic,
        &fitnessTrainingStatusCharacteristic,
        &fitnessSupportedResistanceLevelRangeCharacteristic,
    };
    GattService service(
        UUID(0x1826), characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *)
    );
    ble.addService(service);

    // FTMS - Service Advertising Data
    const uint8_t FTMS_UUID[sizeof(UUID::ShortUUIDBytes_t)] = {0x26, 0x18};
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, FTMS_UUID, sizeof(FTMS_UUID));
    uint8_t serviceData[2+1+2];
    struct_pack(serviceData, "<HBH", 0x1826, 0x01, 1<<5);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, serviceData, sizeof(serviceData));

    // Characteristic Handle
    indoorBikeDataCharacteristicHandle = indoorBikeDataCharacteristic.getValueHandle();
    fitnessMachineControlPointCharacteristicHandle = fitnessMachineControlPointCharacteristic.getValueHandle();
    fitnessMachineFeatureCharacteristicHandle = fitnessMachineFeatureCharacteristic.getValueHandle();
    fitnessMachineStatusCharacteristicHandle = fitnessMachineStatusCharacteristic.getValueHandle();
    fitnessTrainingStatusCharacteristicHandle = fitnessTrainingStatusCharacteristic.getValueHandle();
    fitnessSupportedResistanceLevelRangeCharacteristicHandle = fitnessSupportedResistanceLevelRangeCharacteristic.getValueHandle();

    // GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
    struct_pack(fitnessMachineFeatureCharacteristicBuffer
        , "<II"
        , FTMP_FLAGS_FITNESS_MACINE_FEATURES_FIELD
        , FTMP_FLAGS_TARGET_SETTING_FEATURES_FIELD
    );
    ble.gattServer().write(fitnessMachineFeatureCharacteristicHandle
        ,(uint8_t *)&fitnessMachineFeatureCharacteristicBuffer, sizeof(fitnessMachineFeatureCharacteristicBuffer));
    struct_pack(fitnessTrainingStatusCharacteristicBuffer
        , "<BB"
        , FTMP_FLAGS_TRAINING_STATUS_FIELD_00_STATUS_ONLY
        , FTMP_VAL_TRAINING_STATUS_01_IDEL
    );
    ble.gattServer().write(fitnessTrainingStatusCharacteristicHandle
        ,(uint8_t *)&fitnessTrainingStatusCharacteristicBuffer, sizeof(fitnessTrainingStatusCharacteristicBuffer));
    struct_pack(fitnessSupportedResistanceLevelRangeCharacteristicBuffer
        , "<hhH"
        , VAL_MINIMUM_RESISTANCE_LEVEL
        , VAL_MAXIMUM_RESISTANCE_LEVEL
        , VAL_INCREMENT_RESISTANCE_LEVEL
    );
    ble.gattServer().write(fitnessSupportedResistanceLevelRangeCharacteristicHandle
        ,(uint8_t *)&fitnessSupportedResistanceLevelRangeCharacteristicBuffer, sizeof(fitnessSupportedResistanceLevelRangeCharacteristicBuffer));
    
    // GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE - Fitness Machine Control Point Characteristic
    ble.onDataWritten(this, &MicroBitIndoorBikeMiniService::onDataWritten);
    
    // Microbit Event listen
    if (EventModel::defaultEventBus)
    {
        EventModel::defaultEventBus->listen(this->sensor.getId(), MICROBIT_INDOOR_BIKE_MINI_SENSOR_EVT_DATA_UPDATE
            , this, &MicroBitIndoorBikeMiniService::indoorBikeUpdate);
    }

}

void MicroBitIndoorBikeMiniService::onDataWritten(const GattWriteCallbackParams *params)
{
    if (params->handle == fitnessMachineControlPointCharacteristicHandle && params->len >= 1)
    {
        doFitnessMachineControlPoint(params);
    }
}

void MicroBitIndoorBikeMiniService::indoorBikeUpdate(MicroBitEvent e)
{
    if (ble.getGapState().connected)
    {
        if (e.source==this->sensor.getId())
        {
            struct_pack(indoorBikeDataCharacteristicBuffer, "<HHHhh",
                FTMP_FLAGS_INDOOR_BIKE_DATA_CHAR,
                this->sensor.getSpeed100(),
                this->sensor.getCadence2(),
                (int16_t)(this->getTargetResistanceLevel10()/10),
                this->sensor.getPower()
            );
            ble.gattServer().notify(indoorBikeDataCharacteristicHandle
                , (uint8_t *)&indoorBikeDataCharacteristicBuffer, sizeof(indoorBikeDataCharacteristicBuffer));
        }
    }
}

uint8_t MicroBitIndoorBikeMiniService::getTargetResistanceLevel10()
{
    return this->currentTargetResistanceLevel10;
}

void MicroBitIndoorBikeMiniService::setTargetResistanceLevel10(uint8_t level10)
{
    if (this->currentTargetResistanceLevel10!=level10
        && level10 >= VAL_MINIMUM_RESISTANCE_LEVEL
        && level10 <= VAL_MAXIMUM_RESISTANCE_LEVEL)
    {
        this->currentTargetResistanceLevel10 = level10;
        //this->indoorBike.setResistanceLevel10(level10);
    }
}

int16_t MicroBitIndoorBikeMiniService::getWindSpeed1000(void)
{
    return this->lastWindSpeed1000;
}

int16_t MicroBitIndoorBikeMiniService::getGrade100(void)
{
    return this->lastGrade100;
}

uint8_t MicroBitIndoorBikeMiniService::getCrr10000(void)
{
    return this->lastCrr10000;
}

uint8_t MicroBitIndoorBikeMiniService::getCw100(void)
{
    return this->lastCw100;
}


void MicroBitIndoorBikeMiniService::sendTrainingStatusIdle(void)
{
    static const uint8_t buff[]={FTMP_FLAGS_TRAINING_STATUS_FIELD_00_STATUS_ONLY, FTMP_VAL_TRAINING_STATUS_01_IDEL};
    ble.gattServer().notify(this->fitnessTrainingStatusCharacteristicHandle
        , (uint8_t *)&buff, sizeof(buff));
}

void MicroBitIndoorBikeMiniService::sendTrainingStatusManualMode(void)
{
    static const uint8_t buff[]={FTMP_FLAGS_TRAINING_STATUS_FIELD_00_STATUS_ONLY, FTMP_VAL_TRAINING_STATUS_0D_MANUAL_MODE};
    ble.gattServer().notify(this->fitnessTrainingStatusCharacteristicHandle
        , (uint8_t *)&buff, sizeof(buff));
}
    
void MicroBitIndoorBikeMiniService::sendFitnessMachineStatusReset(void)
{
    static const uint8_t buff[]={FTMP_OP_CODE_FITNESS_MACHINE_STATUS_01_RESET};
    ble.gattServer().notify(this->fitnessTrainingStatusCharacteristicHandle
        , (uint8_t *)&buff, sizeof(buff));
}

void MicroBitIndoorBikeMiniService::sendFitnessMachineStatusTargetResistanceLevelChanged(void)
{
    uint8_t buff[2];
    buff[0]=FTMP_OP_CODE_FITNESS_MACHINE_STATUS_07_TARGET_RESISTANCE_LEVEL_CHANGED;
    buff[1]=this->currentTargetResistanceLevel10;
    ble.gattServer().notify(this->fitnessMachineStatusCharacteristicHandle
        , (uint8_t *)&buff, sizeof(buff));
}

void MicroBitIndoorBikeMiniService::sendFitnessMachineStatusIndoorBikeSimulationParametersChanged(void)
{
    if (this->nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize>0)
    {
        ble.gattServer().notify(fitnessTrainingStatusCharacteristicHandle
            , (uint8_t *)&this->nextFitnessMachineStatusIndoorBikeSimulationParametersChanged
            , this->nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize);
    }
    
}

void MicroBitIndoorBikeMiniService::doFitnessMachineControlPoint(const GattWriteCallbackParams *params)
{
    uint8_t responseBuffer[3];
    responseBuffer[0] = FTMP_OP_CODE_CPPR_80_RESPONSE_CODE;
    uint8_t *opCode=&responseBuffer[1];
    opCode[0]=params->data[0];
    uint8_t *result=&responseBuffer[2];
    result[0] = FTMP_RESULT_CODE_CPPR_03_INVALID_PARAMETER;
    bool changedSimu=false;
    int16_t windSpeed1000=0;
    int16_t grade100=0;
    uint8_t crr10000=0;
    uint8_t cw100=0;
    switch (opCode[0])
    {
    case FTMP_OP_CODE_CPPR_00_REQUEST_CONTROL:
        if (params->len == 1)
        {
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_01_RESET:
        if (params->len == 1)
        {
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL:
        if (params->len == 2
            && params->data[1] >= VAL_MINIMUM_RESISTANCE_LEVEL
            && params->data[1] <= VAL_MAXIMUM_RESISTANCE_LEVEL)
        {
            this->currentTargetResistanceLevel10 = params->data[1];
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_07_START_RESUME:
        if (params->len == 1)
        {
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_08_STOP_PAUSE:
        if (params->len == 2)
        {
            //uint8_t stopOrPause = params->data[1];
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION:
        switch (params->len)
        {
        case 3:
            struct_unpack(&params->data[1], "<h", &windSpeed1000);
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            break;
        
        case 5:
            struct_unpack(&params->data[1], "<hh", &windSpeed1000, &grade100);
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            break;
        
        case 6:
            struct_unpack(&params->data[1], "<hhB", &windSpeed1000, &grade100, &crr10000);
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            break;
            
        case 7:
            struct_unpack(&params->data[1], "<hhBB", &windSpeed1000, &grade100, &crr10000, &cw100);
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            break;

        default:
            break;
        }
        if (result[0]==FTMP_RESULT_CODE_CPPR_01_SUCCESS)
        {
            changedSimu = true;
            // for sendFitnessMachineStatusIndoorBikeSimulationParametersChanged
            this->nextFitnessMachineStatusIndoorBikeSimulationParametersChanged[0]=FTMP_OP_CODE_FITNESS_MACHINE_STATUS_12_INDOOR_BIKE_SIMULATION_PARAMETERS_CHANGED;
            this->nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize=params->len;
            for (int i=1; i<params->len; i++ )
            {
                this->nextFitnessMachineStatusIndoorBikeSimulationParametersChanged[i] = params->data[i];
            }
            if (this->lastWindSpeed1000!=windSpeed1000)
            {
                changedSimu=true;
                this->lastWindSpeed1000=windSpeed1000;
            }
            if (this->lastGrade100!=grade100)
            {
                changedSimu=true;
                this->lastGrade100=grade100;
            }
            if (this->lastCrr10000!=crr10000)
            {
                changedSimu=true;
                this->lastCrr10000=crr10000;
            }
            if (this->lastCw100!=cw100)
            {
                changedSimu=true;
                this->lastCw100=cw100;
            }
        }
        break;

    default:
        result[0] = FTMP_RESULT_CODE_CPPR_02_NOT_SUPORTED;
        break;

    }

    // Response - Fitness Machine Control Point
    ble.gattServer().write(fitnessMachineControlPointCharacteristicHandle
            , (const uint8_t *)&responseBuffer, sizeof(responseBuffer));
    
    // Procedure
    switch (opCode[0])
    {
    case FTMP_OP_CODE_CPPR_00_REQUEST_CONTROL:
        // # 0x00 M Request Control
        // (NOP)
        break;
    case FTMP_OP_CODE_CPPR_01_RESET:
        // # 0x01 M Reset
        this->sendTrainingStatusManualMode();
        break;
    case FTMP_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL:
        // # 0x04 C.3 Set Target Resistance Level [UINT8, Level]
        // this->sendFitnessMachineStatusTargetResistanceLevelChanged();
        // (NOP)
        break;
    case FTMP_OP_CODE_CPPR_07_START_RESUME:
        // # 0x07 M Start or Resume
        this->sendTrainingStatusManualMode();
        break;
    case FTMP_OP_CODE_CPPR_08_STOP_PAUSE:
        // # 0x08 M Stop or Pause [UINT8, 0x01-STOP, 0x02-PAUSE]
        this->sendTrainingStatusIdle();
        break;
    case FTMP_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION:
        // # 0x11 C.14 Set Indoor Bike Simulation [SINT16, Wind Speed], [SINT16, Grade], [UINT8 CRR], [UINT8, CW]
        //this->sendFitnessMachineStatusIndoorBikeSimulationParametersChanged();
        // (NOP)
        break;
    default:
        break;
    }
    
}

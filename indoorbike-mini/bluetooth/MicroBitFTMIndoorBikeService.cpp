/*
MIT License

Copyright (c) 2021 jp-96

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
#include "MicroBitFTMIndoorBikeService.h"
#include "ble/UUID.h"
#include "struct/include/struct/struct.h"

MicroBitFTMIndoorBikeService::MicroBitFTMIndoorBikeService(BLEDevice &_ble, MicroBitIndoorBikeMiniSensor &_indoorBike)
    : ble(_ble), indoorBike(_indoorBike)
{
    
    this->targetResistanceLevel10=0;
    this->stopOrPause=0;
    this->windSpeed1000=0;
    this->grade100=0;
    this->crr10000=0;
    this->cw100=0;
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
    ble.onDataWritten(this, &MicroBitFTMIndoorBikeService::onDataWritten);
    
    // Microbit Event listen
    if (EventModel::defaultEventBus)
    {
        EventModel::defaultEventBus->listen(this->indoorBike.getId(), MICROBIT_INDOOR_BIKE_MINI_EVT_DATA_UPDATE
            , this, &MicroBitFTMIndoorBikeService::indoorBikeUpdate, MESSAGE_BUS_LISTENER_IMMEDIATE);
        EventModel::defaultEventBus->listen(CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE, MICROBIT_EVT_ANY
            , this, &MicroBitFTMIndoorBikeService::onFitnessMachineControlPoint, MESSAGE_BUS_LISTENER_IMMEDIATE);
    }

}

void MicroBitFTMIndoorBikeService::onDataWritten(const GattWriteCallbackParams *params)
{
    if (params->handle == fitnessMachineControlPointCharacteristicHandle && params->len >= 1)
    {
        uint8_t responseBuffer[3];
        responseBuffer[0] = FTMP_OP_CODE_CPPR_80_RESPONSE_CODE;
        uint8_t *opCode=&responseBuffer[1];
        opCode[0]=params->data[0];
        uint8_t *result=&responseBuffer[2];
        result[0] = FTMP_RESULT_CODE_CPPR_03_INVALID_PARAMETER;
        uint16_t eventValue;
        bool changedSimu=false;
        int16_t windSpeed1000=0;
        int16_t grade100=0;
        uint8_t crr10000=0;
        uint8_t cw100=0;
        switch (opCode[0])
        {
        case FTMP_OP_CODE_CPPR_00_REQUEST_CONTROL:
            eventValue=FTMP_EVENT_VAL_OP_CODE_CPPR_00_REQUEST_CONTROL;
            if (params->len == 1)
            {
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            }
            break;

        case FTMP_OP_CODE_CPPR_01_RESET:
            eventValue=FTMP_EVENT_VAL_OP_CODE_CPPR_01_RESET;
            if (params->len == 1)
            {
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            }
            break;

        case FTMP_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL:
            eventValue=FTMP_EVENT_VAL_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL;
            if (params->len == 2
             && params->data[1] >= VAL_MINIMUM_RESISTANCE_LEVEL
             && params->data[1] <= VAL_MAXIMUM_RESISTANCE_LEVEL)
            {
                this->targetResistanceLevel10 = params->data[1];
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            }
            break;

        case FTMP_OP_CODE_CPPR_07_START_RESUME:
            eventValue=FTMP_EVENT_VAL_OP_CODE_CPPR_07_START_RESUME;
            if (params->len == 1)
            {
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            }
            break;

        case FTMP_OP_CODE_CPPR_08_STOP_PAUSE:
            eventValue=FTMP_EVENT_VAL_OP_CODE_CPPR_08_STOP_PAUSE;
            if (params->len == 2)
            {
                this->stopOrPause = params->data[1];
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            }
            break;

        case FTMP_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION:
            eventValue=FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION;
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
                // for sendFitnessMachineStatusIndoorBikeSimulationParametersChanged
                this->nextFitnessMachineStatusIndoorBikeSimulationParametersChanged[0]=FTMP_OP_CODE_FITNESS_MACHINE_STATUS_12_INDOOR_BIKE_SIMULATION_PARAMETERS_CHANGED;
                this->nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize=params->len;
                for (int i=1; i++; i<params->len )
                {
                    this->nextFitnessMachineStatusIndoorBikeSimulationParametersChanged[i] = params->data[i];
                }
                if (this->windSpeed1000!=windSpeed1000)
                {
                    changedSimu=true;
                    this->windSpeed1000=windSpeed1000;
                }
                if (this->grade100!=grade100)
                {
                    changedSimu=true;
                    this->grade100=grade100;
                }
                if (this->crr10000!=crr10000)
                {
                    changedSimu=true;
                    this->crr10000=crr10000;
                }
                if (this->cw100!=cw100)
                {
                    changedSimu=true;
                    this->cw100=cw100;
                }
                if (changedSimu)
                {
                    eventValue=FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION_CHANGED;
                }
            }

        default:
            result[0] = FTMP_RESULT_CODE_CPPR_02_NOT_SUPORTED;
            break;

        }

        // Response - Fitness Machine Control Point
        ble.gattServer().write(fitnessMachineControlPointCharacteristicHandle
                , (const uint8_t *)&responseBuffer, sizeof(responseBuffer));
        
        // Fire MicroBit Event
        if (result[0]==FTMP_RESULT_CODE_CPPR_01_SUCCESS)
        {
            new MicroBitEvent(CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE, eventValue);
        }

    }
}

void MicroBitFTMIndoorBikeService::indoorBikeUpdate(MicroBitEvent e)
{
    if (ble.getGapState().connected)
    {
        struct_pack(indoorBikeDataCharacteristicBuffer, "<HHHhh",
            FTMP_FLAGS_INDOOR_BIKE_DATA_CHAR,
            this->indoorBike.getSpeed100(),
            this->indoorBike.getCadence2(),
            ((int16_t)this->getTargetResistanceLevel10())/10,
            this->indoorBike.getPower()
        );
        ble.gattServer().notify(indoorBikeDataCharacteristicHandle
            , (uint8_t *)&indoorBikeDataCharacteristicBuffer, sizeof(indoorBikeDataCharacteristicBuffer));
    }
}

uint8_t MicroBitFTMIndoorBikeService::getTargetResistanceLevel10()
{
    return this->targetResistanceLevel10;
}

void MicroBitFTMIndoorBikeService::setTargetResistanceLevel10(uint8_t level10)
{
    if (this->targetResistanceLevel10!=level10
        && level10 >= VAL_MINIMUM_RESISTANCE_LEVEL
        && level10 <= VAL_MAXIMUM_RESISTANCE_LEVEL)
    {
        this->targetResistanceLevel10 = level10;
        this->indoorBike.setResistanceLevel10(level10);
        new MicroBitEvent(CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE
                , FTMP_EVENT_VAL_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL);
    }
}

uint8_t MicroBitFTMIndoorBikeService::getStopOrPause()
{
    return this->stopOrPause;
}

int16_t MicroBitFTMIndoorBikeService::getWindSpeed1000(void)
{
    return this->windSpeed1000;
}

int16_t MicroBitFTMIndoorBikeService::getGrade100(void)
{
    return this->grade100;
}

uint8_t MicroBitFTMIndoorBikeService::getCrr10000(void)
{
    return this->crr10000;
}

uint8_t MicroBitFTMIndoorBikeService::getCw100(void)
{
    return this->cw100;
}


void MicroBitFTMIndoorBikeService::sendTrainingStatusIdle(void)
{
    static const uint8_t buff[]={FTMP_FLAGS_TRAINING_STATUS_FIELD_00_STATUS_ONLY, FTMP_VAL_TRAINING_STATUS_01_IDEL};
    ble.gattServer().notify(this->fitnessTrainingStatusCharacteristicHandle
        , (uint8_t *)&buff, sizeof(buff));
}

void MicroBitFTMIndoorBikeService::sendTrainingStatusManualMode(void)
{
    static const uint8_t buff[]={FTMP_FLAGS_TRAINING_STATUS_FIELD_00_STATUS_ONLY, FTMP_VAL_TRAINING_STATUS_0D_MANUAL_MODE};
    ble.gattServer().notify(this->fitnessTrainingStatusCharacteristicHandle
        , (uint8_t *)&buff, sizeof(buff));
}
    
void MicroBitFTMIndoorBikeService::sendFitnessMachineStatusReset(void)
{
    static const uint8_t buff[]={FTMP_OP_CODE_FITNESS_MACHINE_STATUS_01_RESET};
    ble.gattServer().notify(this->fitnessTrainingStatusCharacteristicHandle
        , (uint8_t *)&buff, sizeof(buff));
}

void MicroBitFTMIndoorBikeService::sendFitnessMachineStatusTargetResistanceLevelChanged(void)
{
    uint8_t buff[2];
    buff[0]=FTMP_OP_CODE_FITNESS_MACHINE_STATUS_07_TARGET_RESISTANCE_LEVEL_CHANGED;
    buff[1]=this->targetResistanceLevel10;
    ble.gattServer().notify(this->fitnessMachineStatusCharacteristicHandle
        , (uint8_t *)&buff, sizeof(buff));
}

void MicroBitFTMIndoorBikeService::sendFitnessMachineStatusIndoorBikeSimulationParametersChanged(void)
{
    if (this->nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize>0)
    {
        ble.gattServer().notify(fitnessTrainingStatusCharacteristicHandle
            , (uint8_t *)&this->nextFitnessMachineStatusIndoorBikeSimulationParametersChanged
            , this->nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize);
    }
    
}

void MicroBitFTMIndoorBikeService::onFitnessMachineControlPoint(MicroBitEvent e)
{
    switch (e.value)
    {
    case FTMP_EVENT_VAL_OP_CODE_CPPR_00_REQUEST_CONTROL:
        // # 0x00 M Request Control
        // #define FTMP_EVENT_VAL_OP_CODE_CPPR_00_REQUEST_CONTROL
        // (NOP)
        break;
    case FTMP_EVENT_VAL_OP_CODE_CPPR_01_RESET:
        // # 0x01 M Reset
        // #define FTMP_EVENT_VAL_OP_CODE_CPPR_01_RESET
        this->sendTrainingStatusManualMode();
        break;
    case FTMP_EVENT_VAL_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL:
        // # 0x04 C.3 Set Target Resistance Level [UINT8, Level]
        // #define FTMP_EVENT_VAL_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL
        // this->sendFitnessMachineStatusTargetResistanceLevelChanged();
        // (NOP)
        break;
    case FTMP_EVENT_VAL_OP_CODE_CPPR_07_START_RESUME:
        // # 0x07 M Start or Resume
        // #define FTMP_EVENT_VAL_OP_CODE_CPPR_07_START_RESUME
        this->sendTrainingStatusManualMode();
        break;
    case FTMP_EVENT_VAL_OP_CODE_CPPR_08_STOP_PAUSE:
        // # 0x08 M Stop or Pause [UINT8, 0x01-STOP, 0x02-PAUSE]
        // #define FTMP_EVENT_VAL_OP_CODE_CPPR_08_STOP_PAUSE
        this->sendTrainingStatusIdle();
        break;
    case FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION_CHANGED:
        // # 0x11 C.14 Set Indoor Bike Simulation [SINT16, Wind Speed], [SINT16, Grade], [UINT8 CRR], [UINT8, CW]
        // #define FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION
        // #define FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION_CHANGED
        // this->sendFitnessMachineStatusIndoorBikeSimulationParametersChanged();
        // (NOP)
        break;
    default:
        break;
    }
    
}

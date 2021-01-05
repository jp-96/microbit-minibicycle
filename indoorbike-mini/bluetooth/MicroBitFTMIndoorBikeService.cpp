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

MicroBitFTMIndoorBikeService::MicroBitFTMIndoorBikeService(BLEDevice &_ble, MicroBitIndoorBikeMiniSensor &_indoorBike) :
        ble(_ble), indoorBike(_indoorBike)
{
    
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
    }

}

void MicroBitFTMIndoorBikeService::onDataWritten(const GattWriteCallbackParams *params)
{
    if (params->handle == fitnessMachineControlPointCharacteristicHandle && params->len >= 1)
    {
        static const uint16_t bufferSize = 20;
        uint8_t responseBuffer[bufferSize];
        uint16_t responseIndex=0;
        responseBuffer[responseIndex++] = FTMP_OP_CODE_CPPR_80_RESPONSE_CODE;
        uint8_t *opCode=&responseBuffer[responseIndex++];
        opCode[0]=params->data[0];
        uint8_t *result=&responseBuffer[responseIndex++];
        result[0] = FTMP_RESULT_CODE_CPPR_03_INVALID_PARAMETER;
        for(int i=1;i++;i<(params->len-1))
        {
            responseBuffer[responseIndex++]=params->data[i];
            if (responseIndex>=bufferSize)
            {
                break;
            }
        }
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
                this->stopOrRause = 0x01;
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            }
            break;

        case FTMP_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL:
            if (params->len == 2
             && params->data[1] >= VAL_MINIMUM_RESISTANCE_LEVEL
             && params->data[1] <= VAL_MAXIMUM_RESISTANCE_LEVEL)
            {
                this->targetResistanceLevel10 = params->data[1];
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            }
            break;

        case FTMP_OP_CODE_CPPR_07_START_RESUME:
            if (params->len == 1)
            {
                this->stopOrRause = 0x00;
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            }
            break;

        case FTMP_OP_CODE_CPPR_08_STOP_PAUSE:
            if (params->len == 2)
            {
                this->stopOrRause = params->data[1];
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            }
            break;

        case FTMP_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION:
            switch (params->len)
            {
            case 3:
                //if s==2:
                //    (self._windSpeed1000,) = struct.unpack("<h", self.parameter)
            
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
                break;
            
            case 5:
                //elif s==4:
                //    (self._windSpeed1000, self._grade100,) = struct.unpack("<hh", self.parameter)
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
                break;
            
            case 6:
                //elif s==5:
                //    (self._windSpeed1000, self._grade100, self._crr10000,) = struct.unpack("<hhb", self.parameter)
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
                break;
                
            case 7:
                //elif s==6:
                //    (self._windSpeed1000, self._grade100, self._crr10000, self._cw100,) = struct.unpack("<hhBB", self.parameter)
                result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
                break;

            default:
                break;
            }

        default:
            result[0] = FTMP_RESULT_CODE_CPPR_02_NOT_SUPORTED;
            break;

        }

        // Response - Fitness Machine Control Point
        ble.gattServer().write(fitnessMachineControlPointCharacteristicHandle
                , (const uint8_t *)&responseBuffer, responseIndex);
        
        // Fire MicroBit Event
        if (result[0]==FTMP_RESULT_CODE_CPPR_01_SUCCESS)
        {
            new MicroBitEvent(CUSTOM_EVENT_ID_FITNESS_MACHINE_CONTROL_POINT, opCode[0]);
        }

    }
}

void MicroBitFTMIndoorBikeService::indoorBikeUpdate(MicroBitEvent e)
{
    if (ble.getGapState().connected)
    {
        struct_pack(indoorBikeDataCharacteristicBuffer, "<HHH",
            FTMP_FLAGS_INDOOR_BIKE_DATA_CHAR,
            this->indoorBike.getSpeed100(),
            this->indoorBike.getCadence2()
        );
        ble.gattServer().notify(indoorBikeDataCharacteristicHandle
            ,(uint8_t *)&indoorBikeDataCharacteristicBuffer, sizeof(indoorBikeDataCharacteristicBuffer));
    }
}

uint8_t MicroBitFTMIndoorBikeService::getTargetResistanceLevel10()
{
    return this->targetResistanceLevel10;
}

bool MicroBitFTMIndoorBikeService::isStop()
{
    return this->stopOrRause == 0x01;
}

bool MicroBitFTMIndoorBikeService::isPause()
{
    return this->stopOrRause == 0x02;
}


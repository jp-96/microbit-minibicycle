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
    
    /* CHAR_INDOOR_BIKE_DATA
        _CHAR_INDOOR_BIKE_DATA = (
            bluetooth.UUID(0x2AD2),
            bluetooth.FLAG_NOTIFY,
        )
    */
    GattCharacteristic  indoorBikeDataCharacteristic(UUID(0x2AD2)
        , (uint8_t *)&indoorBikeDataCharacteristicBuffer, 0, sizeof(indoorBikeDataCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);

    /* CHAR_FITNESS_MACHINE_CONTROL_POINT
        _CHAR_FITNESS_MACHINE_CONTROL_POINT = (
            bluetooth.UUID(0x2AD9),
            bluetooth.FLAG_WRITE | _FLAG_INDICATE ,
        )
    */
    GattCharacteristic  fitnessMachineControlPointCharacteristic(UUID(0x2AD9)
        , (uint8_t *)&fitnessMachineControlPointDataCharacteristicBuffer, 0, sizeof(fitnessMachineControlPointDataCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE|GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE);

    /* CHAR_FITNESS_MACHINE_FEATURE
        _CHAR_FITNESS_MACHINE_FEATURE = (
            bluetooth.UUID(0x2ACC),
            bluetooth.FLAG_READ,
        )
    */
    
    /* CHAR_FITNESS_MACHINE_STATUS
        _CHAR_FITNESS_MACHINE_STATUS = (
            bluetooth.UUID(0x2ADA),
            bluetooth.FLAG_NOTIFY,
        )
    */

    /* CHAR_TRAINING_STATUS
        _CHAR_TRAINING_STATUS = (
            bluetooth.UUID(0x2AD3),
            bluetooth.FLAG_READ | bluetooth.FLAG_NOTIFY,
        )
    */
    
    /* CHAR_SUPPORTED_RESISTANCE_LEVEL_RANGE
        _CHAR_SUPPORTED_RESISTANCE_LEVEL_RANGE = (
            bluetooth.UUID(0x2AD6),
            bluetooth.FLAG_READ,
        )
    */

    // Set default security requirements
    indoorBikeDataCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessMachineControlPointCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattCharacteristic *characteristics[] = {
        &indoorBikeDataCharacteristic,
        &fitnessMachineControlPointCharacteristic,
    };
    GattService         service(UUID(0x1826), characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    indoorBikeDataCharacteristicHandle = indoorBikeDataCharacteristic.getValueHandle();
    fitnessMachineControlPointCharacteristicHandle = fitnessMachineControlPointCharacteristic.getValueHandle();
    
    //ble.gattServer().write(temperatureDataCharacteristicHandle,(uint8_t *)&temperatureDataCharacteristicBuffer, sizeof(temperatureDataCharacteristicBuffer));
    
    ble.onDataWritten(this, &MicroBitFTMIndoorBikeService::onDataWritten);
    if (EventModel::defaultEventBus)
        EventModel::defaultEventBus->listen(this->indoorBike.getId(), MICROBIT_THERMOMETER_EVT_UPDATE, this, &MicroBitFTMIndoorBikeService::indoorBikeUpdate, MESSAGE_BUS_LISTENER_IMMEDIATE);
}

void MicroBitFTMIndoorBikeService::indoorBikeUpdate(MicroBitEvent e)
{
    if (ble.getGapState().connected)
    {
        struct_pack(indoorBikeDataCharacteristicBuffer, "<HHH",
            FLAGS_INDOOR_BIKE_DATA_CHAR,
            this->indoorBike.getSpeed100(),
            this->indoorBike.getCadence2()
        );
        ble.gattServer().notify(indoorBikeDataCharacteristicHandle,(uint8_t *)&indoorBikeDataCharacteristicBuffer, sizeof(indoorBikeDataCharacteristicBuffer));
    }
}

void MicroBitFTMIndoorBikeService::onDataWritten(const GattWriteCallbackParams *params)
{
    if (params->handle == fitnessMachineControlPointCharacteristicHandle && params->len >= 1)
    {
        if (params->data[0]==0x04)
        {
            this->targetResistanceLevel10 = params->data[1];
            MicroBitEvent e(CUSTOM_EVENT_ID_FITNESS_MACHINE_CONTROL_POINT, OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL);
        }
        //temperaturePeriodCharacteristicBuffer = *((uint16_t *)params->data);
        //thermometer.setPeriod(temperaturePeriodCharacteristicBuffer);

        // The accelerometer will choose the nearest period to that requested that it can support
        // Read back the ACTUAL period it is using, and report this back.
        //temperaturePeriodCharacteristicBuffer = thermometer.getPeriod();
        //ble.gattServer().write(temperaturePeriodCharacteristicHandle, (const uint8_t *)&temperaturePeriodCharacteristicBuffer, sizeof(temperaturePeriodCharacteristicBuffer));
    }
}

uint8_t MicroBitFTMIndoorBikeService::getTargetResistanceLevel10()
{
    return this->targetResistanceLevel10;
}

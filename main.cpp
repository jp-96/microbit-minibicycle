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

#include "MicroBit.h"
#include "indoorbike-mini/driver/MicroBitIndoorBikeMiniSensor.h"

#define BLE_DEVICE_LOCAL_NAME "FTMS:BIT"
#ifndef BLE_DEVICE_LOCAL_NAME_CHENGE
#define BLE_DEVICE_LOCAL_NAME_CHENGE 1
#endif /* #ifndef BLE_DEVICE_LOCAL_NAME_CHENGE */

MicroBit uBit;
MicroBitIndoorBikeMiniSensor sensor(uBit);

// Button Event
void onButton(MicroBitEvent e)
{
    //uBit.serial.printf("onButton()\r\n");
    if ((e.source == MICROBIT_ID_BUTTON_A) && (e.value == MICROBIT_BUTTON_EVT_CLICK))
    {
        sensor.onCrankSensor(e);
    }
}

void onSensorUpdate(MicroBitEvent e)
{
    //uBit.serial.printf("onUpdate()\r\n");
    uBit.serial.printf("T, %d, C, %d, S, %d\r\n"
        , sensor.getIntervalTime()
        , sensor.getCadence()
        , sensor.getSpeed100()
    );
}


int main()
{
    uBit.init();
    
    // BLE Appearance and LOCAL_NAME
    uBit.ble->gap().accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_CYCLING);
    if (BLE_DEVICE_LOCAL_NAME_CHENGE)
    {
        uBit.ble->gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (const uint8_t *)BLE_DEVICE_LOCAL_NAME, sizeof(BLE_DEVICE_LOCAL_NAME)-1);
    }

    // Register for Button Events on Button(A)
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButton);
    
    // IdleTick
    uBit.addIdleComponent(&sensor);
    uBit.messageBus.listen(CUSTOM_EVENT_ID_INDOORBIKE_MINI, MICROBIT_EVT_ANY, onSensorUpdate);

    release_fiber();
}

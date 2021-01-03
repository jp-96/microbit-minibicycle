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
#include "IndoorBikeMini.h"
#include "indoorbike-mini/driver/MicroBitIndoorBikeMiniSensor.h"
#include "indoorbike-mini/bluetooth/MicroBitFTMIndoorBikeService.h"

#define BLE_DEVICE_LOCAL_NAME "FTMS:BIT"
#ifndef BLE_DEVICE_LOCAL_NAME_CHENGE
#define BLE_DEVICE_LOCAL_NAME_CHENGE 1
#endif /* #ifndef BLE_DEVICE_LOCAL_NAME_CHENGE */

MicroBit uBit;
MicroBitIndoorBikeMiniSensor sensor(uBit);
MicroBitFTMIndoorBikeService* ftms;

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
        , sensor.getCadence2()/2
        , sensor.getSpeed100()
    );
}

void onFtms(MicroBitEvent e)
{
    //uBit.serial.printf("onBLE, Source, %d, Value, %d\r\n", e.source, e.value);
    if (e.source==CUSTOM_EVENT_ID_FITNESS_MACHINE_CONTROL_POINT && e.value == OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL)
    {
        uint8_t lv10 = ftms->getTargetResistanceLevel10();
        int angle = 90;
        if (lv10<=10)     //10  40
        {
            angle = 40;
        }
        else if(lv10<=20) //20  60
        {
            angle = 60;
        }
        else if(lv10<=30) //30  70
        {
            angle = 70;
        }
        else if(lv10<=40) //40  80
        {
            angle = 80;
        }
        else if(lv10<=50) //50  85
        {
            angle = 85;
        }
        else if(lv10<=60) //60  90
        {
            angle = 90;
        }
        else if(lv10<=70) //70  95
        {
            angle = 95;
        }
        else              //80 100
        {
            angle =100;
        }
        uBit.io.P1.setServoValue(angle);
        uBit.serial.printf("Servo Angle: %d\r\n", angle);
    }
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

    // BLE
    ftms = new MicroBitFTMIndoorBikeService(*(uBit.ble), sensor);
    uBit.messageBus.listen(CUSTOM_EVENT_ID_FITNESS_MACHINE_CONTROL_POINT, MICROBIT_EVT_ANY, onFtms);

    release_fiber();
}

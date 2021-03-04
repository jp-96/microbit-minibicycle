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

#include "inttypes.h"
#include "MicroBit.h"
#include "MicroBitCustom.h"
#include "MicroBitIndoorBikeMiniSensor.h"
#include "MicroBitIndoorBikeMiniServo.h"
#include "MicroBitIndoorBikeMiniController.h"
#include "MicroBitIndoorBikeMiniService.h"

#ifndef BLE_DEVICE_LOCAL_NAME_CHENGE
#define BLE_DEVICE_LOCAL_NAME_CHENGE 0
#endif /* #ifndef BLE_DEVICE_LOCAL_NAME_CHENGE */

MicroBit uBit;

MicroBitIndoorBikeMiniSensor* sensor;
MicroBitIndoorBikeMiniServo* servo;
MicroBitIndoorBikeMiniController* controller;
MicroBitIndoorBikeMiniService* service;

// Recalc indoor bike data.
void calcIndoorBikeData(MicroBitIndoorBikeMiniController* sender, uint8_t resistanceLevel10, uint32_t crankIntervalTime, uint32_t* cadence2, uint32_t* speed100, int32_t*  power)
{
    *power = *power * 1.2;
}

// Button Event
void onButton(MicroBitEvent e)
{
    switch (e.source)
    {
    case MICROBIT_ID_BUTTON_A:
        controller->decrementTargetResistanceLevel10();
        break;
    
    case MICROBIT_ID_BUTTON_B:
        controller->incrementTargetResistanceLevel10();
        break;
    
    default:
        break;
    }
}

void setup()
{
    // BLE Appearance and LOCAL_NAME
    uBit.ble->gap().accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_CYCLING);
    if (BLE_DEVICE_LOCAL_NAME_CHENGE)
    {
        uBit.ble->gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME
            , (const uint8_t *)BLE_DEVICE_LOCAL_NAME, sizeof(BLE_DEVICE_LOCAL_NAME)-1);
    }

    // Register for Button Events on Button(A)
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButton);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButton);
    
    // driver
    sensor = new MicroBitIndoorBikeMiniSensor(uBit);
    servo = new MicroBitIndoorBikeMiniServo(uBit);
    //servo->idleTick();
    controller = new MicroBitIndoorBikeMiniController(uBit, *sensor, *servo, calcIndoorBikeData);
    controller->idleTick();
    
    // MicroBitIndoorBikeMiniService
    service = new MicroBitIndoorBikeMiniService(*(uBit.ble), *controller);

}

void displayTargetResistanceLevel()
{
    while (true)
    {
        uBit.display.print(ManagedString((int)controller->getTargetResistanceLevel10()/10));
        uBit.sleep(100);
    }
    
}

int main()
{
    uBit.init();
    create_fiber(setup);
    create_fiber(displayTargetResistanceLevel);
    release_fiber();
}

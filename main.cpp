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
#include "indoorbike-mini/driver/MicroBitIndoorBikeMiniServo.h"
#include "indoorbike-mini/driver/MicroBitIndoorBikeMiniSensor.h"
#include "indoorbike-mini/bluetooth/MicroBitFTMIndoorBikeService.h"

#define BLE_DEVICE_LOCAL_NAME "FTMS:BIT"
#ifndef BLE_DEVICE_LOCAL_NAME_CHENGE
#define BLE_DEVICE_LOCAL_NAME_CHENGE 1
#endif /* #ifndef BLE_DEVICE_LOCAL_NAME_CHENGE */

MicroBit uBit;

MicroBitIndoorBikeMiniServo* servo;
MicroBitIndoorBikeMiniSensor* sensor;
MicroBitFTMIndoorBikeService* ftms;

// Button Event
void onButton(MicroBitEvent e)
{
    switch (e.source)
    {
    case MICROBIT_ID_BUTTON_A:
        uBit.serial.printf("%lu, MICROBIT_ID_BUTTON_A()\r\n", e.timestamp);
        servo->decrementTargetResistanceLevel10();
        ftms->setTargetResistanceLevel10(servo->getTargetResistanceLevel10());
        break;
    
    case MICROBIT_ID_BUTTON_B:
        uBit.serial.printf("%lu, MICROBIT_ID_BUTTON_B()\r\n", e.timestamp);
        servo->incrementTargetResistanceLevel10();
        ftms->setTargetResistanceLevel10(servo->getTargetResistanceLevel10());
        break;
    
    default:
        break;
    }
}

void onServoUpdate(MicroBitEvent e)
{
    uBit.serial.printf("%lu, onServoUpdate()\r\n", e.timestamp);
    uBit.display.print(ManagedString((int)servo->getTargetResistanceLevel10()/10));
}

void onSensorUpdate(MicroBitEvent e)
{
    //uBit.serial.printf("%lu, onSensorUpdate()\r\n", e.timestamp);
    uBit.serial.printf("%lu, T, %d, C, %d, S, %d\r\n", e.timestamp
        , sensor->getIntervalTime()
        , sensor->getCadence2()/2
        , sensor->getSpeed100()
    );
}

// ID: CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE
// VAL: FTMP_EVENT_VAL_OP_CODE_CPPR_01_RESET
void onReset(MicroBitEvent e)
{
    uBit.serial.printf("%lu, onReset()\r\n", e.timestamp);
    ftms->setTargetResistanceLevel10(20);
}

// ID: CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE
// VAL: FTMP_EVENT_VAL_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL
void onSetTargetResistanceLevel(MicroBitEvent e)
{
    uBit.serial.printf("%lu, onSetTargetResistanceLevel()\r\n", e.timestamp);
    uint8_t lv10 = ftms->getTargetResistanceLevel10();
    servo->setTargetResistanceLevel10(lv10);
    ftms->sendFitnessMachineStatusTargetResistanceLevelChanged();
    //uBit.serial.printf("Servo Angle: %d\r\n", servo->getServoAngle());
}

// ID: CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE
// VAL: FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION_CHANGED
void onSimulationChanged(MicroBitEvent e)
{
    uBit.serial.printf("%lu, onSimulationChanged()\r\n", e.timestamp);
    int16_t grade100 = ftms->getGrade100();
    if (grade100<0)
    {
        ftms->setTargetResistanceLevel10(10);
    }
    else if (grade100<100)
    {
        ftms->setTargetResistanceLevel10(20);
    }
    else if (grade100<150)
    {
        ftms->setTargetResistanceLevel10(30);
    }
    else if (grade100<200)
    {
        ftms->setTargetResistanceLevel10(40);
    }
    else if (grade100<250)
    {
        ftms->setTargetResistanceLevel10(50);
    }
    else if (grade100<300)
    {
        ftms->setTargetResistanceLevel10(60);
    }
    else if (grade100<350)
    {
        ftms->setTargetResistanceLevel10(70);
    }
    else
    {
        ftms->setTargetResistanceLevel10(80);
    }
}

void calcIndoorBikeData(uint8_t resistanceLevel10, uint32_t crankIntervalTime, uint32_t* cadence2, uint32_t* speed100, int32_t*  power)
{
    *power = *power * 1.5;
}

void init()
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
    
    // IdleTick
    servo = new MicroBitIndoorBikeMiniServo(uBit);
    sensor = new MicroBitIndoorBikeMiniSensor(uBit, calcIndoorBikeData);
    uBit.addIdleComponent(servo);
    uBit.addIdleComponent(sensor);
    uBit.messageBus.listen(CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO, MICROBIT_EVT_ANY, onServoUpdate);
    uBit.messageBus.listen(CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR, MICROBIT_EVT_ANY, onSensorUpdate);

    // FTMS
    ftms = new MicroBitFTMIndoorBikeService(*(uBit.ble), *sensor);
    uBit.messageBus.listen(CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE
        , FTMP_EVENT_VAL_OP_CODE_CPPR_01_RESET, onReset);
    uBit.messageBus.listen(CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE
        , FTMP_EVENT_VAL_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL, onSetTargetResistanceLevel);
    uBit.messageBus.listen(CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE
        , FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION_CHANGED, onSimulationChanged);
    
    // 
    fiber_sleep(0);

    servo->setTargetResistanceLevel10(20);
    ftms->setTargetResistanceLevel10(servo->getTargetResistanceLevel10());
}

int main()
{
    uBit.init();
    create_fiber(init);
    release_fiber();
}

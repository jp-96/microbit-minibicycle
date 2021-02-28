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

#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_H
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_H

#include "MicroBit.h"
#include "MicroBitCustom.h"
#include "MicroBitCustomComponent.h"

typedef int (*FuncCalcAngle)(uint8_t targetResistanceLevel10);

/**
  * Status flags
  */
// Universal flags used as part of the status field
// #define MICROBIT_COMPONENT_RUNNING		0x01
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_ADDED_TO_IDLE              0x02

enum MicrobitIndoorBikeMiniServoPin
{
    SERVO_P0 = 0,
    SERVO_P1 = 1,
    SERVO_P2 = 2
};

enum MicrobitIndoorBikeMiniServoState
{
    SERVO_NONE,
    SERVO_ON,
    SERVO_OFF
};

class MicroBitIndoorBikeMiniServo : public MicroBitCustomComponent
{
private:
    MicroBit &uBit;
    
    //                                         654321
    static const uint64_t SERVO_PERIOD_ON  = 10000000; // 10s
    static const uint64_t SERVO_PERIOD_OFF = 10000000; // 10s

    MicroBitPin* servoPin;
    
    uint64_t nextServoControlTimestamp;
    MicrobitIndoorBikeMiniServoState servoState;

public:
    MicroBitIndoorBikeMiniServo(MicroBit &_uBit, FuncCalcAngle _pFuncCalcAngle = NULL, MicrobitIndoorBikeMiniServoPin pin = SERVO_P1);

    /**
      * Periodic callback from MicroBit idle thread.
      */
    virtual void idleTick();

private:
    void update(bool force = false);

private:
    FuncCalcAngle pFuncCalcAngle;
    uint8_t targetResistanceLevel10;
    int servoAngle;

public:
    void setTargetResistanceLevel10(uint8_t targetResistanceLevel10);
    uint8_t getTargetResistanceLevel10(void);

};

#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_H */

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

#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_H
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_H

#include "MicroBit.h"
#include "../IndoorBikeMini.h"

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
    SERVO_FREE,
    SERVO_ON,
    SERVO_OFF
};

class MicroBitIndoorBikeMiniServo : public MicroBitComponent
{
private:
    MicroBit &uBit;
    
    static const uint64_t SERVO_PERIOD_ON  = 100000; // 0.1s
    static const uint64_t SERVO_PERIOD_OFF =3000000; // 3.0s

    MicroBitPin* servoPin;
    
    uint16_t nextServoControlTimestamp;
    MicrobitIndoorBikeMiniServoState servoState;

public:
    // Constructor.
    MicroBitIndoorBikeMiniServo(MicroBit &_uBit, FuncCalcAngle _pFuncCalcAngle = NULL, MicrobitIndoorBikeMiniServoPin pin = SERVO_P1, uint16_t id = CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO);

    /**
      * The system timer will call this member function once the component has been added to
      * the array of system components using system_timer_add_component. This callback
      * will be in interrupt context.
      */
    virtual void systemTick();

    /**
      * Periodic callback from MicroBit idle thread.
      */
    virtual void idleTick();

    // Event ID
    uint16_t getId(void);

private:
    void update(MicrobitIndoorBikeMiniServoState force = SERVO_NONE);

private:
    FuncCalcAngle pFuncCalcAngle;
    uint8_t targetResistanceLevel10;
    int servoAngle;

public:
    void setTargetResistanceLevel10(uint8_t targetResistanceLevel10);
    int getServoAngle(void);
    void setServoFree(void);
    void incrementTargetResistanceLevel10(void);
    void decrementTargetResistanceLevel10(void);

};

#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_H */

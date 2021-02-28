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

#ifndef MICROBIT_INDOOR_BIKE_MINI_CONTROLLER_H
#define MICROBIT_INDOOR_BIKE_MINI_CONTROLLER_H

#include "MicroBit.h"
#include "MicroBitCustom.h"
#include "MicroBitCustomComponent.h"
#include "MicroBitIndoorBikeMiniSensor.h"
#include "MicroBitIndoorBikeMiniServo.h"

typedef void (*FuncCalcIndoorBikeData)(uint8_t resistanceLevel10, uint32_t crankIntervalTime, uint32_t* cadence2, uint32_t* speed100, int32_t*  power);

/**
  * Status flags
  */
// Universal flags used as part of the status field
// #define MICROBIT_COMPONENT_RUNNING		0x01
#define MICROBIT_INDOOR_BIKE_MINI_CONTROLLER_ADDED_TO_IDLE              0x02

class MicroBitIndoorBikeMiniController : public MicroBitCustomComponent
{

private:
    MicroBit &uBit;
    MicroBitIndoorBikeMiniSensor &sensor;
    MicroBitIndoorBikeMiniServo &servo;

    static const uint64_t SENSOR_UPDATE_PERIOD_US =    1000000; // 1s
    
public:
    /**
      * Constructor.
      * Create a representation of the IndoorBike-Mini Controller
      * @param _uBit An instance of MicroBit.
      * @param _sensor An instance of MicroBitIndoorBikeMiniSensor to use as our indoor bike source.
      * @param _servo An instance of MicroBitIndoorBikeMiniServo to use as our indoor bike resistance level.
      * @param _pFuncCalcIndoorBikeData (option) A function pointer of calc Indoor Bike Data.
      * @param id (option) CUSTOM_EVENT_ID_INDOORBIKE_MINI_CONTROLLER
      */
    MicroBitIndoorBikeMiniController(MicroBit &_uBit, MicroBitIndoorBikeMiniSensor &_sensor, MicroBitIndoorBikeMiniServo &_servo, FuncCalcIndoorBikeData _pFuncCalcIndoorBikeData = NULL, uint16_t id = CUSTOM_EVENT_ID_INDOORBIKE_MINI_CONTROLLER);

    /**
      * Periodic callback from MicroBit idle thread.
      */
    virtual void idleTick();

private:
    uint64_t updateSampleTimestamp;
    FuncCalcIndoorBikeData pFuncCalcIndoorBikeData;
    void update();

public:
    // Coefficient of Cadence and Speed
    static const uint64_t K_CRANK_CADENCE =  120000000;
    static const uint64_t K_CRANK_SPEED   = 1800000000;

    // https://diary.cyclekikou.net/archives/15876
    static const double K_POWER = 0.8 * (70 * 9.80665) / (360 * 0.95 * 100); // weight(70kg)
    static const double K_INCLINE_A = 0.9; // Incline(%) - a
    static const double K_INCLINE_B = 0.6; // Incline(%) - b

private:
    uint32_t lastCadence2;
    uint32_t lastSpeed100;
    int32_t  lastPower;

public:
    uint32_t getCadence2(void);
    uint32_t getSpeed100(void);
    int32_t  getPower(void);

public:
    uint8_t getTargetResistanceLevel10(void);
    void setTargetResistanceLevel10(uint8_t resistanceLevel10);

    void setIndoorBikeSimulation(int16_t windSpeed1000, int16_t grade100, uint8_t crr10000, uint8_t cw100);
};

#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_CONTROLLER_H */

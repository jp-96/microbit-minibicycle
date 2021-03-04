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

#include "MicroBitIndoorBikeMiniController.h"

void defaultCalcIndoorBikeData(MicroBitIndoorBikeMiniController* sender, uint8_t resistanceLevel10, uint32_t crankIntervalTime, uint32_t* cadence2, uint32_t* speed100, int32_t*  power)
{
    
    if (crankIntervalTime==0)
    {
        *cadence2 = 0;
        *speed100 = 0;
        *power = 0;
    }
    else
    {
        *cadence2  = (uint32_t)( MicroBitIndoorBikeMiniController::K_CRANK_CADENCE / crankIntervalTime );
        *speed100 = (uint32_t)( MicroBitIndoorBikeMiniController::K_CRANK_SPEED   / crankIntervalTime );
        // https://diary.cyclekikou.net/archives/15876
        *power = (int32_t)( (double)(*speed100) * (MicroBitIndoorBikeMiniController::K_INCLINE_A * ((double)resistanceLevel10)/10 + MicroBitIndoorBikeMiniController::K_INCLINE_B) * MicroBitIndoorBikeMiniController::K_POWER);
    }
}

uint8_t defaultCalcTargetResistanceLevel10(MicroBitIndoorBikeMiniController* sender, int16_t windSpeed1000, int16_t grade100, uint8_t crr10000, uint8_t cw100)
{
    uint8_t resistanceLevel10;
    if (grade100<-100)
    {
        resistanceLevel10 = 10;
    }
    else if (grade100<100)
    {
        resistanceLevel10 = 20;
    }
    else if (grade100<300)
    {
        resistanceLevel10 = 30;
    }
    else if (grade100<500)
    {
        resistanceLevel10 = 40;
    }
    else if (grade100<700)
    {
        resistanceLevel10 = 50;
    }
    else if (grade100<900)
    {
        resistanceLevel10 = 60;
    }
    else if (grade100<1100)
    {
        resistanceLevel10 = 70;
    }
    else
    {
        resistanceLevel10 = 80;
    }
    return resistanceLevel10;
}


MicroBitIndoorBikeMiniController::MicroBitIndoorBikeMiniController(MicroBit &_uBit, MicroBitIndoorBikeMiniSensor &_sensor, MicroBitIndoorBikeMiniServo &_servo, FuncCalcIndoorBikeData _pFuncCalcIndoorBikeData, FuncCalcTargetResistanceLevel10 _pFuncCalcTargetResistanceLevel10, uint16_t id)
    : uBit(_uBit), sensor(_sensor), servo(_servo), pFuncCalcIndoorBikeData(_pFuncCalcIndoorBikeData), pFuncCalcTargetResistanceLevel10(_pFuncCalcTargetResistanceLevel10)
{
    this->id = id;
    this->lastCadence2=0;
    this->lastSpeed100=0;
    this->lastPower=0;
    this->updateSampleTimestamp=0;
    this->servo.setTargetResistanceLevel10(VAL_MINIMUM_RESISTANCE_LEVEL);
}

void MicroBitIndoorBikeMiniController::idleTick()
{
    this->update();
}

void MicroBitIndoorBikeMiniController::update(void)
{
    uint64_t currentTime = system_timer_current_time_us();
    if(!(status & MICROBIT_INDOOR_BIKE_MINI_CONTROLLER_ADDED_TO_IDLE))
    {
        // If we're running under a fiber scheduer, register ourselves for a periodic callback to keep our data up to date.
        // Otherwise, we do just do this on demand, when polled through our read() interface.
        fiber_add_idle_component(this);
        status |= MICROBIT_INDOOR_BIKE_MINI_CONTROLLER_ADDED_TO_IDLE;
    }
    
    if (currentTime >= this->updateSampleTimestamp)
    {        
        // Schedule our next sample.
        this->updateSampleTimestamp = currentTime + this->SENSOR_UPDATE_PERIOD_US;
        
        uint8_t resistanceLevel10 = this->servo.getTargetResistanceLevel10();
        uint32_t crankIntervalTime = this->sensor.getCrankIntervalTime();
        defaultCalcIndoorBikeData(this, resistanceLevel10, crankIntervalTime, &this->lastCadence2, &this->lastSpeed100, &this->lastPower);
        if (this->pFuncCalcIndoorBikeData)
        {
            (*this->pFuncCalcIndoorBikeData)(this, resistanceLevel10, crankIntervalTime, &this->lastCadence2, &this->lastSpeed100, &this->lastPower);
        }
        // Send an event to indicate that we'e updated our sensor.
        MicroBitEvent e(id, MICROBIT_INDOOR_BIKE_MINI_CONTROLLER_EVT_DATA_UPDATE);
    }
}

uint32_t MicroBitIndoorBikeMiniController::getCadence2(void)
{
    return this->lastCadence2;
}

uint32_t MicroBitIndoorBikeMiniController::getSpeed100(void)
{
    return this->lastSpeed100;
}

int32_t  MicroBitIndoorBikeMiniController::getPower(void)
{
    return this->lastPower;
}

void MicroBitIndoorBikeMiniController::setTargetResistanceLevel10(uint8_t targetResistanceLevel10)
{
    this->servo.setTargetResistanceLevel10(targetResistanceLevel10);
}

uint8_t MicroBitIndoorBikeMiniController::getTargetResistanceLevel10(void)
{
    return this->servo.getTargetResistanceLevel10();
}

bool MicroBitIndoorBikeMiniController::incrementTargetResistanceLevel10(void)
{
    uint8_t org = this->getTargetResistanceLevel10();
    this->setTargetResistanceLevel10(this->getTargetResistanceLevel10() + VAL_INCREMENT_RESISTANCE_LEVEL);
    return org!=this->getTargetResistanceLevel10();
}

bool MicroBitIndoorBikeMiniController::decrementTargetResistanceLevel10(void)
{
    uint8_t org = this->getTargetResistanceLevel10();
    this->setTargetResistanceLevel10(this->getTargetResistanceLevel10() - VAL_INCREMENT_RESISTANCE_LEVEL);
    return org!=this->getTargetResistanceLevel10();
}

void MicroBitIndoorBikeMiniController::setIndoorBikeSimulation(int16_t windSpeed1000, int16_t grade100, uint8_t crr10000, uint8_t cw100)
{   
    FuncCalcTargetResistanceLevel10 pFunc;
    if (this->pFuncCalcTargetResistanceLevel10)
    {
        pFunc = this->pFuncCalcTargetResistanceLevel10;
    }
    else
    {
        pFunc = defaultCalcTargetResistanceLevel10;
    }
    this->setTargetResistanceLevel10(pFunc(this, windSpeed1000, grade100, crr10000, cw100));
}
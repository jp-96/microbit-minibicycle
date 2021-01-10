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
#include "MicroBitIndoorBikeMiniSensor.h"

void defaultCalcIndoorBikeData(uint8_t resistanceLevel10, uint32_t crankIntervalTime, uint32_t* cadence2, uint32_t* speed100, int32_t*  power)
{
    
    if (crankIntervalTime==0)
    {
        *cadence2 = 0;
        *speed100 = 0;
        *power = 0;
    }
    else
    {
        *cadence2  = (uint32_t)( (uint64_t)K_CRANK_CADENCE / crankIntervalTime );
        *speed100 = (uint32_t)( (uint64_t)K_CRANK_SPEED   / crankIntervalTime );
        // https://diary.cyclekikou.net/archives/15876
        *power = (int32_t)((double)(*speed100) * (K_INCLINE_A * ((double)resistanceLevel10)/10 + K_INCLINE_B) * K_POWER);
    }
}

// コンストラクタ
MicroBitIndoorBikeMiniSensor::MicroBitIndoorBikeMiniSensor(MicroBit &_uBit, FuncCalcIndoorBikeData _pFuncCalcIndoorBikeData, MicrobitIndoorBikeMiniCrankSensorPin pin, uint16_t id)
    : uBit(_uBit), pFuncCalcIndoorBikeData(_pFuncCalcIndoorBikeData)
{
    this->id = id;
    this->lastIntervalTime=0;
    this->lastCadence2=0;
    this->lastSpeed100=0;
    this->updateSampleTimestamp=0;
    this->nextSensorTimestamp=0;
    this->sensorEdgeOn=false;
    this->resistanceLevel10=0;

    if (EventModel::defaultEventBus)
        EventModel::defaultEventBus->listen(MICROBIT_INDOOR_BIKE_MINI_SENSOR_EVENT_IDs[pin], MICROBIT_PIN_EVT_RISE
            , this, &MicroBitIndoorBikeMiniSensor::onCrankSensor);
    switch (pin)
    {
    case EDGE_P0:
        uBit.io.P0.eventOn(MICROBIT_PIN_EVENT_ON_EDGE);
        break;
    case EDGE_P1:
        uBit.io.P1.eventOn(MICROBIT_PIN_EVENT_ON_EDGE);
        break;
    default:    // EDGE_P2
        uBit.io.P2.eventOn(MICROBIT_PIN_EVENT_ON_EDGE);
        break;
    }
    
}

/**
     * The system timer will call this member function once the component has been added to
     * the array of system components using system_timer_add_component. This callback
     * will be in interrupt context.
     */
void MicroBitIndoorBikeMiniSensor::systemTick()
{
    this->update();
}

/**
  * Periodic callback from MicroBit idle thread.
  */
void MicroBitIndoorBikeMiniSensor::idleTick()
{
    this->update();
}

uint16_t MicroBitIndoorBikeMiniSensor::getId(void)
{
    return this->id;
}

void MicroBitIndoorBikeMiniSensor::setCurrentTimeOnCrankSignal(uint64_t currentTime)
{
    if (this->intervalList.size()==0)
    {
        // 初回から、回転数を表示させる。
        this->intervalList.push(currentTime-MAX_CRANK_INTERVAL_TIME_US);
    }
    this->intervalList.push(currentTime);
    if ((uint32_t)this->intervalList.size()>(this->CRANK_INTERVAL_LIST_SIZE))
    {
        this->intervalList.pop();
    }
}
uint32_t MicroBitIndoorBikeMiniSensor::getIntervalTime(void)
{
    return this->lastIntervalTime;
}

uint32_t MicroBitIndoorBikeMiniSensor::getCadence2(void)
{
    return this->lastCadence2;
}

uint32_t MicroBitIndoorBikeMiniSensor::getSpeed100(void)
{
    return this->lastSpeed100;
}

int32_t  MicroBitIndoorBikeMiniSensor::getPower(void)
{
    return this->lastPower;
}

void MicroBitIndoorBikeMiniSensor::update(void)
{
    if (this->sensorEdgeOn)
    {
        this->sensorEdgeOn=false;
        MicroBitEvent e(id, MICROBIT_INDOOR_BIKE_MINI_SENSOR_EVT_DATA_DETECT);
    }
    uint64_t currentTime = system_timer_current_time_us();
    if(!(status & MICROBIT_INDOOR_BIKE_MINI_SENSOR_ADDED_TO_IDLE))
    {
        // If we're running under a fiber scheduer, register ourselves for a periodic callback to keep our data up to date.
        // Otherwise, we do just do this on demand, when polled through our read() interface.
        fiber_add_idle_component(this);
        status |= MICROBIT_INDOOR_BIKE_MINI_SENSOR_ADDED_TO_IDLE;
    }
    
    if (currentTime >= this->updateSampleTimestamp)
    {        
        // Schedule our next sample.
        this->updateSampleTimestamp = currentTime + this->SENSOR_UPDATE_PERIOD_US;
        //uBit.serial.printf("current, %d, ", (uint32_t)currentTime);
        //uBit.serial.printf("next, %d\r\n", (uint32_t)this->updateSampleTimestamp);
        if ((this->intervalList.size()>0) && ((currentTime - this->intervalList.back())>=this->MAX_CRANK_INTERVAL_TIME_US))
        {
            while (this->intervalList.size()>0)
            {
                this->intervalList.pop();
            }
        }
        if (this->intervalList.size() < 2)
        {
            this->lastIntervalTime = 0;
        }
        else
        {
            uint64_t intervalNum = this->intervalList.size() - 1;
            uint64_t periodTime = this->intervalList.back() - this->intervalList.front();
            this->lastIntervalTime = periodTime / intervalNum;
        }
        defaultCalcIndoorBikeData(this->resistanceLevel10, this->lastIntervalTime, &this->lastCadence2, &this->lastSpeed100, &this->lastPower);
        if (this->pFuncCalcIndoorBikeData)
        {
            (*this->pFuncCalcIndoorBikeData)(this->resistanceLevel10, this->lastIntervalTime, &this->lastCadence2, &this->lastSpeed100, &this->lastPower);
        }
        
        //uBit.serial.printf("current, %d, num, %d, period, %d, tt, %d, cd, %d, spd, %d\r\n",
        //    (uint32_t)currentTime, (uint32_t)intervalNum, (uint32_t)periodTime,
        //    this->lastIntervalTime, this->lastCadence, this->lastSpeed100);
        
        // Send an event to indicate that we'e updated our sensor.
        MicroBitEvent e(id, MICROBIT_INDOOR_BIKE_MINI_SENSOR_EVT_DATA_UPDATE);
    }
}

// Indoor Bike Mini Sensor Event (Digital Input Pin)
void MicroBitIndoorBikeMiniSensor::onCrankSensor(MicroBitEvent e) 
{
    if (e.timestamp>this->nextSensorTimestamp)
    {
        this->nextSensorTimestamp = e.timestamp + this->SENSOR_DATA_PACKET_PERIOD;
        this->setCurrentTimeOnCrankSignal(e.timestamp);
        this->sensorEdgeOn=true;
    }
}

void MicroBitIndoorBikeMiniSensor::setResistanceLevel10(uint8_t resistanceLevel10)
{
    if (this->resistanceLevel10!=resistanceLevel10)
    {
        this->resistanceLevel10 = resistanceLevel10;
    }
}

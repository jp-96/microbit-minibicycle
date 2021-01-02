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

// コンストラクタ
MicroBitIndoorBikeMiniSensor::MicroBitIndoorBikeMiniSensor(MicroBit &_uBit, CrankSensorPin pin, uint16_t id)
    : uBit(_uBit)
{
    this->id = id;
    this->lastIntervalTime=0;
    this->lastCadence=0;
    this->lastSpeed100=0;
    this->updateSampleTimestamp=0;
    this->nextSensorTimestamp=0;
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

void MicroBitIndoorBikeMiniSensor::setCurrentTimeOnCrankSignal(uint64_t currentTime)
{
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

uint32_t MicroBitIndoorBikeMiniSensor::getSpeed100(void)
{
    return this->lastSpeed100;
}

uint32_t MicroBitIndoorBikeMiniSensor::getCadence(void)
{
    return this->lastCadence;
}

void MicroBitIndoorBikeMiniSensor::update()
{
    uint64_t currentTime = system_timer_current_time_us();
    if(!(status & MICROBIT_INDOOR_BIKE_MINI_ADDED_TO_IDLE))
    {
        // If we're running under a fiber scheduer, register ourselves for a periodic callback to keep our data up to date.
        // Otherwise, we do just do this on demand, when polled through our read() interface.
        fiber_add_idle_component(this);
        status |= MICROBIT_INDOOR_BIKE_MINI_ADDED_TO_IDLE;
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
        uint64_t intervalNum = 0;
        uint64_t periodTime = 0;
        if (this->intervalList.size() < 2)
        {
            intervalNum = 0;
            periodTime = 0;
            this->lastIntervalTime = 0;
            this->lastCadence = 0;
            this->lastSpeed100 = 0;
        }
        else
        {
            intervalNum = this->intervalList.size() - 1;
            periodTime = this->intervalList.back() - this->intervalList.front();
            this->lastIntervalTime = periodTime / intervalNum;
            this->lastCadence  = (uint32_t)( this->K_CRANK_CADENCE / this->lastIntervalTime );
            this->lastSpeed100 = (uint32_t)( this->K_CRANK_SPEED   / this->lastIntervalTime );
        }
        
        //uBit.serial.printf("current, %d, num, %d, period, %d, tt, %d, cd, %d, spd, %d\r\n",
        //    (uint32_t)currentTime, (uint32_t)intervalNum, (uint32_t)periodTime,
        //    this->lastIntervalTime, this->lastCadence, this->lastSpeed100);
        
        // Send an event to indicate that we'e updated our temperature.
        MicroBitEvent e(id, MICROBIT_INDOOR_BIKE_MINI_EVT_DATA_UPDATE);
    }
}

// Indoor Bike Mini Sensor Event (Digital Input Pin)
void MicroBitIndoorBikeMiniSensor::onCrankSensor(MicroBitEvent e) 
{
    if (e.timestamp>this->nextSensorTimestamp)
    {
        this->nextSensorTimestamp = e.timestamp + this->SENSOR_DATA_PACKET_PERIOD;
        this->setCurrentTimeOnCrankSignal(e.timestamp);
    }
}

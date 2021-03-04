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

#include "MicroBitIndoorBikeMiniSensor.h"

MicroBitIndoorBikeMiniSensor::MicroBitIndoorBikeMiniSensor(MicroBit &_uBit, MicrobitIndoorBikeMiniCrankSensorPin pin)
    : uBit(_uBit)
{
    this->nextSensorTimestamp=0;
    
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

void MicroBitIndoorBikeMiniSensor::onCrankSensor(MicroBitEvent e) 
{
    if (e.timestamp>this->nextSensorTimestamp)
    {
        this->nextSensorTimestamp = e.timestamp + this->SENSOR_DATA_PACKET_PERIOD;
        if (this->intervalList.size()==0)
        {
            // first interval.
            this->intervalList.push(e.timestamp-MAX_CRANK_INTERVAL_TIME_US);
        }
        this->intervalList.push(e.timestamp);
        if ((uint32_t)this->intervalList.size()>(this->CRANK_INTERVAL_LIST_SIZE))
        {
            this->intervalList.pop();
        }
    }
}

uint32_t MicroBitIndoorBikeMiniSensor::getCrankIntervalTime(void)
{
    uint32_t crankIntervalTime = 0;
    uint64_t currentTime = system_timer_current_time_us();
    if (this->intervalList.size()>0 && (currentTime - this->intervalList.back())>=this->MAX_CRANK_INTERVAL_TIME_US)
    {
        while (this->intervalList.size()>0)
        {
            this->intervalList.pop();
        }
    }
    if (this->intervalList.size() > 1)
    {
        uint64_t intervalNum = this->intervalList.size() - 1;
        uint64_t periodTime = this->intervalList.back() - this->intervalList.front();
        crankIntervalTime = (uint32_t)(periodTime / intervalNum);
    }
    return crankIntervalTime;
}

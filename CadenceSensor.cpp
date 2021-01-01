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

#include "MicroBit.h"
#include "CadenceSensor.h"

#ifndef INTERVAL_LIST_SIZE
#define INTERVAL_LIST_SIZE 3
#endif

#ifndef SPEED_WATCHDOG_currentTime
#define SPEED_WATCHDOG_currentTime 4000000
#endif

#ifndef K_CADENCE
#define K_CADENCE 60000000
#endif

#ifndef K_SPEED
#define K_SPEED 1800000000
#endif

// コンストラクタ
CadenceSensor::CadenceSensor(MicroBit &_uBit)
    : uBit(_uBit)
{
    this->setIntervalListSize(INTERVAL_LIST_SIZE);
    this->update();
}

void CadenceSensor::setIntervalListSize(uint32_t size)
{
    if (size<2)
    {
        size = 2;
    }
    this->intervalListSize = size;
}

void CadenceSensor::setCurrentTimeOnCrankSignal(uint64_t currentTime)
{
    this->intervalList.push(currentTime);
    if ((uint32_t)this->intervalList.size()>(this->intervalListSize))
    {
        this->intervalList.pop();
    }
}
uint32_t CadenceSensor::getIntervalTime(void)
{
    return this->lastIntervalTime;
}

uint32_t CadenceSensor::getSpeed100(void)
{
    return this->lastSpeed100;
}

uint32_t CadenceSensor::getCadence(void)
{
    return this->lastCadence;
}

void CadenceSensor::update()
{
    uint64_t currentTime = system_timer_current_time_us();
    if ((this->intervalList.size()>0) && ((currentTime - this->intervalList.back())>(uint64_t)SPEED_WATCHDOG_currentTime))
    {
        uBit.serial.printf("WATCHDOG.\r\n");
        while (this->intervalList.size()>0)
        {
            this->intervalList.pop();
        }
    }
    uint64_t intervalNum = 0;
    uint64_t intervalTotalTime = 0;
    if (this->intervalList.size() < 2)
    {
        intervalNum = 0;
        intervalTotalTime = 0;
        this->lastIntervalTime = 0;
        this->lastCadence = 0;
        this->lastSpeed100 = 0;
    }
    else
    {
        intervalNum = this->intervalList.size() - 1;
        intervalTotalTime = this->intervalList.back() - this->intervalList.front();
        this->lastIntervalTime = intervalTotalTime / intervalNum;
        this->lastCadence  = (uint32_t)( (uint64_t)K_CADENCE / this->lastIntervalTime );
        this->lastSpeed100 = (uint32_t)( (uint64_t)K_SPEED   / this->lastIntervalTime );
    }
    
    uBit.serial.printf("current, %d, num, %d, total, %d, tt, %d, cd, %d, spd, %d\r\n",
        (uint32_t)currentTime, (uint32_t)intervalNum, (uint32_t)intervalTotalTime,
        this->lastIntervalTime, this->lastCadence, this->lastSpeed100);

}

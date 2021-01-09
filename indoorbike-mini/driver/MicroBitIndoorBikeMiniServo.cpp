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
#include "MicroBitIndoorBikeMiniServo.h"

// Constructor.
MicroBitIndoorBikeMiniServo::MicroBitIndoorBikeMiniServo(MicroBit &_uBit, FuncCalcAngle _pFuncCalcAngle, MicrobitIndoorBikeMiniServoPin pin, uint16_t id)
    : uBit(_uBit), pFuncCalcAngle(_pFuncCalcAngle)
{
    this->servoPin = NULL;
    switch (pin)
    {
    case SERVO_P0:
        this->servoPin = &(uBit.io.P0);
        break;
    
    case SERVO_P1:
        this->servoPin = &(uBit.io.P1);
        break;
    
    default:
        this->servoPin = &(uBit.io.P2);
        break;
    }
    this->id = id;
    this->nextServoControlTimestamp = 0;
    this->servoState = SERVO_NONE;
    this->targetResistanceLevel10 = 0;
    this->servoAngle = 0;
}

/**
     * The system timer will call this member function once the component has been added to
     * the array of system components using system_timer_add_component. This callback
     * will be in interrupt context.
     */
void MicroBitIndoorBikeMiniServo::systemTick()
{
    this->update();
}

/**
     * Periodic callback from MicroBit idle thread.
     */
void MicroBitIndoorBikeMiniServo::idleTick()
{
    this->update();
}

// Event ID
uint16_t MicroBitIndoorBikeMiniServo::getId(void)
{
    return this->id;
}

void MicroBitIndoorBikeMiniServo::update(bool force)
{
    uint64_t currentTime = system_timer_current_time_us();
    if(!(status & MICROBIT_INDOOR_BIKE_MINI_SERVO_ADDED_TO_IDLE))
    {
        // If we're running under a fiber scheduer, register ourselves for a periodic callback to keep our data up to date.
        // Otherwise, we do just do this on demand, when polled through our read() interface.
        fiber_add_idle_component(this);
        status |= MICROBIT_INDOOR_BIKE_MINI_SERVO_ADDED_TO_IDLE;
    }
    
    if (force || this->servoState==SERVO_NONE)
    {
        this->servoState = SERVO_OFF;
        this->nextServoControlTimestamp=currentTime;
    }
    
    if (currentTime >= this->nextServoControlTimestamp)
    {
        switch (this->servoState)
        {
        case SERVO_ON:
            this->servoState = SERVO_OFF;
            // Schedule our next control.
            this->nextServoControlTimestamp = currentTime + this->SERVO_PERIOD_OFF;
            //servoPin->getDigitalValue(PullDown);
            break;

        default:
            this->servoState = SERVO_ON;
            // Schedule our next control.
            this->nextServoControlTimestamp = currentTime + this->SERVO_PERIOD_ON;
            servoPin->setServoValue(this->servoAngle);
            break;
            
        }
    }
}

void MicroBitIndoorBikeMiniServo::setTargetResistanceLevel10(uint8_t targetResistanceLevel10)
{
    if (this->targetResistanceLevel10!=targetResistanceLevel10
        && targetResistanceLevel10 >= VAL_MINIMUM_RESISTANCE_LEVEL
        && targetResistanceLevel10 <= VAL_MAXIMUM_RESISTANCE_LEVEL)
    {
        this->targetResistanceLevel10=targetResistanceLevel10;
        if (this->pFuncCalcAngle)
        {
            this->servoAngle = (*this->pFuncCalcAngle)(this->targetResistanceLevel10);
        }
        else
        {
            if (this->targetResistanceLevel10<=10)     //10  40
            {
                this->servoAngle = 40;
            }
            else if(this->targetResistanceLevel10<=20) //20  60
            {
                this->servoAngle = 60;
            }
            else if(this->targetResistanceLevel10<=30) //30  70
            {
                this->servoAngle = 70;
            }
            else if(this->targetResistanceLevel10<=40) //40  80
            {
                this->servoAngle = 80;
            }
            else if(this->targetResistanceLevel10<=50) //50  85
            {
                this->servoAngle = 85;
            }
            else if(this->targetResistanceLevel10<=60) //60  90
            {
                this->servoAngle = 90;
            }
            else if(this->targetResistanceLevel10<=70) //70  95
            {
                this->servoAngle = 95;
            }
            else              //80 100
            {
                this->servoAngle =100;
            }
        }
        
        this->update(true);
        new MicroBitEvent(this->id
                , MICROBIT_INDOOR_BIKE_MINI_SERVO_EVT_DATA_CHANGED);
    }
}

uint8_t MicroBitIndoorBikeMiniServo::getTargetResistanceLevel10(void)
{
    return this->targetResistanceLevel10;
}

int MicroBitIndoorBikeMiniServo::getServoAngle(void)
{
    return this->servoAngle;
}

void MicroBitIndoorBikeMiniServo::incrementTargetResistanceLevel10(void)
{
    this->setTargetResistanceLevel10(this->targetResistanceLevel10 + 10);
}

void MicroBitIndoorBikeMiniServo::decrementTargetResistanceLevel10(void)
{
    this->setTargetResistanceLevel10(this->targetResistanceLevel10 - 10);
}

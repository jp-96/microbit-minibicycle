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
#include "SpeedSensor.h"

MicroBit uBit;
SpeedSensor sensor(uBit);

//
// Print details of all events received to the serial port.
// Default settings are 115200 baud, 8N1 over the USB interface.
//
void onButton(MicroBitEvent e)
{
    if ((e.source == MICROBIT_ID_BUTTON_A) && (e.value == MICROBIT_BUTTON_EVT_CLICK))
    {
        sensor.setCurrentTimeOnCrankSignal();
    }
}

// ▼fiberの使い方
// https://www.instructables.com/BBC-Microbit-C-Getting-Started/
void tickTimer()
{
    uBit.serial.printf("Start.\r\n");
    while(1)
    {
        unsigned long sysTime=uBit.systemTime();
        
        sensor.update();
        sensor.getCadence();
        sensor.getSpeed10Avg();

        fiber_sleep(((unsigned long)1000)-(uBit.systemTime()-sysTime));
    }
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    // Insert your code here!
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButton);
    create_fiber(tickTimer);

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

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
#include "indoorbike-mini/driver/MicroBitIndoorBikeMiniSensor.h"

MicroBit uBit;
MicroBitIndoorBikeMiniSensor sensor(uBit);

// Button Event
void onButton(MicroBitEvent e)
{
    //uBit.serial.printf("onButton()\r\n");
    if ((e.source == MICROBIT_ID_BUTTON_A) && (e.value == MICROBIT_BUTTON_EVT_CLICK))
    {
        sensor.onCrankSensor(e);
    }
}

// Crank Sensor Event (P2, RISE)
void onCrankSensor(MicroBitEvent e) 
{
    //uBit.serial.printf("onSensor()\r\n");
    sensor.onCrankSensor(e);
}

void onSensorUpdate(MicroBitEvent e)
{
    //uBit.serial.printf("onUpdate()\r\n");
    uBit.serial.printf("T, %d, C, %d, S, %d\r\n"
        , sensor.getIntervalTime()
        , sensor.getCadence()
        , sensor.getSpeed100()
    );
}

int main()
{
    uBit.init();

    // Register for Button Events on Button(A)
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onButton);
    
    // Register for Sensor Events on P2.
    uBit.messageBus.listen(MICROBIT_ID_IO_P2, MICROBIT_PIN_EVT_RISE , onCrankSensor);
    uBit.io.P2.eventOn(MICROBIT_PIN_EVENT_ON_EDGE);
    // IdleTick
    uBit.addIdleComponent(&sensor);
    uBit.messageBus.listen(CUSTOM_EVENT_ID_INDOORBIKE_MINI, MICROBIT_EVT_ANY, onSensorUpdate);

    release_fiber();
}

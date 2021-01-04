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

#ifndef MICROBIT_INDOOR_BIKE_MINI_H
#define MICROBIT_INDOOR_BIKE_MINI_H

static const uint16_t CUSTOM_EVENT_ID_BASE = 32768;

// # SINT16 Unitless with a resolution of 0.1
#define VAL_MINIMUM_RESISTANCE_LEVEL    10
// # SINT16 Unitless with a resolution of 0.1
#define VAL_MAXIMUM_RESISTANCE_LEVEL    80
// # UINT16 Unitless with a resolution of 0.1
#define VAL_INCREMENT_RESISTANCE_LEVEL  10

// driver/MicroBitIndoorBikeMiniSensor
#ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI
#define CUSTOM_EVENT_ID_INDOORBIKE_MINI (CUSTOM_EVENT_ID_BASE+1)
#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI */

// bluetooth/MicroBitFTMIndoorBikeService
#ifndef CUSTOM_EVENT_ID_FITNESS_MACHINE_CONTROL_POINT
#define CUSTOM_EVENT_ID_FITNESS_MACHINE_CONTROL_POINT (CUSTOM_EVENT_ID_BASE+2)
#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI */

#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_H */
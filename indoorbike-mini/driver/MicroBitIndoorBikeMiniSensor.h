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

#ifndef MICROBIT_INDOOR_BIKE_MINI_SENSOR_H
#define MICROBIT_INDOOR_BIKE_MINI_SENSOR_H

#include "MicroBit.h"
#include "MicroBitCustom.h"
#include <queue>

enum MicrobitIndoorBikeMiniCrankSensorPin
{
    EDGE_P0 = 0,
    EDGE_P1 = 1,
    EDGE_P2 = 2
};

static const int MICROBIT_INDOOR_BIKE_MINI_SENSOR_EVENT_IDs[] = {MICROBIT_ID_IO_P0, MICROBIT_ID_IO_P1, MICROBIT_ID_IO_P2};

class MicroBitIndoorBikeMiniSensor
{
private:
    MicroBit &uBit;
    
    static const uint64_t SENSOR_DATA_PACKET_PERIOD =   100000; // 100ms
    static const uint64_t MAX_CRANK_INTERVAL_TIME_US = 2500000; // 2.5s
    static const uint64_t CRANK_INTERVAL_LIST_SIZE = 3;
    
public:
    MicroBitIndoorBikeMiniSensor(MicroBit &_uBit, MicrobitIndoorBikeMiniCrankSensorPin pin = EDGE_P2);

private:
    std::queue<uint64_t> intervalList;
    uint64_t nextSensorTimestamp;

private:
    void onCrankSensor(MicroBitEvent);

public:
    uint32_t getCrankIntervalTime(void);

};

#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SENSOR_H */

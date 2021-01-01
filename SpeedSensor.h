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

#ifndef SPEED_SENSOR_H
#define SPEED_SENSOR_H

#include "MicroBit.h"
#include <queue>

class SpeedSensor
{
private:
    MicroBit &uBit;
    
public:
    // Constructor.
    SpeedSensor(MicroBit &_uBit);

private:
    // クランクセンサーの信号取得時間のリスト（マイクロ秒=10の−6乗秒）
    std::queue<uint64_t> intervalList;
    // 平均速度を算出する計測数
    uint32_t intervalListSize;
    // 最新の平均10倍速度
    uint32_t lastSpeed10Avg;
    // 最新の平均クランク回転数
    uint32_t lastCadence;

public:
    // 平均速度を算出する計測数
    void setIntervalListSize(uint32_t);

    // クランクセンサー信号を取得した時間を内部でセットする。
    void setCurrentTimeOnCrankSignal(void);

    // 速度とクランク回転数を再計算する（最新化）
    void update();

    // 平均10倍速度を取得する（単位： km/h の 10倍）
    uint32_t getSpeed10Avg(void);
    // 平均クランク回転数を取得する（単位：rpm）
    uint32_t getCadence(void);

};

#endif
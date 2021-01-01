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
    // クランクセンサー信号の計測時間のリスト（単位: マイクロ秒 - 1秒/1000000）
    std::queue<uint64_t> intervalList;
    // 計測時間の数
    uint32_t intervalListSize;
    // 最新のインターバル時間（単位: マイクロ秒 - 1秒/1000000）
    uint32_t lastIntervalTime;
    // 最新の速度（単位： km/h の 100倍）
    uint32_t lastSpeed100;
    // 最新の平均クランク回転数（単位：rpm）
    uint32_t lastCadence;

public:
    // 速度とクランク回転数を算出する計測時間の数を設定する
    void setIntervalListSize(uint32_t);

    // クランクセンサー信号の計測時間を保持する。
    void setCurrentTimeOnCrankSignal(uint64_t);

    // 速度とクランク回転数を再計算する（最新化）
    void update();

    // インターバル時間を取得する（単位: マイクロ秒 - 1秒/1000000）
    uint32_t getIntervalTime(void);
    // 速度を取得する（単位： km/h の 100倍）
    uint32_t getSpeed100(void);
    // クランク回転数を取得する（単位：rpm）
    uint32_t getCadence(void);

};

#endif
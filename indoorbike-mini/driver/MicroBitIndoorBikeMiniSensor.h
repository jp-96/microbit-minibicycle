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
#include "MicroBitCustomComponent.h"
#include <queue>

// IndoorBikeDataの算出関数の型定義
//
// 負荷のレベル（範囲：10～80）
// uint8_t resistanceLevel10
// クランクセンサーのインターバル時間（単位: マイクロ秒 - 1/1000000秒）
// uint32_t crankIntervalTime
// 算出されたクランク回転数（単位：rpm の 2倍）
// uint32_t* cadence2
// 算出された速度（単位： km/h の 100倍）
// uint32_t* speed100
// 算出されたパワー（単位： ワット）
// int32_t*  power
typedef void (*FuncCalcIndoorBikeData)(uint8_t resistanceLevel10, uint32_t crankIntervalTime, uint32_t* cadence2, uint32_t* speed100, int32_t*  power);

/**
  * Status flags
  */
// Universal flags used as part of the status field
// #define MICROBIT_COMPONENT_RUNNING		0x01
#define MICROBIT_INDOOR_BIKE_MINI_SENSOR_ADDED_TO_IDLE              0x02

enum MicrobitIndoorBikeMiniCrankSensorPin
{
    EDGE_P0 = 0,
    EDGE_P1 = 1,
    EDGE_P2 = 2
};

static const int MICROBIT_INDOOR_BIKE_MINI_SENSOR_EVENT_IDs[] = {MICROBIT_ID_IO_P0, MICROBIT_ID_IO_P1, MICROBIT_ID_IO_P2};

class MicroBitIndoorBikeMiniSensor : public MicroBitCustomComponent
{
private:
    MicroBit &uBit;
    
    static const uint64_t SENSOR_UPDATE_PERIOD_US =    1000000; // 1s
    static const uint64_t SENSOR_DATA_PACKET_PERIOD =   100000; // 100ms
    static const uint64_t MAX_CRANK_INTERVAL_TIME_US = 2500000; // 2.5s
    static const uint64_t CRANK_INTERVAL_LIST_SIZE = 3;
    
public:
    // Constructor.
    MicroBitIndoorBikeMiniSensor(MicroBit &_uBit, FuncCalcIndoorBikeData _pFuncCalcIndoorBikeData = NULL, MicrobitIndoorBikeMiniCrankSensorPin pin = EDGE_P2, uint16_t id = CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR);

    /**
      * Periodic callback from MicroBit idle thread.
      */
    virtual void idleTick();

private:
    // ケイデンスセンサー信号の計測時間のリスト（単位: マイクロ秒 - 1/1000000秒）
    std::queue<uint64_t> intervalList;
    
    // 次のupdate実行時間
    uint64_t updateSampleTimestamp;
    
    // ケイデンスセンサーのオフ待ち
    uint64_t nextSensorTimestamp;

private:
    
    // ケイデンスセンサー信号の計測時間を保持する。
    void setCurrentTimeOnCrankSignal(uint64_t);

    // クランク回転数と速度を再計算する（最新化）
    void update();

private:
    // 最新のインターバル時間（単位: マイクロ秒 - 1/1000000秒）
    uint32_t lastIntervalTime;
    // 最新のクランク回転数（単位：rpm の 2倍）
    uint32_t lastCadence2;
    // 最新の速度（単位： km/h の 100倍）
    uint32_t lastSpeed100;
    // 最新のパワー（単位： ワット）
    int32_t  lastPower;

public:

    // インターバル時間を取得する（単位: マイクロ秒 - 1/1000000秒）
    uint32_t getIntervalTime(void);
    // クランク回転数を取得する（単位：rpm の 2倍）
    uint32_t getCadence2(void);
    // 速度を取得する（単位： km/h の 100倍）
    uint32_t getSpeed100(void);
    // パワーを取得する（単位： ワット）
    int32_t  getPower(void);

private:
    // Indoor Bike Mini Sensor Event (Digital Input Pin)
    void onCrankSensor(MicroBitEvent);

public:
    // Coefficient of Cadence and Speed
    static const uint64_t K_CRANK_CADENCE =  120000000;
    static const uint64_t K_CRANK_SPEED   = 1800000000;

    // https://diary.cyclekikou.net/archives/15876
    static const double K_POWER = 0.8 * (70 * 9.80665) / (360 * 0.95 * 100); // weight(70kg)
    static const double K_INCLINE_A = 0.9; // Incline(%) - a
    static const double K_INCLINE_B = 0.6; // Incline(%) - b


private:
    FuncCalcIndoorBikeData pFuncCalcIndoorBikeData;
    
    // 負荷のレベル（範囲：10～80） - パワーの算出用
    uint8_t resistanceLevel10;


public:
    // 負荷のレベルを取得・設定する（範囲：10～80）
    uint8_t getResistanceLevel10(void);
    void setResistanceLevel10(uint8_t resistanceLevel10);

};

#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SENSOR_H */

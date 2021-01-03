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

#ifndef MICROBIT_FTM_INDOOR_BIKE_SERVICE_H
#define MICROBIT_FTM_INDOOR_BIKE_SERVICE_H

#include "MicroBit.h"
#include "../IndoorBikeMini.h"
#include "../driver/MicroBitIndoorBikeMiniSensor.h"

#define OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL 0x04

class MicroBitFTMIndoorBikeService
{

public:

    /**
      * Constructor.
      * Create a representation of the IndoorBikeService
      * @param _ble The instance of a BLE device that we're running on.
      * @param _indoorBike An instance of MicroBitIndoorBikeMiniSensor to use as our indoor bike source.
      */
    MicroBitFTMIndoorBikeService(BLEDevice &_ble, MicroBitIndoorBikeMiniSensor &_indoorBike);

    /**
      * Callback. Invoked when any of our attributes are written via BLE.
      */
    void onDataWritten(const GattWriteCallbackParams *params);

    /**
     * Temperature update callback
     */
    void indoorBikeUpdate(MicroBitEvent e);

private:

    // Bluetooth stack we're running on.
    BLEDevice   &ble;
    MicroBitIndoorBikeMiniSensor    &indoorBike;

    //# Bit Definitions for the Indoor Bike Data Characteristic
    static const uint16_t FLAGS_INDOOR_BIKE_DATA_CHAR /*
    #   000 (bits 13-15) Reserved for Future Use
    #      0 (bit 12) Remaining Time Present
    #       0 (bit 11) Elapsed Time Present
    #        0 (bit 10) Metabolic Equivalent Present
    #         0 (bit  9) Heart Rate Present
    #          0 (bit  8) Expended Energy Present
    #           0 (bit  7) Average Power Present
    #            0 (bit  6)*Instantaneous Power Present
    #             0 (bit  5)*Resistance Level Present
    #              0 (bit  4) Total Distance Present
    #               0 (bit  3) Average Cadence present
    #                1 (bit  2)*Instantaneous Cadence (uint16, 1/minute with a resolution of 0.5)
    #                 0 (bit  1) Average Speed present
    #                  0 (bit  0) More Data
    #   5432109876543210*/
    = 0b0000000000000100;
    uint8_t indoorBikeDataCharacteristicBuffer[2+2+2];

    uint8_t fitnessMachineControlPointDataCharacteristicBuffer[20];
    uint8_t targetResistanceLevel10=0;

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t indoorBikeDataCharacteristicHandle;
    GattAttribute::Handle_t fitnessMachineControlPointCharacteristicHandle;

public:
    uint8_t getTargetResistanceLevel10(void);
};

#endif /* #ifndef MICROBIT_FTM_INDOOR_BIKE_SERVICE_H */
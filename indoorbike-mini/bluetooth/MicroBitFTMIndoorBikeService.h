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

/*
# Bit Definitions for the Indoor Bike Data Characteristic
#                                          000 (bits 13-15) Reserved for Future Use
#                                             0 (bit 12) Remaining Time Present
#                                              0 (bit 11) Elapsed Time Present
#                                               0 (bit 10) Metabolic Equivalent Present
#                                                0 (bit  9) Heart Rate Present
#                                                 0 (bit  8) Expended Energy Present
#                                                  0 (bit  7) Average Power Present
#                                                   0 (bit  6)*Instantaneous Power Present
#                                                    0 (bit  5)*Resistance Level Present
#                                                     0 (bit  4) Total Distance Present
#                                                      0 (bit  3) Average Cadence present
#                                                       1 (bit  2)*Instantaneous Cadence (uint16, 1/minute with a resolution of 0.5)
#                                                        0 (bit  1) Average Speed present
#                                                         0 (bit  0) More Data
#                                          5432109876543210 */
#define FTMP_FLAGS_INDOOR_BIKE_DATA_CHAR 0b0000000000000100

// # Fitness Machine Control Point Procedure Requirements
// # 0x00 M Request Control
#define FTMP_OP_CODE_CPPR_00_REQUEST_CONTROL             0x00
// # 0x01 M Reset
#define FTMP_OP_CODE_CPPR_01_RESET                       0x01
// # 0x04 C.3 Set Target Resistance Level [UINT8, Level]
#define FTMP_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL 0x04
// # 0x07 M Start or Resume
#define FTMP_OP_CODE_CPPR_07_START_RESUME                0x07
// # 0x08 M Stop or Pause [UINT8, 0x01-STOP, 0x02-PAUSE]
#define FTMP_OP_CODE_CPPR_08_STOP_PAUSE                  0x08
// # 0x11 C.14 Set Indoor Bike Simulation [SINT16, Wind Speed], [SINT16, Grade], [UINT8 CRR], [UINT8, CW]
#define FTMP_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION  0x11

// # 0x80 M Response Code
#define FTMP_OP_CODE_CPPR_80_RESPONSE_CODE         0x80

// # Fitness Machine Control Point Procedure Result Code
// # 0x00 N/A
#define FTMP_RESULT_CODE_CPPR_00_NONE              0x00
// # 0x01 Success
#define FTMP_RESULT_CODE_CPPR_01_SUCCESS           0x01
// # 0x02 Op Code not supported
#define FTMP_RESULT_CODE_CPPR_02_NOT_SUPORTED      0x02
// # 0x03 Invalid Parameter
#define FTMP_RESULT_CODE_CPPR_03_INVALID_PARAMETER 0x03

/*
# Definition of the bits of the Fitness Machine Features field
#                                                  000000000000000 (bits 17-31) Reserved for Future Use
#                                                                 0 (bit 16) User Data Retention Supported
#                                                                  0 (bit 15) Force on Belt and Power Output Supported
#                                                                   0 (bit 14)*Power Measurement Supported
#                                                                    0 (bit 13) Remaining Time Supported
#                                                                     0 (bit 12) Elapsed Time Supported
#                                                                      0 (bit 11) Metabolic Equivalent Supported
#                                                                       0 (bit 10) Heart Rate Measurement Supported
#                                                                        0 (bit  9) Expended Energy Supported
#                                                                         0 (bit  8) Stride Count Supported
#                                                                          1 (bit  7)*Resistance Level Supported
#                                                                           0 (bit  6) Step Count Supported
#                                                                            0 (bit  5) Pace Supported
#                                                                             0 (bit  4) Elevation Gain Supported
#                                                                              0 (bit  3) Inclination Supported
#                                                                               0 (bit  2) Total Distance Supported
#                                                                                0 (bit  1) Cadence Supported
#                                                                                 0 (bit  0) Average Speed Supported
#                                                  10987654321098765432109876543210 */
#define FTMP_FLAGS_FITNESS_MACINE_FEATURES_FIELD 0b00000000000000000000000010000000

/*
# Definition of the bits of the Target Setting Features field
#                                                  000000000000000 (bits 17-31) Reserved for Future Use
#                                                                 0 (bit 16) Targeted Cadence Configuration Supported
#                                                                  0 (bit 15) Spin Down Control Supported
#                                                                   0 (bit 14) Wheel Circumference Configuration Supported
#                                                                    0 (bit 13)*Indoor Bike Simulation Parameters Supported
#                                                                     0 (bit 12) Targeted Time in Five Heart Rate Zones Configuration Supported
#                                                                      0 (bit 11) Targeted Time in Three Heart Rate Zones Configuration Supported
#                                                                       0 (bit 10) Targeted Time in Two Heart Rate Zones Configuration Supported
#                                                                        0 (bit  9) Targeted Training Time Configuration Supported
#                                                                         0 (bit  8) Targeted Distance Configuration Supported
#                                                                          0 (bit  7) Targeted Stride Number Configuration Supported
#                                                                           0 (bit  6) Targeted Step Number Configuration Supported
#                                                                            0 (bit  5) Targeted Expended Energy Configuration Supported
#                                                                             0 (bit  4) Heart Rate Target Setting Supported
#                                                                              0 (bit  3) Power Target Setting Supported
#                                                                               1 (bit  2)*Resistance Target Setting Supported
#                                                                                0 (bit  1) Inclination Target Setting Supported
#                                                                                 0 (bit  0) Speed Target Setting Supported
#                                                  10987654321098765432109876543210 */
#define FTMP_FLAGS_TARGET_SETTING_FEATURES_FIELD 0b00000000000000000000000000000100
    
// # Fitness Machine Status values
// # 0x01 Reset
#define FTMP_OP_CODE_FITNESS_MACHINE_STATUS_01_RESET                                      0x01
// # 0x07 Target Resistance Level Changed
#define FTMP_OP_CODE_FITNESS_MACHINE_STATUS_07_TARGET_RESISTANCE_LEVEL_CHANGED            0x07
// # 0x12 Indoor Bike Simulation Parameters Changed
#define FTMP_OP_CODE_FITNESS_MACHINE_STATUS_12_INDOOR_BIKE_SIMULATION_PARAMETERS_CHANGED  0x12

// # Bit Definitions for the Training Status Characteristic
// # (bits 2-7) Reseved for Future Use
// # (bit 1) Extended String present
// # (bit 0) Trainig Status String present
// # UINT8                                                76543210
#define FTMP_FLAGS_TRAINING_STATUS_FIELD_00_STATUS_ONLY 0b00000000

// # Training Status Field Definitions
// # UINT8 Other
#define FTMP_VAL_TRAINING_STATUS_00_OTHER       0x00
// # UINT8 Idle
#define FTMP_VAL_TRAINING_STATUS_01_IDEL        0x01
// # UINT8 Manual Mode (Quick Start)
#define FTMP_VAL_TRAINING_STATUS_0D_MANUAL_MODE 0x0D

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
     * Indoor Bike update callback
     */
    void indoorBikeUpdate(MicroBitEvent e);

private:
    // instance
    BLEDevice &ble;
    MicroBitIndoorBikeMiniSensor &indoorBike;
    
    // Characteristic buffer
    uint8_t indoorBikeDataCharacteristicBuffer[2+2+2];
    uint8_t fitnessMachineControlPointCharacteristicBuffer[1+2+2+1+1];
    uint8_t fitnessMachineFeatureCharacteristicBuffer[4+4];
    uint8_t fitnessMachineStatusCharacteristicBuffer[2];
    uint8_t fitnessTrainingStatusCharacteristicBuffer[2];
    uint8_t fitnessSupportedResistanceLevelRangeCharacteristicBuffer[2+2+2];
    
    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t indoorBikeDataCharacteristicHandle;
    GattAttribute::Handle_t fitnessMachineControlPointCharacteristicHandle;
    GattAttribute::Handle_t fitnessMachineFeatureCharacteristicHandle;
    GattAttribute::Handle_t fitnessMachineStatusCharacteristicHandle;
    GattAttribute::Handle_t fitnessTrainingStatusCharacteristicHandle;
    GattAttribute::Handle_t fitnessSupportedResistanceLevelRangeCharacteristicHandle;

    // var
    uint8_t targetResistanceLevel10;

public:
    // getter/setter
    uint8_t getTargetResistanceLevel10(void);
};

#endif /* #ifndef MICROBIT_FTM_INDOOR_BIKE_SERVICE_H */
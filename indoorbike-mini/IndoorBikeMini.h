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
#ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR
#define CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR (CUSTOM_EVENT_ID_BASE+1)
#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR */
// Event Value
#define MICROBIT_INDOOR_BIKE_MINI_SENSOR_EVT_DATA_UPDATE 0b0000000000000001

// driver/MicroBitIndoorBikeMiniServo
#ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO
#define CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO (CUSTOM_EVENT_ID_BASE+2)
#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO */
// Event Value - Resistance Level
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_EVT_DATA_CHANGED 0b0000000000000001

// bluetooth/MicroBitFTMIndoorBikeService
// # Fitness Machine Control Point Procedure Requirements
// Custom Event ID
#ifndef CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE
#define CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE (CUSTOM_EVENT_ID_BASE+3)
#endif /* #ifndef CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE */
// FTMP Event Val
// # 0x00 M Request Control
#define FTMP_EVENT_VAL_OP_CODE_CPPR_00_REQUEST_CONTROL                      0b0000000000000001
// # 0x01 M Reset
#define FTMP_EVENT_VAL_OP_CODE_CPPR_01_RESET                                0b0000000000000010
// # 0x04 C.3 Set Target Resistance Level [UINT8, Level]
#define FTMP_EVENT_VAL_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL          0b0000000000000100
// # 0x07 M Start or Resume
#define FTMP_EVENT_VAL_OP_CODE_CPPR_07_START_RESUME                         0b0000000000001000
// # 0x08 M Stop or Pause [UINT8, 0x01-STOP, 0x02-PAUSE]
#define FTMP_EVENT_VAL_OP_CODE_CPPR_08_STOP_PAUSE                           0b0000000000010000
// # 0x11 C.14 Set Indoor Bike Simulation [SINT16, Wind Speed], [SINT16, Grade], [UINT8 CRR], [UINT8, CW]
#define FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION           0b0000000000100000
#define FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION_CHANGED   0b0000000001000000

#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_H */
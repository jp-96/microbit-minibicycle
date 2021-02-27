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

#ifndef MICROBIT_CUSTOM_H
#define MICROBIT_CUSTOM_H

// The base of custom Event Bus ID.
static const uint16_t MICROBIT_CUSTOM_ID_BASE = 32768;

/*
 * MicroBitIndoorBikeStepSensor
 */

// Event Bus ID for IndoorBike step sensor
#ifndef MICROBIT_INDOORBIKE_STEP_SENSOR_ID
#define MICROBIT_INDOORBIKE_STEP_SENSOR_ID (MICROBIT_CUSTOM_ID_BASE+1)
#endif /* #ifndef MICROBIT_INDOORBIKE_STEP_SENSOR_ID */

// Event value
#define MICROBIT_INDOOR_BIKE_STEP_SENSOR_EVT_DATA_UPDATE 0b0000000000000001

/*
 * MicroBitIndoorBikeStepService
 */

#ifndef BLE_DEVICE_LOCAL_NAME_CHENGE
#define BLE_DEVICE_LOCAL_NAME_CHENGE 1
#endif /* #ifndef BLE_DEVICE_LOCAL_NAME_CHENGE */

#ifndef BLE_DEVICE_LOCAL_NAME
#define BLE_DEVICE_LOCAL_NAME "STEP:BIT"
#endif /* #ifndef BLE_DEVICE_LOCAL_NAME */

// # SINT16 Unitless with a resolution of 0.1
#define VAL_MINIMUM_RESISTANCE_LEVEL    10
// # SINT16 Unitless with a resolution of 0.1
#define VAL_MAXIMUM_RESISTANCE_LEVEL    80
// # UINT16 Unitless with a resolution of 0.1
#define VAL_INCREMENT_RESISTANCE_LEVEL  10

// driver/MicroBitIndoorBikeMiniSensor
#ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR
#define CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR (MICROBIT_CUSTOM_ID_BASE+1)
#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR */
// Event Value
#define MICROBIT_INDOOR_BIKE_MINI_SENSOR_EVT_DATA_UPDATE 0b0000000000000001

// driver/MicroBitIndoorBikeMiniServo
#ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO
#define CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO (MICROBIT_CUSTOM_ID_BASE+2)
#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO */
// Event Value - Resistance Level
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_EVT_DATA_CHANGED 0b0000000000000001

// bluetooth/MicroBitFTMIndoorBikeService
// # Fitness Machine Control Point Procedure Requirements
// Custom Event ID
#ifndef CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE
#define CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE (MICROBIT_CUSTOM_ID_BASE+3)
#endif /* #ifndef CUSTOM_EVENT_ID_FITNESS_MACHINE_INDOOR_BIKE_SERVICE */
// FTMP Event Val
// # 0x11 C.14 Set Indoor Bike Simulation [SINT16, Wind Speed], [SINT16, Grade], [UINT8 CRR], [UINT8, CW]
#define FTMP_EVENT_VAL_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION_CHANGED   0b0000000000000001

#endif /* #ifndef MICROBIT_CUSTOM_H */
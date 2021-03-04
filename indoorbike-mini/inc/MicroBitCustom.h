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

// driver/MicroBitIndoorBikeMiniSensor
//#ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR
//#define CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR (MICROBIT_CUSTOM_ID_BASE+1)
//#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SENSOR */
//// Event Value - Crank Interval
//#define MICROBIT_INDOOR_BIKE_MINI_SENSOR_EVT_DATA_UPDATE 0b0000000000000001

// driver/MicroBitIndoorBikeMiniServo
//#ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO
//#define CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO (MICROBIT_CUSTOM_ID_BASE+2)
//#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVO */
//// Event Value - Resistance Level
//#define MICROBIT_INDOOR_BIKE_MINI_SERVO_EVT_DATA_CHANGED 0b0000000000000001
#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_10
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_10 40
#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_10 */
#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_20
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_20 60
#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_20 */
#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_30
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_30 70
#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_30 */
#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_40
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_40 80
#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_40 */
#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_50
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_50 85
#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_50 */
#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_60
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_60 90
#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_60 */
#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_70
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_70 95
#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_70 */
#ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_80
#define MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_80 100
#endif /* #ifndef MICROBIT_INDOOR_BIKE_MINI_SERVO_ANGLE_80 */

// driver/MicroBitIndoorBikeMiniController
#ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_CONTROLLER
#define CUSTOM_EVENT_ID_INDOORBIKE_MINI_CONTROLLER (MICROBIT_CUSTOM_ID_BASE+3)
#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_CONTROLLER */
// Event Value                                                 7654321076543210
#define MICROBIT_INDOOR_BIKE_MINI_CONTROLLER_EVT_DATA_UPDATE 0b0000000000000001

// bluetooth/MicroBitIndoorBikeMiniService
// # Fitness Machine Control Point Procedure Requirements
//// Custom Event ID
//#ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVICE
//#define CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVICE (MICROBIT_CUSTOM_ID_BASE+4)
//#endif /* #ifndef CUSTOM_EVENT_ID_INDOORBIKE_MINI_SERVICE */
//// FTMP Event Val
//// # 0x11 C.14 Set Indoor Bike Simulation [SINT16, Wind Speed], [SINT16, Grade], [UINT8 CRR], [UINT8, CW]
//#define MICROBIT_INDOOR_BIKE_MINI_SERVO_EVT_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION_CHANGED  0b0000000000000001

/*
 * MicroBitIndoorBikeMiniService
 */

#ifndef BLE_DEVICE_LOCAL_NAME_CHENGE
#define BLE_DEVICE_LOCAL_NAME_CHENGE 1
#endif /* #ifndef BLE_DEVICE_LOCAL_NAME_CHENGE */

#ifndef BLE_DEVICE_LOCAL_NAME
#define BLE_DEVICE_LOCAL_NAME "BIKEMINI:BIT"
#endif /* #ifndef BLE_DEVICE_LOCAL_NAME */

// # SINT16 Unitless with a resolution of 0.1
#define VAL_MINIMUM_RESISTANCE_LEVEL    10
// # SINT16 Unitless with a resolution of 0.1
#ifndef VAL_MAXIMUM_RESISTANCE_LEVEL
#define VAL_MAXIMUM_RESISTANCE_LEVEL    80
#endif /* #ifndef VAL_MAXIMUM_RESISTANCE_LEVEL */
// # UINT16 Unitless with a resolution of 0.1
#ifndef VAL_INCREMENT_RESISTANCE_LEVEL
#define VAL_INCREMENT_RESISTANCE_LEVEL  10
#endif /* #ifndef VAL_INCREMENT_RESISTANCE_LEVEL */

#endif /* #ifndef MICROBIT_CUSTOM_H */
/*
Copyright 2022 Ulrich Spörlein

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID    0x444D
#define MANUFACTURER tshort

#define PRODUCT      Daqtyl (4x5)
#define PRODUCT_ID 0x3435  // for 4x5
#define DEVICE_VER 0x0101

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 10

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 */
#define MATRIX_ROW_PINS { E6, B4, B5, B7, F4 }
#define MATRIX_COL_PINS { D7, C6, D4, D0, D1, B6, F0, F1, C7, D5 }
#define UNUSED_PINS // { B0 }

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* Well-worn Cherry MX key switches can bounce for up to 20ms, despite the
 * Cherry data sheet specifying 5ms. Because we use the sym_eager_pk debounce
 * algorithm, this debounce latency only affects key releases (not key
 * presses). */
#undef DEBOUNCE
#define DEBOUNCE 10

/* EVQWGD001 encoders are apparently A, B, C, - as seen from the 2 switch pins.
 */
#define ENCODERS_PAD_A { F5, D3 }
#define ENCODERS_PAD_B { D3, D2 }
#define ENCODER_RESOLUTION 2

/* PMW3360 sensor(s)
 */
#define PMW3360_CS_PINS { F6, F7 }
// global defs
//#define POINTING_DEVICE_INVERT_X
//#define ROTATIONAL_TRANSFORM_ANGLE 180 - 135
// per sensor defs
#define POINTING_DEVICE_ROTATION_pwm3360 { 0, 90 }
// These apply _before_ rotating, so you need to make sure to flip the right one.
#define POINTING_DEVICE_INVERT_XY_pwm3360 { { true, false }, { true, false } }

#define PERMISSIVE_HOLD
#define IGNORE_MOD_TAP_INTERRUPT

/* mouse config */
#define MOUSEKEY_INTERVAL    20
#define MOUSEKEY_DELAY       0
#define MOUSEKEY_TIME_TO_MAX 60
#define MOUSEKEY_MAX_SPEED   7
#define MOUSEKEY_WHEEL_DELAY 0

// Reduce input latency by lowering the USB polling interval
// from its 10ms default to the 125μs minimum that USB 2.x (High Speed) allows:
#define USB_POLLING_INTERVAL_MS 1

/*
Copyright 2021 Ulrich Spörlein

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
#define MATRIX_ROWS 2
#define MATRIX_COLS 2

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
 */
#define MATRIX_ROW_PINS { LINE_PIN3, LINE_PIN4 }
#define MATRIX_COL_PINS { LINE_PIN19, LINE_PIN20 }
#define UNUSED_PINS

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* Well-worn Cherry MX key switches can bounce for up to 20ms, despite the
 * Cherry data sheet specifying 5ms. Because we use the sym_eager_pk debounce
 * algorithm, this debounce latency only affects key releases (not key
 * presses). */
#undef DEBOUNCE
#define DEBOUNCE 20

/* EVQWGD001 encoders are apparently A, B, C, - as seem from the 2 switch pins.
 */
#define ENCODERS_PAD_A { LINE_PIN16 /*, LINE_PIN14 */ }
#define ENCODERS_PAD_B { LINE_PIN15 /*, LINE_PIN37 */ }
#define ENCODER_RESOLUTION 2

/* PMW3360 sensor(s)
 */
//#define PMW3360_CS_PIN SPI_SS_PIN //LINE_PIN10
#define PMW3360_CS_PIN LINE_PIN10

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

/* We use the i.MX RT1060 high-speed GPIOs (GPIO6-9) which are connected to the
 * AHB bus (AHB_CLK_ROOT), which runs at the same speed as the ARM Core Clock,
 * i.e. 600 MHz. See MIMXRT1062, page 949, 12.1 Chip-specific GPIO information.
 * No additional delay is necessary. */

// in clock cycles
#define GPIO_INPUT_PIN_DELAY 0


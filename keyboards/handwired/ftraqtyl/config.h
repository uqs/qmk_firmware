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

/* Well-worn Cherry MX key switches can bounce for up to 20ms, despite the
 * Cherry data sheet specifying 5ms. Because we use the sym_eager_pk debounce
 * algorithm, this debounce latency only affects key releases (not key
 * presses). */
#undef DEBOUNCE
#define DEBOUNCE 10

/* EVQWGD001 encoders are apparently A, B, C, - as seen from the 2 switch pins.
 */
#define ENCODER_RESOLUTION 4

/* PMW3360 sensor(s)
 */
#define PMW33XX_CS_PINS { F7 }
#define POINTING_DEVICE_ROTATION_90
#define POINTING_DEVICE_INVERT_X

/* mouse config */
#define MOUSEKEY_INTERVAL    20
#define MOUSEKEY_DELAY       0
#define MOUSEKEY_TIME_TO_MAX 60
#define MOUSEKEY_MAX_SPEED   7

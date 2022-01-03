/* Copyright 2022 Ulrich Spörlein
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "daqtyl.h"
#include "pointing_device.h"
#include "drivers/sensors/pmw3360.h"

#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init_kb(void) {
    pmw3360_init(1);
    pointing_device_set_cpi(800);

    pointing_device_init_user();
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  debug_mouse=true;
  // Set TCNT3 to count ticks of 4us each.
  TCCR3A = 0; // TCCR0A register set to 0
  TCCR3B = 0; // same with registreb B
  TCNT3  = 0; // counter value to 0
  // set prescaler to 64, should be 4us per tick then ...
  TCCR3B |=(1<<CS31)|(1<<CS30);
}

static int16_t signed_sat_add16(int16_t a, int16_t b) {
    int16_t res;
    bool overflow = __builtin_add_overflow(a, b, &res);
    if (overflow) {
        // if b was positive, this will return INT16_MAX, if b was negative,
        // it'll slap its negative sign bit onto INT16_MAX, returning
        // INT16_MIN.
        return ((uint16_t)b >> 15) + INT16_MAX;
    }
    return res;
}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    report_pmw3360_t data = pmw3360_read_burst(1);
    if (data.isOnSurface && data.isMotion) {
// From quantum/pointing_device_drivers.c
#define constrain_hid(amt) ((amt) < -127 ? -127 : ((amt) > 127 ? 127 : (amt)))
        mouse_report.x = constrain_hid(signed_sat_add16(mouse_report.x, data.dx));
        mouse_report.y = constrain_hid(signed_sat_add16(mouse_report.y, data.dy));
    }

    return pointing_device_task_user(mouse_report);
}
#endif

//#ifdef POINTING_DEVICE_ENABLE
//void matrix_power_up(void) { pointing_device_task(); }
//#endif


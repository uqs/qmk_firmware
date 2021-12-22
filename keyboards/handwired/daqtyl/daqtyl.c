/* Copyright 2021 Ulrich Spörlein
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

#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init_kb(void) {
    //pointing_device_init_user();
    pointing_device_set_cpi(255);
}

//report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
//    if (is_keyboard_master()) {
//        mouse_report = pointing_device_task_user(mouse_report);
//    }
//    return mouse_report;
//}
#endif

//#ifdef POINTING_DEVICE_ENABLE
//void matrix_power_up(void) { pointing_device_task(); }
//#endif


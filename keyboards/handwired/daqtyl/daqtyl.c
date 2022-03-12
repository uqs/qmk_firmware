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
    pointing_device_set_cpi(500);

    pointing_device_init_user();
}

// TODO: the sensors are installed at a 45 degree angle, which avoids
// installing them at the bottom, which should help keep them a bit more dust
// free. But this means they "see" less of a distance travelled in one of the
// axis, which we need to compensate for.
// TODO: insert trigonometry for this

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    report_pmw3360_t data = pmw3360_read_burst(1);
    if (data.isOnSurface && data.isMotion) {
// From quantum/pointing_device_drivers.c
#define constrain_hid(amt) ((amt) < -127 ? -127 : ((amt) > 127 ? 127 : (amt)))
        // This would need a saturated add to be correct, but the generated
        // values are typically small enough and moving 2 sensors at the same
        // time is rare.
        mouse_report.x = constrain_hid(mouse_report.x + data.dx);
        mouse_report.y = constrain_hid(mouse_report.y + data.dy);
    }

    return pointing_device_task_user(mouse_report);
}
#endif

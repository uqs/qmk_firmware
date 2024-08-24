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
#include "drivers/sensors/pmw33xx_common.h"

#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init_kb(void) {
    // calling this freezes the board, works when it's _not_ called for index 0
    // though. Need to check pins? Both sensors were working before I merged
    // the pmw33xx common changes in Jan 2023. So yeah, not sure what's up
    // there
    //pmw33xx_init(1);

    pointing_device_set_cpi(400);
    pointing_device_init_user();
}

// TODO: the sensors are installed at a 45 degree angle, which avoids
// installing them at the bottom, which should help keep them a bit more dust
// free. But this means they "see" less of a distance travelled in one of the
// axis, which we need to compensate for.
// TODO: insert trigonometry for this

#if 1

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    pmw33xx_report_t data = pmw33xx_read_burst(1);
    if (data.motion.b.is_lifted && data.motion.b.is_motion) {
// From quantum/pointing_device_drivers.c
#define constrain_hid(amt) ((amt) < -127 ? -127 : ((amt) > 127 ? 127 : (amt)))
        // This would need a saturated add to be correct, but the generated
        // values are typically small enough and moving 2 sensors at the same
        // time is rare.
        mouse_report.x = constrain_hid(mouse_report.x + data.delta_x);
        mouse_report.y = constrain_hid(mouse_report.y + data.delta_y);
    }

    // From https://www.reddit.com/r/ErgoMechKeyboards/comments/zttki9/developments_in_posture_efficiency_and_mouse/
    // Using pow() explodes the image size by about 2200 bytes.
    // Check out https://www.wolframalpha.com/input?i=plot+%28x%5E2%29+%2F+4+%2B+x+and+x%5E3+%2F+16+%2B+x+and+x+%28x%5E1.8%29+%2B+1.5*x+for+x%3D-10+to+10
    if (mouse_report.x != 0 || mouse_report.y != 0) {
#if 1
        dprintf("turning x/y %d %d", mouse_report.x, mouse_report.y);
        mouse_xy_report_t x = mouse_report.x;
        mouse_xy_report_t y = mouse_report.y;
        x = (x*x*x) / 64 + x;
        y = (y*y*y) / 64 + y;
        mouse_report.x = x;
        mouse_report.y = y;
        dprintf(" into x/y %d %d\n", mouse_report.x, mouse_report.y);
#endif
    }
    return pointing_device_task_user(mouse_report);
}
#endif
#endif

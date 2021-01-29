/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

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

#define USE_SERIAL

//#define MASTER_LEFT
//#define MASTER_RIGHT
#define EE_HANDS
// Rows are doubled-up

// If you are using an Elite C rev3 on the slave side, uncomment the lines below:
//#define SPLIT_USB_DETECT
//#define NO_USB_STARTUP_CHECK

#define RGBLED_NUM 12
#define RGBLED_SPLIT {6,6}

#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_SLEEP
//#define RGBLIGHT_LAYERS

#define DYNAMIC_KEYMAP_LAYER_COUNT 5  // default is 4 for VIA builds

#define LEADER_TIMEOUT 300
#define LEADER_PER_KEY_TIMING

#define UNICODE_SELECTED_MODES UC_LNX

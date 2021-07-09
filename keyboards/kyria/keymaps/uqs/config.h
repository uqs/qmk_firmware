/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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

#pragma once

#ifdef OLED_DRIVER_ENABLE
  #define OLED_DISPLAY_128X64
#endif

#ifdef RGBLIGHT_ENABLE
  //#define RGBLIGHT_ANIMATIONS
  #define RGBLIGHT_HUE_STEP 8
  #define RGBLIGHT_SAT_STEP 8
  #define RGBLIGHT_VAL_STEP 8
  #define RGBLIGHT_LIMIT_VAL 150
#endif

// If you are using an Elite C rev3 on the slave side, uncomment the lines below:
//#define SPLIT_USB_DETECT
//#define NO_USB_STARTUP_CHECK
#define EE_HANDS

//#define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_SLEEP
#define RGBLIGHT_LAYERS
#define RGBLIGHT_MAX_LAYERS 8  // default is 16

#define DYNAMIC_KEYMAP_LAYER_COUNT 6  // default is 4 for VIA builds

#define TAPPING_TOGGLE 2  // number of taps for a toggle-on-tap
#define TAPPING_TERM 170  // ms to trigger tap
#define IGNORE_MOD_TAP_INTERRUPT
// Might have to look at TAPPING_FORCE_HOLD, see https://precondition.github.io/home-row-mods

#define LEADER_TIMEOUT 400
#define LEADER_PER_KEY_TIMING

#define UNICODE_SELECTED_MODES UC_LNX

//#define PREVENT_STUCK_MODIFIERS

// KC_ACL0 et al work when held.
#define MK_COMBINED
#define MOUSEKEY_WHEEL_INTERVAL 40  // default is 50, lower means more scroll events

#define ENCODER_RESOLUTION 2

// From https://michael.stapelberg.ch/posts/2021-05-08-keyboard-input-latency-qmk-kinesis/
#define USB_POLLING_INTERVAL_MS 1

// Testing OSM_SHIFT for CAPS_WORDS
#define ONESHOT_TAP_TOGGLE 2

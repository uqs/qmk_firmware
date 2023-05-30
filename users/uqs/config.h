// Copyright 2022 Ulrich Spörlein (@uqs)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_SLEEP
#    define RGBLIGHT_LAYERS
#    define RGBLIGHT_MAX_LAYERS 8      // default is 16
#    define RGBLIGHT_DISABLE_KEYCODES  // RGB_foo keys no longer work, saves 600 bytes
#    define RGBLIGHT_DEFAULT_HUE 15
#endif

#define DYNAMIC_KEYMAP_LAYER_COUNT 6  // default is 4 for VIA builds

#define TAPPING_TOGGLE 2  // number of taps for a toggle-on-tap
#define TAPPING_TERM 200  // ms to trigger tap
#define TAPPING_TERM_PER_KEY
// https://precondition.github.io/home-row-mods
#define TAPPING_FORCE_HOLD  // make tap-then-hold _not_ do key auto repeat
#define TAPPING_FORCE_HOLD_PER_KEY  // ... but do it for some!
#define HOLD_ON_OTHER_KEY_PRESS  // obsolete my LT_NUM_BSPC
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY  // ... but not for mod-taps!
#define IGNORE_MOD_TAP_INTERRUPT
#define PERMISSIVE_HOLD  // I don't think this works for me, hence I rolled my own implementation.

#define LEADER_TIMEOUT 400
#define LEADER_PER_KEY_TIMING

//#define COMBO_TERM 80  // default is 40ms

#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX

// make KC_ACL0 et al work when held.
#define MK_COMBINED
#define MOUSEKEY_WHEEL_DELAY 10
#define MOUSEKEY_WHEEL_INTERVAL 100 // 80 is default
#define MOUSEKEY_WHEEL_MAX_SPEED 4
#define MOUSEKEY_WHEEL_TIME_TO_MAX 250 // 40 is default

#define AUTO_BUTTONS_TIMEOUT 750

// From https://michael.stapelberg.ch/posts/2021-05-08-keyboard-input-latency-qmk-kinesis/
#define USB_POLLING_INTERVAL_MS 1
// Too quick polling of trackball makes acceleration code useless
#define POINTING_DEVICE_TASK_THROTTLE_MS 8
#define QMK_KEYS_PER_SCAN 4

#ifdef KEYBOARD_preonic_rev3
// Some games seem to not register Esc otherwise when tapped, maybe try with this delay?
#    define TAP_CODE_DELAY 30
#else
#    define TAP_CODE_DELAY 10
#endif

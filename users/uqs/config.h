#pragma once

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

// From https://michael.stapelberg.ch/posts/2021-05-08-keyboard-input-latency-qmk-kinesis/
//#define USB_POLLING_INTERVAL_MS 1
#define TAP_CODE_DELAY 10

// Testing OSM_SHIFT for CAPS_WORDS
#define ONESHOT_TAP_TOGGLE 2

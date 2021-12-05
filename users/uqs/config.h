#pragma once

#ifdef RGBLIGHT_ENABLE
  //#define RGBLIGHT_ANIMATIONS
  #define RGBLIGHT_SLEEP
  // Disable these to save loads of space in the EEPROM
  //#define RGBLIGHT_LAYERS
  //#define RGBLIGHT_MAX_LAYERS 8  // default is 16
  #define RGBLIGHT_DISABLE_KEYCODES  // RGB_foo keys no longer work, saves 600 bytes
  #define RGBLIGHT_DEFAULT_HUE 15
#endif

#define DYNAMIC_KEYMAP_LAYER_COUNT 6  // default is 4 for VIA builds

#define TAPPING_TOGGLE 2  // number of taps for a toggle-on-tap
#define TAPPING_TERM 170  // ms to trigger tap
// https://precondition.github.io/home-row-mods
#define TAPPING_FORCE_HOLD  // make tap-then-hold _not_ do key auto repeat
#define IGNORE_MOD_TAP_INTERRUPT
#define PERMISSIVE_HOLD  // I don't think this works for me, hence I rolled my own implementation.

#define LEADER_TIMEOUT 400
#define LEADER_PER_KEY_TIMING

#define UNICODE_SELECTED_MODES UC_LNX

//#define PREVENT_STUCK_MODIFIERS

// KC_ACL0 et al work when held.
#define MK_COMBINED
#define MOUSEKEY_WHEEL_INTERVAL 40  // default is 50, lower means more scroll events, 40 works ok.

// From https://michael.stapelberg.ch/posts/2021-05-08-keyboard-input-latency-qmk-kinesis/
#define USB_POLLING_INTERVAL_MS 1
//#define TAP_CODE_DELAY 10


// Copyright 2022 Ulrich Spörlein (@uqs)
// SPDX-License-Identifier: GPL-2.0-or-later
// vi:et sw=4:

#include "uqs.h"
#include "pointing_device.h"

// LOG:
// late Jan 2020, got Ohkeycaps Dactyl Manuform 5x6
// https://play.typeracer.com shows about 75-80wpm (en) or ~400cpm (de) on my classic keeb.
// Never did proper touch typing, basically didn't use ring finger much, mostly index/middle and pinky (only to hold down modifiers, really).
// Feb 2020, switching to Colemak DH after 30 years of Qwerty, uh oh...
// mid Feb, 20wpm/87% on monkeytype.com (no punct, numbers)
// early March, 28wpm/90% on MT (plus punct./numbers from here on); 25wpm on typeracer
// early April, 35wpm/92% on MT; 41wpm on typeracer
// early May, 45wpm/96% on MT; 46wpm on typeracer; my qwerty is deteriorating, I need to look at the keys more and more o_O
// early June, 49wpm/95% on MT (sigh ...); 50wpm on typeracer;
// early July, 50wpm/96% on MT (...); 52wpm/96% on typeracer;
// early August, 55wpm/96% on MT; 55wpm/98% on typeracer;
// early September, 57wpm/97% on MT; 58wpm/97% on typeracer;
// early October, 59wpm/96% on MT; 61wpm/97% on typeracer;
// November, 56wpm/97% on MT; 62wpm/98% on typeracer;
// December, 62wpm/96% on MT; 66wpm/98% on typeracer;
// January, 61wpm/97% on MT; 65wpm/98% on typeracer;
// February, 64wpm/97% on MT; 67wpm/98% on typeracer; my qwerty on the laptop is still fine, but I miss my shortcuts badly.
//
// So that's one year on Colemak. Was it worth the switch? Probably not, though
// I also had to first learn proper technique, but that was actually swift, as
// the keyboard nicely forces that on you. I really like home row mods though,
// they are so comfy. Need to rethink my combos some more, still.


#ifdef RGBLIGHT_LAYERS
layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(L_QWER, layer_state_cmp(state, L_QWER));
    rgblight_set_layer_state(L_WASD, layer_state_cmp(state, L_WASD));
    rgblight_set_layer_state(L_COLM, layer_state_cmp(state, L_COLM));
    return state;
}
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
#if 0
    // defining layer L_FUNC when both keys are pressed
    state = update_tri_layer_state(state, L_EXTD, L_NUM, L_FUNC);
#endif
#ifdef RGBLIGHT_LAYERS
    rgblight_set_layer_state(L_EXTD, layer_state_cmp(state, L_EXTD));
    rgblight_set_layer_state(L_NUM, layer_state_cmp(state, L_NUM));
    rgblight_set_layer_state(L_FUNC, layer_state_cmp(state, L_FUNC));
    rgblight_set_layer_state(L_MOUSE, layer_state_cmp(state, L_MOUSE));
#else
#endif
    return state;
}

#ifdef RGBLIGHT_LAYERS
// NOTE: at most 2 elements, last one needs to be RGBLIGHT_END_SEGMENTS
typedef rgblight_segment_t rgblight_layer_t[3];

const rgblight_layer_t PROGMEM my_rgb_segments[] = {
    [L_QWER] = {{0, RGBLED_NUM, HSV_WHITE},  RGBLIGHT_END_SEGMENTS},
    [L_WASD] = {{0, RGBLED_NUM/2, HSV_RED}, {RGBLED_NUM/2, RGBLED_NUM/2, HSV_OFF}, RGBLIGHT_END_SEGMENTS},
    [L_COLM] = {{0, RGBLED_NUM, HSV_GREEN},  RGBLIGHT_END_SEGMENTS},
    [L_EXTD] = {{0, RGBLED_NUM, HSV_BLUE},   RGBLIGHT_END_SEGMENTS},
    [L_NUM] =  {{0, RGBLED_NUM, HSV_ORANGE}, RGBLIGHT_END_SEGMENTS},
    [L_FUNC] = {{0, RGBLED_NUM, HSV_YELLOW}, RGBLIGHT_END_SEGMENTS},
    [L_MOUSE]= {{0, RGBLED_NUM, HSV_PURPLE}, RGBLIGHT_END_SEGMENTS},
};

// This array needs pointers, :/
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = {
    my_rgb_segments[L_QWER],
    my_rgb_segments[L_WASD],
    my_rgb_segments[L_COLM],
    my_rgb_segments[L_EXTD],
    my_rgb_segments[L_NUM],
    my_rgb_segments[L_FUNC],
    my_rgb_segments[L_MOUSE],
};

_Static_assert(sizeof(my_rgb_layers) / sizeof(my_rgb_layers[0]) ==
        sizeof(my_rgb_segments) / sizeof(my_rgb_segments[0]),
        "Number of rgb_segment definitions does not match up!");
#endif

#ifdef COMBO_ENABLE
enum combo_events {
  C_AUML,
  C_OUML,
  C_UUML,
  C_SZ,
  C_CBR,
  C_PRN,
  C_BRC,
  C_QUOT,
};

// Maybe use this?
// #define COMBO_ONLY_FROM_LAYER L_COLM

// The official way has way too much duplication and intermediate names for my taste...
const uint16_t PROGMEM my_action_combos[][3] = {
#ifndef KEYBOARD_preonic_rev3
    [C_AUML] = {KC_G_A, KC_W, COMBO_END},
    [C_OUML] = {KC_G_O, KC_Y, COMBO_END},
    [C_UUML] = {KC_C_N, KC_U, COMBO_END},
    [C_SZ]   = {KC_S_S, KC_Z, COMBO_END},
#else
    [C_AUML] = {COMBO_END},
    [C_OUML] = {COMBO_END},
    [C_UUML] = {COMBO_END},
    [C_SZ]   = {COMBO_END},
#endif
    [C_CBR]  = {KC_COLN, KC_LCBR, COMBO_END},
    [C_PRN]  = {KC_LCBR, KC_LPRN, COMBO_END},
    [C_BRC]  = {KC_LPRN, KC_LBRC, COMBO_END},
    [C_QUOT] = {KC_COMM, KC_DOT, COMBO_END},
};
const uint16_t PROGMEM my_combos[][4] = {
#ifndef KEYBOARD_preonic_rev3
    {KC_RCBR, KC_X, KC_C, COMBO_END},  // hotkey for gmail archive
    {KC_TAB,  KC_G_A, KC_A_R, COMBO_END},
    {KC_BSLS, KC_F, KC_U, COMBO_END},
    {LSFT(KC_BSLS), KC_P, KC_L, COMBO_END},
    {KC_MINUS, KC_C_T, KC_C_N, COMBO_END},
    {LSFT(KC_MINUS), KC_D, KC_H, COMBO_END},
    {KC_GRV,  KC_Q, KC_W, COMBO_END},  // remove this? turn into esc:wq?
    {KC_GRV,  KC_C, KC_COMM, COMBO_END},
    {LSFT(KC_GRV), KC_G, KC_M, COMBO_END},
#else
    {COMBO_END},
    {COMBO_END},
    {KC_BSLS, KC_E, KC_I, COMBO_END},
    {LSFT(KC_BSLS), KC_R, KC_U, COMBO_END},
    {KC_MINUS, KC_F, KC_J, COMBO_END},
    {LSFT(KC_MINUS), KC_V, KC_M, COMBO_END},
    {COMBO_END},
    {KC_GRV,  KC_C, KC_COMM, COMBO_END},
    {LSFT(KC_GRV), KC_G, KC_H, COMBO_END},
#endif
    {KC_BTN3, KC_BTN1, KC_BTN2, COMBO_END},
    {KC_BTN1, KC_BTN2, KC_BTN3, COMBO_END},
};

const uint16_t COMBO_LEN = sizeof(my_action_combos) / sizeof(my_action_combos[0]) + sizeof(my_combos) / sizeof(my_combos[0]);

#define MY_ACTION_COMBO(ck) \
    [ck] = { .keys = &(my_action_combos[ck][0]) }
#define MY_COMBO(ck) \
    { .keys = &(my_combos[ck][1]), .keycode = my_combos[ck][0] }

// NOTE: while my_combos can live in PROGMEM, the key_combos data also
// contains state that is tweaked at runtime, so we need to indirect. Ugh.
#define COMBO_STATICALLY
#ifdef COMBO_STATICALLY
// TODO: fill this at runtime with a loop?
combo_t key_combos[] = {
  MY_ACTION_COMBO(0),
  MY_ACTION_COMBO(1),
  MY_ACTION_COMBO(2),
  MY_ACTION_COMBO(3),
  MY_ACTION_COMBO(4),
  MY_ACTION_COMBO(5),
  MY_ACTION_COMBO(6),
  MY_ACTION_COMBO(7),
  MY_COMBO(0),
  MY_COMBO(1),
  MY_COMBO(2),
  MY_COMBO(3),
  MY_COMBO(4),
  MY_COMBO(5),
  MY_COMBO(6),
  MY_COMBO(7),
  MY_COMBO(8),
  MY_COMBO(9),
  MY_COMBO(10),
};

_Static_assert(sizeof(key_combos) / sizeof(key_combos[0]) ==
        (sizeof(my_action_combos) / sizeof(my_action_combos[0]) + sizeof(my_combos) / sizeof(my_combos[0])),
        "Number of combo definitions does not match up!");
#else
combo_t key_combos[sizeof(my_action_combos) / sizeof(my_action_combos[0]) + sizeof(my_combos) / sizeof(my_combos[0])];
#endif

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case C_AUML:
            if (pressed) {
                tap_code16(KC_RALT);
                tap_code16(LSFT(KC_QUOT));
                tap_code16(KC_A);
            }
            break;
        case C_OUML:
            if (pressed) {
                tap_code16(KC_RALT);
                tap_code16(LSFT(KC_QUOT));
                tap_code16(KC_O);
            }
            break;
        case C_UUML:
            if (pressed) {
                tap_code16(KC_RALT);
                tap_code16(LSFT(KC_QUOT));
                tap_code16(KC_U);
            }
            break;
        case C_SZ:
            if (pressed) {
                tap_code16(KC_RALT);
                tap_code16(KC_S);
                tap_code16(KC_S);
            }
            break;
        case C_CBR:
            if (pressed) {
                tap_code16(KC_LCBR);
                tap_code16(KC_RCBR);
                tap_code16(KC_LEFT);
            }
            break;
        case C_PRN:
            if (pressed) {
                tap_code16(KC_LPRN);
                tap_code16(KC_RPRN);
                tap_code16(KC_LEFT);
            }
            break;
        case C_BRC:
            if (pressed) {
                tap_code16(KC_LBRC);
                tap_code16(KC_RBRC);
                tap_code16(KC_LEFT);
            }
            break;
        case C_QUOT:
            if (pressed) {
                tap_code16(KC_QUOT);
                tap_code16(KC_QUOT);
                // need to drop shift here, if present!
                unregister_mods(MOD_BIT(KC_LSFT));
                unregister_mods(MOD_BIT(KC_RSFT));
                tap_code16(KC_LEFT);
            }
            break;
    }
}
#endif

void keyboard_post_init_user(void) {
#ifndef KEYBOARD_preonic_rev3
    debug_enable=true;
    //debug_matrix=true;
    debug_keyboard=true;
    debug_mouse=true;
    // Set TCNT3 to count ticks of 4us each.
    TCCR3A = 0; // TCCR0A register set to 0
    TCCR3B = 0; // same with registreb B
    TCNT3  = 0; // counter value to 0
    // set prescaler to 64, should be 4us per tick then ...
    TCCR3B |=(1<<CS31)|(1<<CS30);
    dprintf("keyboard_post_init_user done\n");
#endif
#ifndef KEYBOARD_preonic_rev3
    default_layer_set(1ul << L_COLM);
#endif
#ifdef RGBLIGHT_LAYERS
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
    rgblight_set_layer_state(0, true);
#endif
#if defined(COMBO_ENABLE) && !defined(COMBO_STATICALLY)
    uint8_t i = 0;
    for (; i < sizeof(my_action_combos) / sizeof(my_action_combos[0]); i++) {
        key_combos[i].keys = &(my_action_combos[i][0]);
    }
    for (uint8_t j = 0; j < sizeof(my_combos) / sizeof(my_combos[0]); j++, i++) {
        key_combos[i].keycode = my_combos[j][0];
        key_combos[i].keys = &(my_combos[j][1]);
    }
#endif
}

#ifdef TAPPING_TERM_PER_KEY
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_G_A:
        case KC_A_R:
        case KC_A_I:
        case KC_G_O:
            return TAPPING_TERM + 50;
        case KC_C_T:
        case KC_C_N:
            return TAPPING_TERM - 50;
        case KC_S_S:
        case KC_S_E:
            return TAPPING_TERM - 80;
        default:
            return TAPPING_TERM;
    }
}
#endif

// Make the backspace and tab be holds on the tap key, not the hold key on
// double-tap-and-hold
#ifdef TAPPING_FORCE_HOLD_PER_KEY
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(L_FUNC, KC_TAB):
        //case LT(L_NUM, KC_BSPC):  // would be nice, but can't backspace + number entry quickly anymore
        //case LT(L_MOUSE, DRAG_SCROLL):  // doesn't work, as DRAG_SCROLL is then not handled as a keypress event
            return false;
            // For mod-taps aka home row mods, default to holding the hold
            // function, so I can type 's' followed by holding it, to get a
            // shifted '?'. If this sounds reversed, that's because I have
            // TAPPING_FORCE_HOLD defined globally, so tapping will force a
            // hold of the hold function, not a hold of the tapping function.
            // See https://github.com/qmk/qmk_firmware/pull/7859
        default:
            return true;
    }
}
#endif

// This works to replace my custom LT_NUM_BSPC, but it must not apply to mod-taps!
#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(L_NUM, KC_BSPC):
            // Immediately select the hold action when another key is pressed.
            return true;
        default:
            // Do not select the hold action when another key is pressed.
            return false;
    }
}
#endif

uint16_t key_timer;
bool delkey_registered;
bool num_layer_was_used;
bool extd_layer_was_used;
// These keep state about the long-press-means-umlaut keys.
bool auml_pressed;
bool ouml_pressed;
bool uuml_pressed;

void maybe_send_umlaut(uint16_t keycode, bool *is_pressed) {
    // Some other key did _not_ already re-arm this key, so now we need to do
    // that ourselves.
    if (*is_pressed) {
        *is_pressed = false;
        // If released within the timer, then just KC_A, KC_O, KC_U
        if (timer_elapsed(key_timer) < TAPPING_TERM) {
            tap_code16(keycode);
        } else {
            tap_code16(KC_RALT);
            tap_code16(LSFT(KC_QUOT));
            tap_code16(keycode);
        }
    }
}

bool set_scrolling = false;
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (set_scrolling /*|| layer_state_is(L_FUNC) */) {
#if 0
        static uint32_t last_exec = 0;
        if (timer_elapsed32(last_exec) < 20 /*ms*/) {
            mouse_report.x = mouse_report.y = 0;
            return mouse_report;
        }
        last_exec = timer_read32();
        if (mouse_report.x > 0) {
            mouse_report.h = 1;
        } else if (mouse_report.x < 0) {
            mouse_report.h = -1;
        }
        if (mouse_report.y > 0) {
            mouse_report.v = -1;
        } else if (mouse_report.y < 0) {
            mouse_report.v = 1;
        }
#else
        mouse_report.h =  mouse_report.x;
        mouse_report.v = -mouse_report.y;
#endif
        mouse_report.x = mouse_report.y = 0;
        // TODO: accumulate movement in a var and spit it out when a threshold has been reached.
        if (mouse_report.h != 0 || mouse_report.v != 0) {
            dprintf("drag_scroll report sending: h=%d v=%d\n", mouse_report.h, mouse_report.v);
        }
    }
    return mouse_report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // TODO: why not use key_timer here? is it dynamic or not?
    static uint16_t extd_layer_timer;
    if (layer_state_is(L_EXTD) && record->event.pressed) {
        extd_layer_was_used = true;
    }
    if (layer_state_is(L_NUM) && record->event.pressed) {
        num_layer_was_used = true;
    }

    // An umlaut key was pressed previously (but will only emit the key on
    // release), but we've pressed a different key now, so fire the regular key,
    // re-arm it and continue with whatever actual key was pressed just now.
    if (record->event.pressed) {
        if (auml_pressed) {
            tap_code16(KC_A);
            auml_pressed = false;
        }
        if (ouml_pressed) {
            tap_code16(KC_O);
            ouml_pressed = false;
        }
        if (uuml_pressed) {
            tap_code16(KC_U);
            uuml_pressed = false;
        }
    }
    if (keycode == DRAG_SCROLL2) {
        if (record->event.pressed) {
            set_scrolling = !set_scrolling;
            return true;
        }
    }
    if (keycode == DRAG_SCROLL) {
        if (record->event.pressed) {
            dprintf("drag scroll pressed\n");
            set_scrolling = 1;
#if 0
            // Done by clamping the output values instead
            if (set_scrolling) {
                pointing_device_set_cpi(100);
            } else {
                pointing_device_set_cpi(600);
            }
#endif
        } else {
            dprintf("drag scroll UNpressed\n");
            set_scrolling = 0;
        }
        return true;
    }

    static bool force_shift = false;

    switch (keycode) {
        // From https://github.com/qmk/qmk_firmware/issues/6053
    case LT_EXTD_ESC:
        if (record->event.pressed) {
            extd_layer_was_used = false;
            extd_layer_timer = timer_read();
            layer_on(L_EXTD);
        } else {
            layer_off(L_EXTD);
            unregister_mods(MOD_BIT(KC_LALT));   // undo what ALT_TAB might've set
            // NOTE: need to track whether we made use of the extd layer and
            // that all happened within the tapping term. Otherwise we'd emit
            // that layer key code _plus_ an extra Esc.
            if (timer_elapsed(extd_layer_timer) < TAPPING_TERM && !extd_layer_was_used) {
                tap_code(KC_ESC);
            }
        }
        return true;
    case LT_NUM_BSPC:
        if (record->event.pressed){
            num_layer_was_used = false;
            extd_layer_timer = timer_read();
            layer_on(L_NUM);
        } else {
            layer_off(L_NUM);
            // NOTE: Custom LT method so that any press of a key on that layer will prevent the backspace.
            if (timer_elapsed(extd_layer_timer) < TAPPING_TERM && !num_layer_was_used) {
                tap_code(KC_BSPC);
            }
        }
        return true;
#if 1
        /* Looks like PERMISSIVE_HOLD on LT and OSM doesn't work properly. This
         * is probably https://github.com/qmk/qmk_firmware/issues/8971
         */
    case OSM_GUI:
        /* OSM(MOD_LGUI) is delaying the event, but I need immediate triggering
         * of the modifier to move windows around with the mouse. If only
         * tapped, however, have it be a win OSM */
        if (record->event.pressed) {
            key_timer = timer_read();
            register_mods(MOD_BIT(KC_LGUI));
        } else {
            unregister_mods(MOD_BIT(KC_LGUI));
            if (timer_elapsed(key_timer) < TAPPING_TERM) {
                add_oneshot_mods(MOD_BIT(KC_LGUI));
            } else {
                del_oneshot_mods(MOD_BIT(KC_LGUI));
            }
        }
        return true;
        // Why do I have to roll my own? It seems the original ones work on
        // keyrelease, at which time I might have let go of the layer tap
        // already, so I cannot roll them fast...
    case OSM_SFT:
        if (record->event.pressed) {
            key_timer = timer_read();
            register_mods(MOD_BIT(KC_LSFT));
        } else {
            unregister_mods(MOD_BIT(KC_LSFT));
            if (timer_elapsed(key_timer) < TAPPING_TERM) {
                add_oneshot_mods(MOD_BIT(KC_LSFT));
            } /*else {
                del_oneshot_mods(MOD_BIT(KC_LSFT));
            }*/
        }
        return true;
    case OSM_CTL:
        if (record->event.pressed) {
            key_timer = timer_read();
            register_mods(MOD_BIT(KC_LCTL));
        } else {
            unregister_mods(MOD_BIT(KC_LCTL));
            if (timer_elapsed(key_timer) < TAPPING_TERM) {
                add_oneshot_mods(MOD_BIT(KC_LCTL));
            } /*else {
                del_oneshot_mods(MOD_BIT(KC_LCTL));
            }*/
        }
        return true;
    case OSM_ALT:
        if (record->event.pressed) {
            key_timer = timer_read();
            register_mods(MOD_BIT(KC_LALT));
        } else {
            unregister_mods(MOD_BIT(KC_LALT));
            if (timer_elapsed(key_timer) < TAPPING_TERM) {
                add_oneshot_mods(MOD_BIT(KC_LALT));
            } /*else {
                del_oneshot_mods(MOD_BIT(KC_LALT));
            }*/
        }
        return true;
#else
#define OSM_ALT OSM(MOD_LALT)
#define OSM_CTL OSM(MOD_LCTL)
#define OSM_GUI OSM(MOD_LGUI)
#define OSM_SFT OSM(MOD_LSFT)
#endif
    // Obsoleted by using combos for umlauts now.
    case KC_A_AE:
        if (record->event.pressed) {
            key_timer = timer_read();
            auml_pressed = true;
        } else {
            maybe_send_umlaut(KC_A, &auml_pressed);
        }
        break;
    case KC_O_OE:
        if (record->event.pressed) {
            key_timer = timer_read();
            ouml_pressed = true;
        } else {
            maybe_send_umlaut(KC_O, &ouml_pressed);
        }
        break;
    case KC_U_UE:
        if (record->event.pressed) {
            key_timer = timer_read();
            uuml_pressed = true;
        } else {
            maybe_send_umlaut(KC_U, &uuml_pressed);
        }
        break;
    case MINS_UNDSCR:
        if (record->event.pressed) {
            key_timer = timer_read();
        } else {
            if (timer_elapsed(key_timer) < TAPPING_TERM) {
                // Can't send KC_KP_MINUS, it doesn't compose to, say →
                tap_code16(KC_MINUS);
            } else {
                tap_code16(KC_UNDERSCORE);
            }
        }
        break;
        // This turns on dragscroll when held, otherwise '"
    case LT(3,KC_NO):
        if (record->tap.count > 0 && record->event.pressed) {
            tap_code16(KC_QUOT); // Intercept tap function
        } else if (record->event.pressed) {
#if defined(POINTING_DEVICE_ENABLE)
            set_scrolling = 1;
            pointing_device_set_cpi(100);
        } else {
            set_scrolling = 0;
            pointing_device_set_cpi(500);
#endif
        }
        return false;
        break;
        // Need to remember if this was pressed, to make the RCTL_T(KC_N) work
        // with that key held.
        // BUG: should one roll ST improperly, the default handling of
        // LSFT_T(KS_S) will eventually clear the shift mod, even though it
        // wasn't the one that added it. So shift can get lost here
    case SFT_T(KC_SPC):
        if (record->event.pressed) {
            force_shift = true;
        } else {
            force_shift = false;
            unregister_mods(MOD_BIT(KC_LSHIFT));
            unregister_mods(MOD_BIT(KC_RSHIFT));
        }
        break;
    case KC_S_S:
        if (record->event.pressed) {
            return true;  // process normally
        } else {
            // restore shift, if we're in force_shift
            if (force_shift) {
                add_mods(MOD_BIT(KC_LSHIFT));
            }
        }
        break;
    case KC_S_E:
        if (record->event.pressed) {
            return true;  // process normally
        } else {
            // restore shift, if we're in force_shift
            if (force_shift) {
                add_mods(MOD_BIT(KC_RSHIFT));
            }
        }
        break;
    // From https://precondition.github.io/home-row-mods#rolled-modifiers-cancellation
    case KC_C_N:
        if (record->event.pressed && record->tap.count > 0) {
            // Detect right Shift
            if (!force_shift && get_mods() & MOD_BIT(KC_RSHIFT)) {
                // temporarily disable right Shift
                // so that we can send KC_E and KC_N
                // without Shift on.
                unregister_mods(MOD_BIT(KC_RSHIFT));
                tap_code(KC_E);
                tap_code(KC_N);
                // restore the mod state
                add_mods(MOD_BIT(KC_RSHIFT));
                // to prevent QMK from processing RCTL_T(KC_N) as usual in our special case
                return false;
            }
        }
        /*else process RCTL_T(KC_N) as usual.*/
        break;
        /* This also uses right shift, as holding the SFT_T(KC_SPC) key shall *not* produce the lowercase st, as it's my sort-of-caps-lock key.
         * TODO: how to detect that key-hold and skip it here?
         */
    case KC_C_T:
        if (record->event.pressed && record->tap.count > 0) {
            if (!force_shift && get_mods() & MOD_BIT(KC_LSHIFT)) {
                unregister_mods(MOD_BIT(KC_LSHIFT));
                tap_code(KC_S);
                tap_code(KC_T);
                add_mods(MOD_BIT(KC_LSHIFT));
                return false;
            }
        }
        break;
    case ALT_TAB:
        if (record->event.pressed) {
            register_mods(MOD_BIT(KC_LALT));
            tap_code16(KC_TAB);
        }
        break;
    case INS_HARD:
        // Do Alt-Shift-Ins first to have xdotool copy from SELECTION to CLIPBOARD, then Shift-Ins to paste.
        if (record->event.pressed) {
            tap_code16(LSFT(LALT(KC_INS)));
        } else {
            tap_code16(LSFT(KC_INS));
        }
        break;
    case SHIFT_INS:
        if (record->event.pressed) {
            // when keycode is pressed
            key_timer = timer_read();
            // Shift when held ...
            register_mods(MOD_BIT(KC_RSFT));
        } else {
            // If released within the timer, then Shift+Ins
            if (timer_elapsed(key_timer) < TAPPING_TERM) {
                tap_code16(KC_INS);
            }
            unregister_mods(MOD_BIT(KC_RSFT));
        }
        break;
    case ALT_SHIFT_INS:
        if (record->event.pressed) {
            key_timer = timer_read();
            // Shift when held ...
            register_mods(MOD_BIT(KC_LSFT));
        } else {
            // If released within the timer, then Shift+Alt+Ins
            if (timer_elapsed(key_timer) < TAPPING_TERM) {
                register_mods(MOD_BIT(KC_LALT));
                tap_code16(KC_INS);
            }
            // Note: this makes xev(1) see KeyPress for Meta_L but KeyRelease for Alt_L
            unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT));
        }
        break;
/*
 * Obsoleted by making tmux understand Ctrl-(Shift)-Tab natively.
    case TM_NEXT:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("a") "n");
        break;
    case TM_PREV:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("a") "p");
        break;
*/
        // TODO: use key overrides to turn, e.g. Win+Ctrl-Tab into VIM_NEXT.
        // Not sure why Ctrl-Pgup works in vim, but not in vim-inside-tmux.
    case VIM_NEXT:
        if (record->event.pressed) SEND_STRING(SS_TAP(X_ESC) SS_TAP(X_G) SS_TAP(X_T));
        break;
    case VIM_PREV:
        if (record->event.pressed) SEND_STRING(SS_TAP(X_ESC) SS_TAP(X_G) SS_LSFT("t"));
        break;
    case WIN_LEFT:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") SS_TAP(X_H));
        break;
    case WIN_DN:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") SS_TAP(X_J));
        break;
    case WIN_UP:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") SS_TAP(X_K));
        break;
    case WIN_RGHT:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") SS_TAP(X_L));
        break;
    }

    return true;
}

#ifdef LEADER_ENABLE
LEADER_EXTERNS();

void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

#ifdef UCIS_ENABLE
    SEQ_ONE_KEY(KC_U) {
      qk_ucis_start();
    }
    SEQ_ONE_KEY(KC_H) {
      send_unicode_string("ᕕ( ᐛ )ᕗ");  // happy
    }
    SEQ_ONE_KEY(KC_D) {
      send_unicode_string("ಠ_ಠ");  // disapproval
    }
    SEQ_ONE_KEY(KC_L) {
      send_unicode_string("( ͡° ͜ʖ ͡°)");  // lenny
    }
    SEQ_ONE_KEY(KC_S) {
      send_unicode_string("¯\\_(ツ)_/¯");  // shrug
    }
    // tableflip (LEADER - TF)
    SEQ_TWO_KEYS(KC_T, KC_F) {
      //set_unicode_input_mode(UC_LNX);
      //send_unicode_hex_string("0028 30CE 0CA0 75CA 0CA0 0029 30CE 5F61 253B 2501 253B");
      send_unicode_string("(╯°□°）╯︵ ┻━┻");
    }
    // untableflip
    SEQ_THREE_KEYS(KC_U, KC_T, KC_F) {
      //set_unicode_input_mode(UC_LNX);
      //send_unicode_hex_string("0028 30CE 0CA0 75CA 0CA0 0029 30CE 5F61 253B 2501 253B");
      send_unicode_string("┬─┬ノ( º _ ºノ)");
    }
#endif
  }
}
#endif

#ifdef UCIS_ENABLE
// 3 codepoints at most, otherwise increase UCIS_MAX_CODE_POINTS
const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE(
    UCIS_SYM("poop", 0x1F4A9),                // 💩
    UCIS_SYM("rofl", 0x1F923),                // 🤣
    UCIS_SYM("look", 0x0CA0, 0x005F, 0x0CA0)  // ಠ_ಠ
);
#endif

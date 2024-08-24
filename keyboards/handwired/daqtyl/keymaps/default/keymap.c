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
#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    L_QWER = 0,
    L_EXTD,
    L_NUM,
    L_FUNC,
    L_MOUSE,
    L_LAST, // unused
};

#define KC_CTAB LCTL(KC_TAB)
#define KC_SCTAB LCTL(LSFT(KC_TAB))

#ifndef LEADER_ENABLE
#define QK_LEAD KC_NO
#endif

// Shorter names
#define MS_WHDN KC_MS_WH_DOWN
#define MS_WHUP KC_MS_WH_UP
#define MS_WHLEFT KC_MS_WH_LEFT
#define MS_WHRGHT KC_MS_WH_RIGHT

// GASC/◆⎇⇧⎈ home row mod, read all about it here:
// https://precondition.github.io/home-row-mods
// Left-hand home row mods
#define KC_G_A LGUI_T(KC_A)
#define KC_A_R LALT_T(KC_R)
#define KC_S_S LSFT_T(KC_S)
#define KC_C_T LCTL_T(KC_T)

// Right-hand home row mods
#define KC_C_N RCTL_T(KC_N)
#define KC_S_E RSFT_T(KC_E)
#define KC_A_I LALT_T(KC_I)  // RALT is special, it's AltGr and my compose key under Win (layout UScmpse) and *nix (setxkbmap -option compose:ralt)
#define KC_G_O RGUI_T(KC_O)

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    SHIFT_INS = SAFE_RANGE,
    ALT_SHIFT_INS,
    INS_HARD,
    KC_A_AE,
    KC_O_OE,
    KC_U_UE,
    MINS_UNDSCR,  // obsoleted by combos, remove this!
    TM_NEXT,
    TM_PREV,
    VIM_NEXT,
    VIM_PREV,
    WIN_LEFT,
    WIN_RGHT,
    WIN_UP,
    WIN_DN,
    T_PANE,
    LT_EXTD_ESC,
    LT_NUM_BSPC,
    OSM_GUI,
    OSM_SFT,
    OSM_CTL,
    OSM_ALT,
    ALT_TAB,
    DRAG_SCROLL,
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(MS_WHDN);
        } else {
            tap_code(MS_WHUP);
        }
    } else if (index == 1) { /* Second encoder */
        if (clockwise) {
            tap_code(MS_WHUP);
        } else {
            tap_code(MS_WHDN);
        }
    }
    return false;
}

static bool set_scrolling = false;
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (set_scrolling) {
        mouse_report.h =  mouse_report.x;
        mouse_report.v = -mouse_report.y;
        mouse_report.x = mouse_report.y = 0;
        // TODO: accumulate movement in a var and spit it out when a threshold has been reached?
        if (mouse_report.h != 0 || mouse_report.v != 0) {
            //dprintf("dragscroll report sending: h=%d v=%d\n", mouse_report.h, mouse_report.v);
        }
    }

    return mouse_report;
}

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
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [L_QWER] = LAYOUT(
               KC_BTN2,  KC_BTN3,  KC_BTN1,
     KC_Q,  KC_W,    KC_E,    KC_R,   KC_T, /**/ KC_Y, KC_U,    KC_I,    KC_O,   LT(3,KC_NO) /*this is KC_P when tapped*/,
     KC_A,  KC_S,    KC_D,    KC_F,   KC_G, /**/ KC_H, KC_J,    KC_K,    KC_L,   KC_QUOT,
     KC_Z,  KC_X,    KC_C,    KC_V,   KC_B, /**/ KC_N, KC_M,    KC_COMM, KC_DOT, KC_SLSH,
            KC_LCTL, KC_LALT,               /**/       KC_RALT, KC_RCTL,
  /*thumb*/ LT_EXTD_ESC, LSFT_T(KC_SPC), LT(L_FUNC, KC_TAB),/**/ MO(L_MOUSE), RSFT_T(KC_ENT), LT(L_NUM, KC_BSPC)
    ),
    [L_EXTD] = LAYOUT(
               _______,  _______,  _______,
     T_PANE,   KC_SCTAB, KC_PGUP,  KC_CTAB,  QK_LEAD,  /**/ KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   KC_INS,
     OSM_GUI,  OSM_ALT,  OSM_SFT,  OSM_CTL,  KC_RALT,  /**/ KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_DEL,
     ALT_TAB,  MS_WHUP,  MS_WHDN,  KC_PGDN,  INS_HARD, /**/ WIN_LEFT, WIN_DN,   WIN_UP,   WIN_RGHT, KC_PSTE,
               _______,  _______,                      /**/           _______,  _______,
  /*thumb*/              _______,  _______,  _______,  /**/ _______,  KC_ENT,   KC_BSPC
    ),
    [L_NUM] = LAYOUT(
               _______,  _______,  _______,
     KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  /**/ KC_KP_ASTERISK, KC_7, KC_8,   KC_9,     KC_KP_PLUS,
     KC_SCLN,  KC_COLN,  KC_LCBR,  KC_LPRN,  KC_LBRC,  /**/ KC_EQUAL, KC_4,     KC_5,     KC_6,     MINS_UNDSCR,
     KC_CIRC,  KC_AMPR,  KC_RCBR,  KC_RPRN,  KC_RBRC,  /**/ KC_0,     KC_1,     KC_2,     KC_3,     KC_KP_SLASH,
               _______,  _______,                      /**/           KC_0,     KC_KP_DOT,
  /*thumb*/              KC_ESC,   KC_SPC,   KC_TAB,   /**/ _______,  _______,  _______
    ),
    [L_FUNC] = LAYOUT(
               _______,  _______,  _______,
     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    /**/ KC_PSCR,  KC_F7,    KC_F8,    KC_F9,    KC_F10,
     KC_LGUI,  KC_LALT,  KC_LSFT,  KC_LCTL,  KC_NO,    /**/ KC_SCRL,  KC_F4,    KC_F5,    KC_F6,    KC_F11,
     KC_NO,    KC_VOLU,  KC_VOLD,  KC_MUTE,  KC_NO,    /**/ KC_PAUS,  KC_F1,    KC_F2,    KC_F3,    KC_F12,
               _______,  _______,                      /**/           _______,  _______,
  /*thumb*/              KC_ESC,   KC_SPC,   KC_TAB,   /**/ _______,  _______,  _______
    ),
    [L_MOUSE] = LAYOUT(
               _______,  _______,  _______,
     KC_BTN1,  KC_BTN2,  KC_PGUP,  KC_BTN1,  KC_BTN3,  /**/ KC_BTN3,  KC_BTN1,  KC_MS_U,  KC_BTN2,  _______,
     KC_LGUI,  KC_LALT,  KC_LSFT,  KC_LCTL,  KC_NO,    /**/ KC_NO,    KC_MS_L,  KC_MS_D,  KC_MS_R,  KC_NO,
     ALT_TAB,  MS_WHUP,  MS_WHDN,  KC_PGDN,  KC_NO,    /**/ KC_BTN1,  KC_BTN2,  KC_BTN3,  KC_BTN4,  KC_BTN5,
               _______,  _______,                      /**/           _______,  _______,
  /*thumb*/              _______,  KC_BTN1,  KC_BTN2,  /**/ _______,  _______,  _______
    ),
};

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
    if (keycode == DRAG_SCROLL) {
#if defined(POINTING_DEVICE_ENABLE)
        if (record->event.pressed) {
            dprintf("drag scroll pressed\n");
            set_scrolling = 1;
            pointing_device_set_cpi(100);
        } else {
            dprintf("drag scroll UNpressed\n");
            set_scrolling = 0;
            pointing_device_set_cpi(400);
        }
#endif
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
        // This turns on dragscroll when held, otherwise p
    case LT(3,KC_NO):
        if (record->tap.count > 0 && record->event.pressed) {
            tap_code16(KC_P); // Intercept tap function
        } else if (record->event.pressed) {
#if defined(POINTING_DEVICE_ENABLE)
            set_scrolling = 1;
            pointing_device_set_cpi(100);
        } else {
            set_scrolling = 0;
            pointing_device_set_cpi(400);
#endif
        }
        return false;
        break;
        // Need to remember if this was pressed, to make the RCTL_T(KC_N) work
        // with that key held.
        // BUG: should one roll ST improperly, the default handling of
        // LSFT_T(KS_S) will eventually clear the shift mod, even though it
        // wasn't the one that added it. So shift can get lost here
    case LSFT_T(KC_SPC):
        if (record->event.pressed) {
            force_shift = true;
        } else {
            force_shift = false;
            unregister_mods(MOD_BIT(KC_LSFT));
            unregister_mods(MOD_BIT(KC_RSFT));
        }
        break;
    case RSFT_T(KC_ENT):
        if (record->event.pressed) {
            force_shift = true;
        } else {
            force_shift = false;
            unregister_mods(MOD_BIT(KC_LSFT));
            unregister_mods(MOD_BIT(KC_RSFT));
        }
        break;
    case KC_S_S:
        if (record->event.pressed) {
            return true;  // process normally
        } else {
            // restore shift, if we're in force_shift
            if (force_shift) {
                add_mods(MOD_BIT(KC_LSFT));
            }
        }
        break;
    case KC_S_E:
        if (record->event.pressed) {
            return true;  // process normally
        } else {
            // restore shift, if we're in force_shift
            if (force_shift) {
                add_mods(MOD_BIT(KC_RSFT));
            }
        }
        break;
    // From https://precondition.github.io/home-row-mods#rolled-modifiers-cancellation
    case KC_C_N:
        if (record->event.pressed && record->tap.count > 0) {
            // Detect right Shift
            if (!force_shift && get_mods() & MOD_BIT(KC_RSFT)) {
                // temporarily disable right Shift
                // so that we can send KC_E and KC_N
                // without Shift on.
                unregister_mods(MOD_BIT(KC_RSFT));
                tap_code(KC_E);
                tap_code(KC_N);
                // restore the mod state
                add_mods(MOD_BIT(KC_RSFT));
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
            if (!force_shift && get_mods() & MOD_BIT(KC_LSFT)) {
                unregister_mods(MOD_BIT(KC_LSFT));
                tap_code(KC_S);
                tap_code(KC_T);
                add_mods(MOD_BIT(KC_LSFT));
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
        if (record->event.pressed) SEND_STRING(SS_LCTL("a") "n");
        break;
    case TM_PREV:
        if (record->event.pressed) SEND_STRING(SS_LCTL("a") "p");
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
        if (record->event.pressed) SEND_STRING(SS_LCTL("w") SS_TAP(X_H));
        break;
    case WIN_DN:
        if (record->event.pressed) SEND_STRING(SS_LCTL("w") SS_TAP(X_J));
        break;
    case WIN_UP:
        if (record->event.pressed) SEND_STRING(SS_LCTL("w") SS_TAP(X_K));
        break;
    case WIN_RGHT:
        if (record->event.pressed) SEND_STRING(SS_LCTL("w") SS_TAP(X_L));
        break;
    case T_PANE:
        if (record->event.pressed) SEND_STRING(SS_LCTL("a") SS_TAP(X_SCLN));
        break;
    }

    return true;
}

#ifdef COMBO_ENABLE
enum combo_events {
  C_AUML,
  C_OUML,
  C_UUML,
  C_SZ,
  C_CBR,
  C_PRN,
  C_BRC,
};

// The official way has way too much duplication and intermediate names for my taste...
const uint16_t PROGMEM my_action_combos[][3] = {
    [C_AUML] = {KC_G_A, KC_W, COMBO_END},
    [C_OUML] = {KC_G_O, KC_Y, COMBO_END},
    [C_UUML] = {KC_C_N, KC_U, COMBO_END},
    [C_SZ]   = {KC_S_S, KC_Z, COMBO_END},
    [C_CBR]  = {KC_COLN, KC_LCBR, COMBO_END},
    [C_PRN]  = {KC_LCBR, KC_LPRN, COMBO_END},
    [C_BRC]  = {KC_LPRN, KC_LBRC, COMBO_END},
};
const uint16_t PROGMEM my_combos[][4] = {
    {KC_RCBR, KC_X, KC_C, COMBO_END},  // hotkey for gmail archive
    {KC_TAB,  KC_A, KC_S, COMBO_END},
    {KC_BSLS, KC_E, KC_I, COMBO_END},
    {LSFT(KC_BSLS), KC_R, KC_U, COMBO_END},
    {KC_MINUS, KC_F, KC_J, COMBO_END},
    {KC_MINUS, KC_LPRN, KC_4, COMBO_END},  // dupe to work on NUM layer
    {LSFT(KC_MINUS), KC_V, KC_M, COMBO_END},
    {LSFT(KC_MINUS), KC_RPRN, KC_1, COMBO_END},  // dupe to work on NUM
    {KC_GRV,  KC_Q, KC_W, COMBO_END},  // remove this? turn into esc:wq?
    {KC_GRV,  KC_C, KC_COMM, COMBO_END},
    {LSFT(KC_GRV), KC_G, KC_H, COMBO_END},
    {KC_BTN3, KC_BTN1, KC_BTN2, COMBO_END},
    {KC_BTN1, KC_BTN2, KC_BTN3, COMBO_END},
};

const uint16_t COMBO_LEN = ARRAY_SIZE(my_action_combos) + ARRAY_SIZE(my_combos);

#define MY_ACTION_COMBO(ck) \
    [ck] = { .keys = &(my_action_combos[ck][0]) }
#define MY_COMBO(ck) \
    { .keys = &(my_combos[ck][1]), .keycode = my_combos[ck][0] }

// NOTE: while my_combos can live in PROGMEM, the key_combos data also
// contains state that is tweaked at runtime, so we need to indirect. Ugh.
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
  MY_COMBO(11),
};

_Static_assert(ARRAY_SIZE(key_combos) ==
               (ARRAY_SIZE(my_action_combos) + ARRAY_SIZE(my_combos)),
               "Number of combo definitions does not match up!");

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
    }
}
#endif

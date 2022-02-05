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
    _BASE,
    _FN
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    QMKURL
};

#if 0

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_QWER] = LAYOUT_uqs(
               KC_BTN2,  KC_BTN3,  KC_BTN1,
     KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,
     KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
     KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,
                         KC_LALT,  KC_LGUI,
     /*thumb*/ LT_EXTD_ESC, SFT_T(KC_SPC), LT(L_MOUSE, KC_TAB),
     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,
     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,
     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,
                         KC_MINS,  KC_EQL,
     /*thumb*/ LT_FUNC_SHIFT_INS, KC_ENT, LT_NUM_BSPC
  ),

  [L_WASD] = LAYOUT_uqs(
          // XXX missing KC_GRV?
               KC_2,     KC_3,     KC_4,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,
     KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,
                         KC_LALT,  KC_LGUI,
     /*thumb*/ LT_EXTD_ESC,  KC_SPC,   KC_R,
     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,
     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,
     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,
                         KC_MINS,  KC_EQL,
     /*thumb*/ DF(L_COLM), KC_ENT, KC_BSPC
     // idea, change DF(L_COLM) to only MO COLM when held (and DF on tap), that
     // would allow typing with regular layout while pressing that key. hitting
     // enter/bkspc is then still doable just fine, I think.
  ),

  // TODO: implement CAPS_WORD on the left pinky shift on tap (maybe?) But it
  // can't actually set caps lock, as I'm rebinding that for a saner laptop
  // keyboard. See drashna's keymap.
  [L_COLM] = LAYOUT_uqs(
               KC_BTN2,  KC_BTN3,  KC_BTN1,
     KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,
     KC_G_A,   KC_A_R,   KC_S_S,   KC_C_T,   KC_G,
     KC_Z,     KC_X,     KC_C,     KC_D,     KC_V,
               MS_WHUP,  MS_WHDN, // these are on the encoder
     /*thumb*/ LT_EXTD_ESC, SFT_T(KC_SPC), LT(L_MOUSE, KC_TAB),
     KC_J,     KC_L,     KC_U,     KC_Y,     KC_QUOT,
     KC_M,     KC_C_N,   KC_S_E,   KC_A_I,   KC_G_O,
     KC_K,     KC_H,     KC_COMM,  KC_DOT,   KC_SLSH,
                         KC_NO,    KC_NO,
     /*thumb*/ LT_FUNC_SHIFT_INS, KC_ENT, LT_NUM_BSPC

  ),
};

#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
            KC_BTN2, KC_BTN3, KC_BTN1,
     KC_Q,  KC_W,    KC_E,    KC_R,   KC_T, /**/ KC_Y, KC_U,    KC_I,    KC_O,   KC_P,
     KC_A,  KC_S,    KC_D,    KC_F,   KC_G, /**/ KC_H, KC_J,    KC_K,    KC_L,   KC_SCLN,
     KC_Z,  KC_X,    KC_C,    KC_V,   KC_B, /**/ KC_N, KC_M,    KC_COMM, KC_DOT, KC_SLSH,
            KC_LALT, KC_LGUI,               /**/       KC_MINS, KC_EQL,
  /*thumb*/ KC_LCTL, SFT_T(KC_SPC), KC_LALT,/**/ KC_RGUI, KC_TAB, KC_RSFT
    ),
};

// TODO: get a SQUAL reading from the sensors!
bool encoder_update_user(uint8_t index, bool clockwise) {
#ifdef POINTING_DEVICE_ENABLE
    static const uint16_t us_per_tick = 64 / (F_CPU/1000000);
#endif
    if (index == 0) { /* First encoder */
        if (clockwise) {
#if defined(CONSOLE_ENABLE) && defined(POINTING_DEVICE_ENABLE)
            uint16_t tcnt1 = TCNT3;
            pointing_device_set_cpi(100);
            uint16_t tcnt2 = TCNT3;
            dprintf("set_cpi took: %u ticks which is %u us\n", tcnt2-tcnt1, us_per_tick*(tcnt2-tcnt1));
#endif
            tap_code(KC_UP);
        } else {
#if defined(CONSOLE_ENABLE) && defined(POINTING_DEVICE_ENABLE)
            uint16_t tcnt1 = TCNT3;
            uint16_t cpi = pointing_device_get_cpi();
            uint16_t tcnt2 = TCNT3;
            dprintf("get_cpi took: %u ticks which is %u us: cpi is %u\n", tcnt2-tcnt1, us_per_tick*(tcnt2-tcnt1), cpi);
#endif
            tap_code(KC_DOWN);
        }
    } else if (index == 1) { /* Second encoder */
        if (clockwise) {
            tap_code(KC_LEFT);
        } else {
            tap_code(KC_RIGHT);
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QMKBEST:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("QMK is the best thing ever!");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case QMKURL:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                SEND_STRING("https://qmk.fm/\n");
            } else {
                // when keycode QMKURL is released
            }
            break;
    }
    return true;
}

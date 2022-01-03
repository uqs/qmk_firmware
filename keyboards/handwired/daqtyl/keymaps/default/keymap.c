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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
        KC_A,    KC_1,
            KC_B,   KC_2
    ),
    [_FN] = LAYOUT(
        QMKBEST, QMKURL,
            RESET,    XXXXXXX
    )
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    static const uint16_t us_per_tick = 64 / (F_CPU/1000000);
    if (index == 0) { /* First encoder */
        if (clockwise) {
#ifdef CONSOLE_ENABLE
            uint16_t tcnt1 = TCNT3;
            pointing_device_set_cpi(100);
            uint16_t tcnt2 = TCNT3;
            dprintf("set_cpi took: %u ticks which is %u us\n", tcnt2-tcnt1, us_per_tick*(tcnt2-tcnt1));
#endif
            tap_code(KC_P);
        } else {
#ifdef CONSOLE_ENABLE
            uint16_t tcnt1 = TCNT3;
            uint16_t cpi = pointing_device_get_cpi();
            uint16_t tcnt2 = TCNT3;
            dprintf("get_cpi took: %u ticks which is %u us: cpi is %u\n", tcnt2-tcnt1, us_per_tick*(tcnt2-tcnt1), cpi);
#endif
            tap_code(KC_U);
        }
    } else if (index == 1) { /* Second encoder */
        if (clockwise) {
            tap_code(KC_5);
        } else {
            tap_code(KC_6);
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

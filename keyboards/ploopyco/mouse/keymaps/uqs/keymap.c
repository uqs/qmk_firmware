/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
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

enum custom_keycodes {
  DBL_CLK_NO = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case DBL_CLK_NO:
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_BTN2) SS_DELAY(300) SS_TAP(X_BTN2) SS_DELAY(300) SS_TAP(X_N));
      }
      return false;
  }
  return true;
}


// Layout is:
// left-most, M1, M3, M2, right-most, fwd, back (on side), tiny middle one

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(KC_LCTL, KC_BTN1, KC_BTN3, KC_BTN2, KC_LSFT, KC_BTN4, KC_BTN5, DF(1)),  // shooters
    [1] = LAYOUT(KC_1, KC_BTN1, KC_Y, KC_BTN2, DBL_CLK_NO, KC_N, KC_Y, DF(0)),        // stardew valley, sword on 1
};

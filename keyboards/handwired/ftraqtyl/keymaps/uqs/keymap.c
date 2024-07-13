// Copyright 2024 Ulrich Spörlein (@uqs)
// SPDX-License-Identifier: GPL-2.0-or-later
// vi:et sw=4 ve=all cc=5,15,25,35,45,55,56,57,65,75,85,95,105:

#include QMK_KEYBOARD_H
#include "uqs.h"

/*
 * My custom layout macro that allows for easy diff(1) operations between
 * various keyboards. It probably doesn't make sense to you.
 */
#define LAYOUT_uqs(          \
    L10, L11, L12, L13, L14,            R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,            R20, R21, R22, R23, R24, \
    L30, L31, L32, L33, L34,            R30, R31, R32, R33, R34, \
         L40, L41,                                R43, R44,      \
                   L42, L43, L44,  R40, R41, R42       \
) \
LAYOUT_split_4x5(            \
  L10, L11, L12, L13, L14,           R10, R11, R12, R13, R14, \
  L20, L21, L22, L23, L24,           R20, R21, R22, R23, R24, \
  L30, L31, L32, L33, L34,           R30, R31, R32, R33, R34, \
       L40, L41, L42, L43, L44, R40, R41, R42, R43, R44       \
)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_QWER] = LAYOUT_uqs(
     KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,
     KC_A,     KC_S,     KC_D,     KC_F,     KC_G,               KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,
     KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,               KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,
               KC_LBRC,  KC_RBRC,                                                    KC_MINS,  KC_EQL,
     LT_EXTD_ESC, LSFT_T(KC_SPC), LT(L_FUNC, KC_TAB),    MO(L_MOUSE), RSFT_T(KC_ENT), LT(L_NUM, KC_BSPC)
  ),

  // The encoder could up/down the mouse sensitivity maybe? Hard to do in game,
  // but possible if/when mouse is directly in the keyboard?
  [L_WASD] = LAYOUT_uqs(
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,               KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,
     KC_LSFT,  KC_A,     KC_S,     KC_D,     KC_F,               KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,
     KC_GRV,   KC_Z,     KC_X,     KC_C,     KC_V,               KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,
               KC_LGUI,  KC_LALT,                                                    KC_MINS,  KC_EQL,
     KC_LCTL,  KC_SPC,   KC_LALT,                        DF(L_COLM), KC_ENT, LT(L_NUM, KC_BSPC)
  ),                                                     // TODO: make DF a tap, on hold do an MO to COLM??

  [L_COLM] = LAYOUT_uqs(
     KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,               KC_J,     KC_L,     KC_U,     KC_Y,     LT(3, KC_NO),
     KC_G_A,   KC_A_R,   KC_S_S,   KC_C_T,   KC_G,               KC_M,     KC_C_N,   KC_S_E,   KC_A_I,   KC_G_O,
     KC_Z,     KC_X,     KC_C,     KC_D,     KC_V,               KC_K,     KC_H,     KC_COMM,  KC_DOT,   KC_SLSH,
               KC_NO,    KC_NO,                                                      KC_NO,    KC_NO,
     LT_EXTD_ESC, LSFT_T(KC_SPC), LT(L_FUNC, KC_TAB),    MO(L_MOUSE), RSFT_T(KC_ENT), LT(L_NUM, KC_BSPC)
  ),

  // Updated with inspiration from https://forum.colemak.com/topic/2014-extend-extra-extreme/
  // I like the AltGr trick from https://stevep99.github.io/seniply/ and should probably incorporate some stuff from it.
  [L_EXTD] = LAYOUT_uqs(
     T_PANE,   KC_SCTAB, KC_PGUP,  KC_CTAB,  QK_LEAD,            KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   KC_INS,
     OSM_GUI,  OSM_ALT,  OSM_SFT,  OSM_CTL,  KC_RALT,            KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_DEL,
     ALT_TAB,  MS_WHUP,  MS_WHDN,  KC_PGDN,  INS_HARD,           WIN_LEFT, WIN_DN,   WIN_UP,   WIN_RGHT, KC_PSTE,
               _______,  _______,                                                    _______,  _______,
                         _______,  _______,  _______,    _______,  KC_ENT,   KC_BSPC
  ),

  [L_NUM] = LAYOUT_uqs(
     KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,            KC_KP_ASTERISK, KC_7, KC_8,   KC_9,     KC_KP_PLUS,
     KC_SCLN,  KC_COLN,  KC_LCBR,  KC_LPRN,  KC_LBRC,            KC_EQUAL, KC_4,     KC_5,     KC_6,     MINS_UNDSCR,
     KC_CIRC,  KC_AMPR,  KC_RCBR,  KC_RPRN,  KC_RBRC,            KC_0,     KC_1,     KC_2,     KC_3,     KC_KP_SLASH,
               KC_NO,    KC_NO,                                                      KC_0,     KC_KP_DOT,
                         KC_ESC,   KC_SPC,   KC_TAB,     _______,  _______,  _______
  ),

  [L_FUNC] = LAYOUT_uqs(
     KC_NO,    KC_NO,    KC_NO,    KC_NO,    DF(L_QWER),         KC_PSCR,  KC_F7,    KC_F8,    KC_F9,    KC_F10,
     KC_LGUI,  KC_LALT,  KC_LSFT,  KC_LCTL,  DF(L_WASD),         KC_SCRL,  KC_F4,    KC_F5,    KC_F6,    KC_F11,
     KC_NO,    KC_VOLU,  KC_VOLD,  KC_MUTE,  DF(L_COLM),         KC_PAUS,  KC_F1,    KC_F2,    KC_F3,    KC_F12,
               KC_NO,    KC_NO,                                                      KC_NO,    KC_NO,
                         _______,  _______,  _______,    _______,  _______,  _______
  ),

  [L_MOUSE] = LAYOUT_uqs(
     KC_BTN1,  KC_BTN2,  KC_PGUP,  KC_BTN1,  KC_BTN3,            KC_BTN3,  KC_BTN1,  KC_BTN5,  KC_BTN2,  KC_BTN4,
     KC_LGUI,  KC_LALT,  KC_LSFT,  KC_LCTL,  KC_NO,              KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  MS_WHUP,
     KC_NO,    MS_WHUP,  MS_WHDN,  KC_PGDN,  KC_NO,              KC_MS_L,  KC_MS_D,  KC_MS_U,  KC_MS_R,  MS_WHDN,
               KC_NO,    KC_NO,                                                      KC_NO,    KC_NO,
                     LT_EXTD_ESC,  KC_BTN1,  KC_BTN2,    _______,  _______,  _______
  ),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(MS_WHDN);
        } else {
            tap_code(MS_WHUP);
        }
    }
    return false;
}

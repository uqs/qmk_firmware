// Copyright 2024 Ulrich Spörlein (@uqs)
// SPDX-License-Identifier: GPL-2.0-or-later
// vi:et sw=4 ve=all cc=5,15,25,35,45,55,65,75,85,95,105,115:

#include "uqs.h"

/*
 * My custom layout macro that allows for easy diff(1) operations between
 * various keyboards. It probably doesn't make sense to you.
 */
#define LAYOUT_uqs(               \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, \
    k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, \
    k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, \
    k40, k41, k42, k43,      k44, k46,      k48, k49, k4a, k4b  \
) \
LAYOUT_preonic_2x2u( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, \
    k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, \
    k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, \
    k40, k41, k42, k43, k44,      k46,      k48, k49, k4a, k4b \
)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_QWER] = LAYOUT_uqs(
     KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_BSPC,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
     KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,
     KC_GRV,   KC_LGUI,  KC_LALT,  MO(L_EXTD),         KC_SPC,   KC_ENT,          LT_NUM_BSPC, MO(L_FUNC), KC_NO,  KC_ENT
  ),

  [L_EXTD] = LAYOUT_uqs(
     KC_F11,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F12,
     _______,  T_PANE,   KC_SCTAB, KC_PGUP,  KC_CTAB,  QK_LEAD,  KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   KC_INS,   KC_NO,
     _______,  OSM_GUI,  OSM_ALT,  OSM_SFT,  OSM_CTL,  KC_RALT,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_DEL,   KC_NO,
     _______,  ALT_TAB,  MS_WHLU,  MS_WHLD,  KC_PGDN,  INS_HARD, WIN_LEFT, WIN_DN,   WIN_UP,   WIN_RGHT, KC_PSTE,  _______,
     _______,  _______,  _______, _______,             _______,  _______,            KC_BSPC, _______,  _______,   _______
  ),

  [L_NUM] = LAYOUT_uqs(
     KC_F11,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F12,
     _______,  KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_KP_ASTERISK, KC_7, KC_8,   KC_9,     KC_KP_PLUS,_______,
     _______,  KC_SCLN,  KC_COLN,  KC_LCBR,  KC_LPRN,  KC_LBRC,  KC_EQUAL, KC_4,     KC_5,     KC_6,     MINS_UNDSCR,_______,
     _______,  KC_CIRC,  KC_AMPR,  KC_RCBR,  KC_RPRN,  KC_RBRC,  KC_0,     KC_1,     KC_2,     KC_3,     KC_KP_SLASH,KC_KP_ENTER,  // Enter here, because thumb is occupied
     _______,  _______,  _______, _______,             _______,  _______,            _______,  KC_0,     KC_KP_DOT, _______
  ),

  [L_FUNC] = LAYOUT_uqs(
     _______,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
     _______,  MS_BTN1,  MS_BTN2,  MS_BTN3,  MS_BTN1,  KC_NO,    KC_PSCR,  KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_NO,
     _______,  KC_LGUI,  KC_LALT,  KC_LSFT,  KC_LCTL,  KC_NO,    KC_SCRL,  KC_F4,    KC_F5,    KC_F6,    KC_F11,   KC_NO,
     _______,  _______,  KC_VOLU,  KC_VOLD,  KC_MUTE,  KC_NO,    KC_PAUS,  KC_F1,    KC_F2,    KC_F3,    KC_F12,   KC_NO,
     _______,  _______,  _______, _______,             _______,  _______,            _______,  _______,  _______,  _______
  ),

  // No way to enter this layer
  [L_MOUSE] = LAYOUT_uqs(
     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
     _______,  _______,  _______, _______,             _______,  _______,            _______,  _______,  _______,  _______
  ),
};

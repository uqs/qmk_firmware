// Copyright 2025 Ulrich Spörlein (@uqs)
// SPDX-License-Identifier: GPL-2.0-or-later
// vi:et sw=4 ve=all cc=5,15,25,35,45,55,65,66,67,77,87,97,107,117:

#include "uqs.h"

/*
 * My custom layout macro that allows for easy diff(1) operations between
 * various keyboards. It probably doesn't make sense to you.
 */
#define LAYOUT_uqs(               \
    L00, L01, L02, L03, L04, L05,    R06, R07, R08, R09, R10, R11, \
    L12, L13, L14, L15, L16, L17,    R18, R19, R20, R21, R22, R23, \
    L24, L25, L26, L27, L28, L29,    R34, R35, R36, R37, R38, R39, \
              L40, L41,                        R48, R49,           \
                   L42, L43, L44,   R45, R46, R47,           \
                        L30, L31,   R32, R33                 \
)                                 \
LAYOUT_split_3x6_5( \
    L00, L01, L02, L03, L04, L05,                          R06, R07, R08, R09, R10, R11, \
    L12, L13, L14, L15, L16, L17,                          R18, R19, R20, R21, R22, R23, \
    L24, L25, L26, L27, L28, L29, L30, L31,      R32, R33, R34, R35, R36, R37, R38, R39, \
                   L40, L42, L43, L44, L44,      R45, R45, R46, R47, R49 \
)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_QWER] = LAYOUT_uqs(
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,       KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
     KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,       KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,       KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  RSFT_T(KC_GRV),
                         KC_LBRC,  KC_RBRC,                                            KC_MINS,  KC_EQL,
     /*thumb*/ LT_EXTD_ESC, KC_SPC, LT(L_MOUSE, KC_TAB),           /*thumb*/ LT(L_FUNC, QK_LEAD), KC_ENT, LT(L_NUM, KC_BSPC),
      /*aux*/  OSM_GUI,  KC_LALT,                                   /*aux*/  KC_RALT,  KC_APP
  ),

  [L_WASD] = LAYOUT_uqs(
     KC_T,     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,       KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
     KC_G,     KC_LSFT,  KC_A,     KC_S,     KC_D,     KC_F,       KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
     KC_B,     KC_GRV,   KC_Z,     KC_X,     KC_C,     KC_V,       KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  RSFT_T(KC_GRV),
                         KC_LBRC,  KC_RBRC,                                            KC_MINS,  KC_EQL,
     /*thumb*/ KC_LCTL,  KC_SPC,   KC_R,                           /*thumb*/ DF(L_COLM), KC_ENT, KC_BSPC,
      /*aux*/  OSM_GUI,  KC_LALT,                                   /*aux*/  KC_RALT,  KC_APP
  ),

  [L_COLM] = LAYOUT_uqs(
     MS_BTN2,  KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,       KC_J,     KC_L,     KC_U,     KC_Y,     KC_QUOT,  KC_NO,
     MS_BTN1,  KC_G_A,   KC_A_R,   KC_S_S,   KC_C_T,   KC_G,       KC_M,     KC_C_N,   KC_S_E,   KC_A_I,   KC_G_O,   KC_NO,
     MS_BTN3,  KC_Z,     KC_X,     KC_C,     KC_D,     KC_V,       KC_K,     KC_H,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_NO,
                         MS_WHLU,  MS_WHLD,                                            KC_NO,    KC_NO,
     /*thumb*/ LT_EXTD_ESC, LSFT_T(KC_SPC), LT(L_FUNC, KC_TAB),    /*thumb*/ MO(L_MOUSE), KC_ENT, LT_NUM_BSPC,
      /*aux*/  OSM_GUI,  KC_LALT,                                   /*aux*/  KC_RALT,  KC_APP
  ),

  // Updated with inspiration from https://forum.colemak.com/topic/2014-extend-extra-extreme/
  // I like the AltGr trick from https://stevep99.github.io/seniply/ and should probably incorporate some stuff from it.
  [L_EXTD] = LAYOUT_uqs(
     _______,  T_PANE,   KC_SCTAB, KC_PGUP,  KC_CTAB,  QK_LEAD,    KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   KC_INS,   KC_NO,
     _______,  OSM_GUI,  OSM_ALT,  OSM_SFT,  OSM_CTL,  KC_RALT,    KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_DEL,   KC_NO,
     _______,  ALT_TAB,  MS_WHLU,  MS_WHLD,  KC_PGDN,  INS_HARD,   WIN_LEFT, WIN_DN,   WIN_UP,   WIN_RGHT, KC_PSTE,  KC_NO,  // KC_PSTE works in XTerm to emulate middle-click
                         _______,  _______,                                            _______,  _______,
     /*thumb*/ _______,  _______,  _______,                        /*thumb*/ _______,  KC_ENT,   KC_BSPC,
      /*aux*/  _______,  _______,                                   /*aux*/  _______,  _______
              /* ^^^^ can't be used */                                      /* ^^^^ use these */
  ),

  [L_NUM] = LAYOUT_uqs(
     _______,  KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,    KC_KP_ASTERISK, KC_7,  KC_8,  KC_9,     KC_KP_PLUS,_______,
     _______,  KC_SCLN,  KC_COLN,  KC_LCBR,  KC_LPRN,  KC_LBRC,    KC_EQUAL, KC_4,     KC_5,     KC_6,     KC_MINS,  _______,
     _______,  KC_CIRC,  KC_AMPR,  KC_RCBR,  KC_RPRN,  KC_RBRC,    KC_0,     KC_1,     KC_2,     KC_3,     KC_KP_SLASH,KC_KP_ENTER,  // Enter here, because thumb is occupied
                         KC_GRV,   KC_TILDE,                                           KC_0,     KC_KP_DOT,
     /*thumb*/ _______,  _______,  _______,                        /*thumb*/ _______,  _______,  _______,
      /*aux*/  _______, _______,                                    /*aux*/  _______,  _______
              /* ^^^^ use these */                                          /* ^^^^ can't be used */
  ),

  [L_FUNC] = LAYOUT_uqs(
     _______,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    DF(L_QWER), KC_PSCR,  KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_NO,
     _______,  KC_LGUI,  KC_LALT,  KC_LSFT,  KC_LCTL,  DF(L_WASD), KC_SCRL,  KC_F4,    KC_F5,    KC_F6,    KC_F11,   KC_NO,
     _______,  KC_NO,    KC_VOLU,  KC_VOLD,  KC_MUTE,  DF(L_COLM), KC_PAUS,  KC_F1,    KC_F2,    KC_F3,    KC_F12,   KC_NO,
                         KC_NO,    KC_NO,                                              KC_NO,    KC_NO,
     /*thumb*/ KC_NO,    KC_NO,    KC_NO,                          /*thumb*/ _______,  _______,  _______,
      /*aux*/  _______,  _______,                                   /*aux*/  _______,  _______
              /* ^^^^ can't be used */                                      /* use these ^^^^ */
  ),

  [L_MOUSE] = LAYOUT_uqs(
     _______,  MS_BTN1,  MS_BTN2,  KC_PGUP,  MS_BTN1,  MS_BTN3,    MS_BTN3,  MS_BTN1,  MS_UP,    MS_WHLU,  MS_BTN2,  _______,
     _______,  KC_LGUI,  KC_LALT,  KC_LSFT,  KC_LCTL,  KC_NO,      MS_WHLL,  MS_LEFT,  MS_DOWN,  MS_RGHT,  MS_WHLR,  _______,
     _______,  KC_NO,    MS_WHLU,  MS_WHLD,  KC_PGDN,  KC_MUTE,    _______,  MS_WHLD,  MS_BTN3,  MS_BTN4,  MS_BTN5,  _______,
                         KC_NO,    KC_NO,                                              KC_NO,    KC_NO,
     /*thumb*/ _______,  _______,  _______,                        /*thumb*/ _______,  _______,  _______,
      /*aux*/  _______,  _______,                                   /*aux*/  _______,  _______
               /* ^^^^ use these */                                               /* ^^^^ can't be used */
  ),
};

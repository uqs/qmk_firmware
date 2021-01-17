/* A standard layout for the Dactyl Manuform 5x6 Keyboard */

#include QMK_KEYBOARD_H

enum layers {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
};

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

// defining Adjust layer
uint32_t layer_state_set_user(uint32_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

#define KC_SHIFT_INS LSFT(KC_INS)
#define KC_ALT_SHIFT_INS LALT(LSFT(KC_INS))
// Works as well as the above, but only needed for more complex stuff.
/*
enum custom_keycodes {
    SHIFT_INS = SAFE_RANGE,
    ALT_SHIFT_INS,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case SHIFT_INS:
        if (record->event.pressed) {
            // when keycode is pressed
            SEND_STRING(SS_LSFT(SS_TAP(X_INS)));
        } else {
            // when keycode is released
        }
        break;

    case ALT_SHIFT_INS:
        if (record->event.pressed) {
            SEND_STRING(SS_LALT(SS_LSFT(SS_TAP(X_INS))));
        } else {
        }
        break;
    }

    return true;
};
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT_5x6(
     KC_ESC , KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                        KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_BSPC,
     KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                        KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_BSLS,
     KC_LCTL, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                        KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
     KC_LSFT, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                        KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,KC_GRV,
                      KC_LBRC, KC_RBRC,                                                     KC_MINS, KC_EQL,
                                      LOWER, KC_SPC,                         KC_ENT, RAISE,
                                      /* Order is TR, BR, TL, BL             Order is BL, TL, BR, TR */
                                      KC_LSFT, KC_NO,                        KC_NO , KC_RSFT,
                                      KC_LALT, KC_LGUI,                      KC_APP, KC_RALT

                                       /*   Shift                          Shift       *
                                        * Alt   Nix                      Nix   Alt     *
                                        *    Win                            Menu       */

  ),

  [_LOWER] = LAYOUT_5x6(
     KC_TILD,KC_EXLM, KC_AT ,KC_HASH,KC_DLR ,KC_PERC,                        KC_CIRC,KC_AMPR,KC_ASTR,KC_LPRN,KC_RPRN,KC_DEL,
     _______,_______,_______,KC_PGUP,_______,KC_LBRC,                        KC_RBRC,_______,_______,_______,KC_ALT_SHIFT_INS,KC_SHIFT_INS,  // 2 macros here
     _______,_______,KC_HOME,KC_PGDN,KC_END ,_______,                        KC_LEFT,KC_DOWN, KC_UP ,KC_RGHT,_______,KC_PGUP,
     _______,_______,_______,_______,_______,KC_LPRN,                        KC_RPRN,_______,_______,_______,_______,KC_PGDN,
                     _______,_______,                                                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______

  ),

  [_RAISE] = LAYOUT_5x6(
     KC_F1  , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 ,                        KC_F7  , KC_F8 , KC_F9 , KC_F10,KC_F11 ,KC_F12 ,
     _______,_______,_______, KC_UP ,_______,_______,                        _______,_______,_______,_______,_______,KC_MUTE,
     _______,_______,KC_LEFT,KC_DOWN,KC_RGHT,_______,                        _______,KC_MPRV,KC_MPLY,KC_MNXT,_______,KC_VOLU,
     _______,_______,_______,_______,_______,_______,                        _______,_______,_______,_______,_______,KC_VOLD,
                     _______,_______,                                                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______
  ),

  [_ADJUST] = LAYOUT_5x6(
     RGB_TOG,_______,_______,_______,_______,_______,                        _______,_______,KC_MS_BTN3,_______,KC_MS_BTN4,KC_MS_BTN5,
     RGB_MOD,RGB_HUI,RGB_SAI,RGB_VAI,RGB_SPI,_______,                        _______,KC_MS_BTN1,KC_MS_UP,KC_MS_BTN2,KC_MS_WH_LEFT,KC_MS_WH_RIGHT,
     RGB_RMOD,RGB_HUD,RGB_SAD,RGB_VAD,RGB_SPD,_______,                       _______,KC_MS_LEFT,KC_MS_DOWN,KC_MS_RIGHT,_______,KC_MS_WH_UP,
     /* Plain, Breath, Rainbow, Swirl, Snake, KnightRider, Xmas, Gradient */
     RGB_M_P,RGB_M_B,RGB_M_R,RGB_M_SW,RGB_M_SN,RGB_M_K,                      KC_ACL0,KC_ACL1,KC_ACL2,_______,_______,KC_MS_WH_DOWN,
                     RGB_M_X,RGB_M_G,                                                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______
  ),
};

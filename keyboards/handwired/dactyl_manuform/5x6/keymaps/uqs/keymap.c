/* A standard layout for the Dactyl Manuform 5x6 Keyboard */

#include QMK_KEYBOARD_H

enum layers {
    L0 = 0,
    L1,
    L2,
    L3,
    L4,  // wasd gaming
};

uint8_t l_prev = L0;
uint8_t rgb_desired = RGBLIGHT_MODE_BREATHING + 3;

// defining layer L3 when both keys are pressed
layer_state_t layer_state_set_user(layer_state_t state) {
#ifdef RGBLIGHT_LAYERS
  rgblight_set_layer_state(0, true);
  rgblight_set_layer_state(1, layer_state_cmp(state, L4));
#else
  uint8_t layer = biton32(state);
  if (l_prev != layer) {
    if (layer == L4) {
      rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      rgblight_setrgb_range(RGB_TURQUOISE, 3, 6);
      rgblight_setrgb_range(RGB_TURQUOISE, 3, 12);
      rgblight_setrgb_range(RGB_TURQUOISE, 6, 12);
      rgblight_setrgb_range(RGB_RED, 0, 3);
      //rgblight_setrgb_at(RGB_TURQUOISE, 11);
      //rgblight_setrgb_at(RGB_TURQUOISE, 12);
    }
    // If we're about to exit L3, don't mess with the RGB again, obviously!
    if (l_prev == L3) {
      rgb_desired = rgblight_get_mode();
    }
    if (l_prev == L4) {
      rgblight_mode(rgb_desired);
    }
    l_prev = layer;
  }
#endif
  state = update_tri_layer_state(state, L1, L2, L3);
  return state;
}

#ifdef RGBLIGHT_LAYERS
const rgblight_segment_t PROGMEM my_default_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    //{0, 6, HSV_SPRINGGREEN},
    {0, 12, HSV_TURQUOISE}
);

const rgblight_segment_t PROGMEM my_wasd_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 6, HSV_RED},
    {6, 6, HSV_OFF}
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_default_layer,
    my_wasd_layer
);
#endif

void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_LAYERS
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
    rgblight_set_layer_state(0, true);
#else
#endif
    rgb_desired = rgblight_get_mode();
}

// Maybe do something with KC_PASTE instead (or KC_CUT, KC_COPY)
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


LEADER_EXTERNS();

void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

    SEQ_ONE_KEY(KC_U) {
      qk_ucis_start();
    }
    SEQ_ONE_KEY(KC_F) {
      // Anything you can do in a macro.
      SEND_STRING("QMK is awesome.");
    }
    SEQ_TWO_KEYS(KC_D, KC_D) {
      SEND_STRING(SS_LCTL("a") SS_LCTL("c"));
    }
    //tableflip (LEADER - TF)
    SEQ_TWO_KEYS(KC_T, KC_F) {
        set_unicode_input_mode(UC_LNX);
        send_unicode_hex_string("0028 30CE 0CA0 75CA 0CA0 0029 30CE 5F61 253B 2501 253B");
    }
    SEQ_THREE_KEYS(KC_D, KC_D, KC_S) {
      SEND_STRING("https://start.duckduckgo.com\n");
    }
  }
}

// 3 codepoints at most, otherwise increase UCIS_MAX_CODE_POINTS
const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE(
    UCIS_SYM("poop", 0x1F4A9),                // 💩
    UCIS_SYM("rofl", 0x1F923),                // 🤣
    UCIS_SYM("cuba", 0x1F1E8, 0x1F1FA),       // 🇨🇺
    UCIS_SYM("look", 0x0CA0, 0x005F, 0x0CA0)  // ಠ_ಠ
//   <lenny> = ( ͡° ͜ʖ ͡°)
//   <shrug> = ¯\_(ツ)_/¯
//   <fliptable> = (╯°□°）╯︵ ┻━┻
//   <unfliptable> = ┬─┬ノ( º _ ºノ)
//   <happy> = ᕕ( ᐛ )ᕗ
//   <disapproval> = ಠ_ಠ
//   <hopeless> = ＼(^o^)／
//   <idklol> = ¯\(°_o)/¯
);

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [L0] = LAYOUT_5x6(
     KC_GESC, KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                        KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_BSPC,
     KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                        KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_BSLS,
     KC_LCTL, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                        KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
     KC_LSFT, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                        KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,RSFT_T(KC_GRV),
                      KC_LBRC, KC_RBRC,                                                     KC_MINS, KC_EQL,
                                      MO(L1), KC_SPC,                        KC_ENT, MO(L2),
                                      /* Order is TR, BR, TL, BL             Order is BL, TL, BR, TR */
                                      KC_LSFT, KC_LEAD,                      KC_LEAD,KC_RSFT,
                                      KC_LALT, KC_LGUI,                      KC_APP, KC_RALT

                                       /*   Shift                          Shift       *
                                        * Alt   Nix                      Nix   Alt     *
                                        *    Win                            Menu       */
  ),

  [L1] = LAYOUT_5x6(
     KC_TILD,KC_EXLM, KC_AT ,KC_HASH,KC_DLR ,KC_PERC,                        KC_CIRC,KC_AMPR,KC_ASTR,KC_LPRN,KC_RPRN,KC_DEL,
     _______,_______,KC_WBAK,KC_PGUP,KC_WFWD,KC_LBRC,                        KC_RBRC,KC_HOME,KC_PGUP,_______,KC_ALT_SHIFT_INS,KC_SHIFT_INS,  // 2 macros here
     _______,_______,KC_HOME,KC_PGDN,KC_END ,_______,                        KC_LEFT,KC_DOWN, KC_UP ,KC_RGHT,_______,KC_INS,
     _______,_______,_______,_______,_______,KC_LPRN,                        KC_RPRN,KC_PGDN, KC_END,_______,_______,_______,
                     _______,KC_PSCR,                                                        KC_SLCK,KC_PAUS,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______

  ),

  [L2] = LAYOUT_5x6(
     KC_F1  , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 ,                        KC_F7  , KC_F8 , KC_F9 , KC_F10,KC_F11 ,KC_F12 ,
     _______,_______,_______, KC_UP ,_______,_______,                        _______,_______,KC_MSEL,_______,_______,KC_MUTE,
     _______,_______,KC_LEFT,KC_DOWN,KC_RGHT,_______,                        _______,KC_MPRV,KC_MPLY,KC_MNXT,_______,KC_VOLU,
     _______,_______,_______,_______,_______,_______,                        _______,_______,KC_MSTP,_______,_______,KC_VOLD,
                     _______,_______,                                                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        TG(L4) ,_______,
                                     _______,_______,                        _______,_______
  ),

  [L3] = LAYOUT_5x6(
     RGB_TOG,_______,_______,_______,_______,_______,                  KC_MS_WH_LEFT,KC_MS_WH_UP,KC_MS_BTN3,KC_MS_WH_DOWN,KC_MS_WH_RIGHT,KC_MS_BTN4,
     RGB_MOD,_______,_______,_______,_______,_______,                        _______,KC_MS_BTN1 ,  KC_MS_UP, KC_MS_BTN2  ,_______, KC_MS_BTN5,
    RGB_RMOD,_______,KC_ACL0,KC_ACL1,KC_ACL2,_______,                        _______,KC_MS_LEFT ,KC_MS_DOWN,KC_MS_RIGHT  ,_______,_______,
     /* Plain, Breath, Rainbow, Swirl, Snake, KnightRider, Xmas, Gradient */
     RGB_M_P,RGB_M_B,RGB_M_R,RGB_M_SW,RGB_M_SN,RGB_M_K,                      _______,_______,_______,_______,_______,_______,
                     RGB_M_X,RGB_M_G,                                                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     RGB_HUI,RGB_SAI,                        RGB_VAI,KC_BRIU,
                                     RGB_HUD,RGB_SAD,                        RGB_VAD,KC_BRID
  ),

  [L4] = LAYOUT_5x6(
     KC_ESC ,  KC_1  , KC_2  , KC_3  , KC_4  , KC_5 ,                        _______,_______,_______,_______,_______,_______,
     KC_TAB , KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R ,                        KC_T   ,KC_Y   ,_______,_______,_______,_______,
     KC_LSFT, KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F ,                        KC_G   ,KC_H   ,_______,_______,_______,_______,
     KC_LCTL, KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V ,                        KC_B   ,KC_N   ,_______,_______,_______,_______,
                      KC_LBRC, KC_RBRC,                                                      _______,_______,
                                      KC_LCTL,KC_SPC,                        KC_ENT, TG(L4),
                                      /* Order is TR, BR, TL, BL             Order is BL, TL, BR, TR */
                                      KC_LSFT, KC_NO,                        KC_NO , KC_RSFT,
                                      KC_LALT, KC_NO,                        KC_APP, KC_RALT

                                       /*   Shift                          Shift       *
                                        * Alt   Nix                      Nix   Alt     *
                                        *    Win                            Menu       */

  ),
};

/* A standard layout for the Dactyl Manuform 5x6 Keyboard */
// vi:et sw=4:

#include QMK_KEYBOARD_H
#include <tmk_core/common/wait.h>

// Note to self: I should really try Colemak DHm, like https://www.reddit.com/r/MechanicalKeyboards/comments/9weri6/nuclear_data_in_colemak_dhm/
// Data on it being pretty good: https://github.com/bclnr/kb-layout-evaluation
// "Hands Down" might be good for minimal split keebs, https://sites.google.com/alanreiser.com/handsdown
// But what about VIM? Use Layers+Cursor instead maybe?
//
// LOG:
// Jan 2020, got Dactyl Manuform
// https://play.typeracer.com/  shows about 75-80wpm (en) or ~400cpm (de) on my classic keeb.
// Never did proper touch typing, basically didn't use ring finger much, mostly index/middle and pinky
// Feb 2020, switching to Colemak DH
// mid Feb, 20wpm/87% on monkeytype.com (no punct, numbers)
// early March, 28wpm/90% on MT (plus punct./numbers); 25wpm on typeracer
// early April, 35wpm/92% on MT; 41wpm on typeracer
// early May, 45wpm/96% on MT; 46wpm on typeracer; my qwerty is deteriorating, I need to look at the keys more and more o_O
// early June, 49wpm/95% on MT (sigh ...); 50wpm on typeracer;

enum layers {
    L_QWER = 0,
    L_WASD,  // wasd gaming
    L_COLM,  // Colemak DHm
    L_EXTD,
    L_NUM,
    L_FUNC,
    L_MOUSE,
    L_LAST, // unused
};

#ifdef VIA_ENABLE
_Static_assert(DYNAMIC_KEYMAP_LAYER_COUNT >= L_LAST, "VIA enabled, but not enough DYNAMIC_KEYMAP_LAYER_COUNT for all layers");
#endif

#ifdef RGBLIGHT_LAYERS
layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(L_QWER, layer_state_cmp(state, L_QWER));
    rgblight_set_layer_state(L_WASD, layer_state_cmp(state, L_WASD));
    rgblight_set_layer_state(L_COLM, layer_state_cmp(state, L_COLM));
    return state;
}
#endif

// NOTE have a look at keyboards/jones/v03/keymaps/default_jp/keymap.c for default layers and put WASD there and NUM?
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
    [L_QWER] = {{0, 12, HSV_WHITE},  RGBLIGHT_END_SEGMENTS},
    [L_WASD] = {{0,  6, HSV_RED}, {6, 6, HSV_OFF}, RGBLIGHT_END_SEGMENTS},
    [L_COLM] = {{0, 12, HSV_GREEN},  RGBLIGHT_END_SEGMENTS},
    [L_EXTD] = {{0, 12, HSV_BLUE},   RGBLIGHT_END_SEGMENTS},
    [L_NUM] =  {{0, 12, HSV_ORANGE}, RGBLIGHT_END_SEGMENTS},
    [L_FUNC] = {{0, 12, HSV_YELLOW}, RGBLIGHT_END_SEGMENTS},
    [L_MOUSE]= {{0, 12, HSV_PURPLE}, RGBLIGHT_END_SEGMENTS},
};

// This array needs pointers, :/
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = {
    [L_QWER] = my_rgb_segments[L_QWER],
    [L_WASD] = my_rgb_segments[L_WASD],
    [L_COLM] = my_rgb_segments[L_COLM],
    [L_EXTD] = my_rgb_segments[L_EXTD],
    [L_NUM]  = my_rgb_segments[L_NUM],
    [L_FUNC] = my_rgb_segments[L_FUNC],
    [L_MOUSE]= my_rgb_segments[L_MOUSE],
};
#endif

void keyboard_post_init_user(void) {
    default_layer_set(1ul << L_COLM);
#ifdef RGBLIGHT_LAYERS
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
    rgblight_set_layer_state(0, true);
#else
    rgb_desired = rgblight_get_mode();
#endif
}

#define KC_ATAB LALT(KC_TAB)
#define KC_SATAB LALT(LSFT(KC_TAB))
#define KC_CTAB LCTL(KC_TAB)
#define KC_SCTAB LCTL(LSFT(KC_TAB))
// Maybe do something with KC_PASTE instead (or KC_CUT, KC_COPY)
#define KC_S_INS LSFT(KC_INS)
#define KC_A_S_INS LALT(LSFT(KC_INS))
// Note RSFT_T() can only work with the basic layer, supposedly custom macros
// should work, like given below, except they don't. RSFT_T(KC_S_INS) results
// in xev seeing Backspace ... A plain KC_S_INS of course works fine.

// Works as well as the above, but only needed for more complex stuff.
enum custom_keycodes {
    SHIFT_INS = SAFE_RANGE,
    ALT_SHIFT_INS,
    INS_HARD,
    ALTGR_QUOT,
    KC_A_AE,
    KC_O_OE,
    KC_U_UE,
    MINS_UNDSCR,
    TM_NEXT,
    TM_PREV,
    WIN_NEXT,
    WIN_PREV,
    WIN_LEFT,
    WIN_RGHT,
    WIN_UP,
    WIN_DN,
    LT_EXTD_ESC,
    LT_EXTD_ESC_WIN,
    LT_NUM_BSPC,
    LT_NUM_BSPC_DEL,
    LT_MOUSE_ALT_SHIFT_INS,
    LT_FUNC_SHIFT_INS,
    OSM_GUI,
    ALT_TAB,
    ALT_STAB,
};

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // TODO: why not use key_timer here? is it dynamic or not?
    static uint16_t extd_layer_timer;
    uint8_t mod_state = get_mods();
    if (layer_state_is(L_EXTD) && record->event.pressed) {
        extd_layer_was_used = true;
        // immediately let go off WIN to have proper and regular use of the layer.
        unregister_mods(MOD_BIT(KC_LWIN));
        // never triggered Win+MSWheel under Linux, but FreeBSD seems to need a
        // bit more time to let go of the modifier.
        wait_ms(10);
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
    case LT_EXTD_ESC_WIN:
        if (record->event.pressed) {
            extd_layer_was_used = false;
            extd_layer_timer = timer_read();
            layer_on(L_EXTD);
            register_mods(MOD_BIT(KC_LWIN));
            // QMK is fecking stupid. All I want is to send a custom keycode
            // that I bind to Hyper via xmodmap, but this doesn't seem possible
            // according to some reddit search. Maybe I should use F13..F24
            // instead? Anyway, I'll be using Control_R and rebind that to
            // Hyper_L via xmodmap. Why? So that I can have this magic key
            // trigger Hyper_L when pressed, but unpress it in case I hit any
            // other key on that layer. This can then be used in combination
            // with the mouse to move/resize windows. I would use the Win key
            // for this, but then under Windows that opens the Startmenu, so I
            // need a modifier that is unknown to windows. A "silent" RCTL tap
            // under Windows should be benign, I think.
            // .xmodmaprc:
            //   remove mod4 = Hyper_L
            //   remove control = Hyper_L
            //   keycode 105 = Hyper_L
            //   add mod3 = Hyper_L
            //
            // While this works once, re-plugging the keyboard makes the apps
            // no longer see Hyper but Control again. Feck. I probably need to
            // come up with some setxkb magic to have this mapping be more
            // permanent. Or switch to WIN directly, even though that will
            // mess up Windows pretty bad.

        } else {
            layer_off(L_EXTD);
            unregister_mods(MOD_BIT(KC_LALT));
            unregister_mods(MOD_BIT(KC_LWIN));
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
    case LT_NUM_BSPC_DEL:
        if (record->event.pressed){
            num_layer_was_used = false;
            extd_layer_timer = timer_read();
            layer_on(L_NUM);
        } else {
            layer_off(L_NUM);
            // BUG: this will fire a backspace, even if I pressed something on the num layer, in which case of course that was intended and not sending backspace or delete. Need to check how layer tap is implemented.
            if (timer_elapsed(extd_layer_timer) < TAPPING_TERM && !num_layer_was_used) {
                if (mod_state & MOD_MASK_SHIFT) {
                    uint8_t which_shift = mod_state & MOD_MASK_SHIFT;
                    if (!(mod_state & MOD_BIT(KC_LSHIFT)) != !(mod_state & MOD_BIT(KC_RSHIFT))) {
                        unregister_mods(MOD_MASK_SHIFT);
                        tap_code(KC_DEL);
                        register_mods(which_shift);
                    } else {
                        tap_code(KC_DEL);
                    }
                    //return false;  // stop processing
                } else {
                    tap_code(KC_BSPC);
                }
            }
        }
        return true;
    case LT_MOUSE_ALT_SHIFT_INS:
        if (record->event.pressed) {
            key_timer = timer_read();
            layer_on(L_MOUSE);
        } else {
            layer_off(L_MOUSE);
            if (timer_elapsed(key_timer) < TAPPING_TERM) {
                tap_code16(LALT(LSFT(KC_INS)));
            }
        }
        return true;
    case LT_FUNC_SHIFT_INS:
        if (record->event.pressed) {
            key_timer = timer_read();
            layer_on(L_FUNC);
        } else {
            layer_off(L_FUNC);
            if (timer_elapsed(key_timer) < TAPPING_TERM) {
                tap_code16(LSFT(KC_INS));
            }
        }
        return true;
#if 1
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
                set_oneshot_mods(MOD_BIT(KC_LGUI));
            } else {
                clear_oneshot_mods();
            }
        }
        return true;
#endif
#if 0
        // From https://github.com/precondition/dactyl-manuform-keymap/blob/main/keymap.c
    case KC_BSPC:
        if (record->event.pressed) {
            if (mod_state & MOD_MASK_SHIFT) {
                // In case only one shift is held
                // see https://stackoverflow.com/questions/1596668/logical-xor-operator-in-c
                // This also means that in case of holding both shifts and pressing KC_BSPC,
                // Shift+Delete is sent (useful in Firefox) since the shift modifiers aren't deleted.
                if (!(mod_state & MOD_BIT(KC_LSHIFT)) != !(mod_state & MOD_BIT(KC_RSHIFT))) {
                    del_mods(MOD_MASK_SHIFT);
                }
                register_code(KC_DEL);
                delkey_registered = true;
                set_mods(mod_state);
                return false;
            }
        } else {
            if (delkey_registered) {
                unregister_code(KC_DEL);
                delkey_registered = false;
                return false;
            }
        }
        return true;  // fall through to regular handling
        break;
#endif
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
    case ALT_TAB:
        if (record->event.pressed) {
            register_mods(MOD_BIT(KC_LALT));
            tap_code16(KC_TAB);
        }
        break;
    case ALT_STAB:
        if (record->event.pressed) {
            register_mods(MOD_BIT(KC_LALT));
            tap_code16(LSFT(KC_TAB));
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
    case ALTGR_QUOT:
        // Shortcut for composing Umlauts
        if (record->event.pressed) {
            tap_code16(KC_RALT);
            tap_code16(LSFT(KC_QUOT));
        }
        break;
    case TM_NEXT:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("a") "n");
        break;
    case TM_PREV:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("a") "p");
        break;
    case WIN_NEXT:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") "w");
        break;
    case WIN_PREV:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") "W");
        break;
    case WIN_LEFT:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") SS_TAP(X_H));
        break;
    case WIN_RGHT:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") SS_TAP(X_L));
        break;
    case WIN_UP:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") SS_TAP(X_K));
        break;
    case WIN_DN:
        if (record->event.pressed) SEND_STRING(SS_LCTRL("w") SS_TAP(X_J));
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
#endif
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
  }
}
#else
#define KC_LEAD KC_NO
#endif

#ifdef UCIS_ENABLE
// 3 codepoints at most, otherwise increase UCIS_MAX_CODE_POINTS
const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE(
    UCIS_SYM("poop", 0x1F4A9),                // 💩
    UCIS_SYM("rofl", 0x1F923),                // 🤣
    UCIS_SYM("cuba", 0x1F1E8, 0x1F1FA),       // 🇨🇺
    UCIS_SYM("look", 0x0CA0, 0x005F, 0x0CA0)  // ಠ_ಠ
);
#endif

// Shorter names
#define MS_WHDN KC_MS_WH_DOWN
#define MS_WHUP KC_MS_WH_UP
#define MS_WHLEFT KC_MS_WH_LEFT
#define MS_WHRGHT KC_MS_WH_RIGHT

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_QWER] = LAYOUT_5x6(
     KC_GESC, KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                                      KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_BSPC,
     KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                                      KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_BSLS,
     KC_LCTL, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                                      KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
     KC_LSFT, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                                      KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,RSFT_T(KC_GRV),
                      KC_LBRC, KC_RBRC,                                                                   KC_MINS, KC_EQL,
                                 LT_EXTD_ESC, KC_SPC,                                      KC_ENT, LT_NUM_BSPC,
                                  /* Order is TR, BR, TL, BL                           Order is BL, TL, BR, TR */
                       LT_MOUSE_ALT_SHIFT_INS, KC_LEAD,                                    KC_LEAD, LT_FUNC_SHIFT_INS,
                                      OSM_GUI, KC_LALT,                                    KC_RALT, KC_APP
  ),

  [L_WASD] = LAYOUT_5x6(
     KC_ESC ,  KC_1  , KC_2  , KC_3  , KC_4  , KC_5 ,                                      KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_BSPC,
     KC_T   , KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R ,                                      KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_BSLS,
     KC_G   , KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F ,                                      KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
     KC_B   , KC_GRV , KC_Z  , KC_X  , KC_C  , KC_V ,                                      KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,RSFT_T(KC_GRV),
                      KC_LBRC, KC_RBRC,                                                                   KC_MINS, KC_EQL,
                                 KC_LCTL, KC_SPC,                                              KC_ENT, KC_BSPC,
                                  /* Order is TR, BR, TL, BL                           Order is BL, TL, BR, TR */
                                      KC_R, A(KC_M),                                       KC_F5,DF(L_COLM),
                                      KC_LGUI, KC_LALT,                                    KC_RALT, KC_APP

  ),

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

  // TODO: implement CAPS_WORD on the left pinky shift on tap (maybe?) But it
  // can't actually set caps lock, as I'm rebinding that for a saner laptop
  // keyboard. See drashna's keymap.
  [L_COLM] = LAYOUT_5x6(
     _______, KC_NO , KC_NO , KC_NO , KC_NO , KC_NO ,                                          KC_NO , KC_NO , KC_NO , KC_NO , KC_NO , KC_NO ,
     KC_TAB , KC_Q  , KC_W  , KC_F  , KC_P  , KC_B  ,                                          KC_J  , KC_L  ,KC_U_UE, KC_Y  ,KC_QUOT,KC_BSLS,
     OSM_GUI,KC_A_AE, KC_A_R, KC_S_S, KC_C_T, KC_G  ,                                          KC_M  , KC_C_N, KC_S_E, KC_A_I,KC_O_OE,KC_MINUS,
     KC_LSFT, KC_Z  , KC_X  , KC_C  , KC_D  , KC_V  ,                                          KC_K  , KC_H  ,KC_COMM,KC_DOT ,KC_SLSH,RSFT_T(KC_GRV),
                     KC_LBRC,KC_RBRC,                                                                   ALTGR_QUOT, SHIFT_INS,
              /* These two ^^^^  are here for Gmail hotkeys only  */
                            LT_EXTD_ESC, SFT_T(KC_SPC),                                        KC_ENT, LT_NUM_BSPC,
                                  /* Order is TR, BR                                       Order is BL, TL,
                                              TL, BL                                                BR, TR */
                       LT_MOUSE_ALT_SHIFT_INS, KC_LEAD,                                        KC_LEAD, LT_FUNC_SHIFT_INS,
                                      OSM_GUI, KC_LALT,                                        KC_RALT, KC_APP
// NOTE: RSFT_T(KC_S_INS) doesn't work, only INS comes through. RSFT_T stuff
// only works on "simple" keycodes. See process_record_user for how this works,
// thanks to ridingqwerty on Discord.
// KC_PASTE essentially does shift-insert for Linux (mostly, for example not in
// Qt apps! Also Chrome makes it copy from Clipboard, not Selection, like XTerm
// does. Wargh), does nothing on Windows though, where Shift-Ins works.
//
// Some people do: NavL | Bspc/Shft | Enter/Fkeys | | Esc | Spc/SymbL | empty
// but sometimes you have to hold enter or space? or what about shift-enter? hmmm
// Needs more thinking, there's 3 useful/quick thumb keys after all, would have
// to move shift-ins stuff someplace else.
// More thoughts: "I have Shift-Enter on the left big thumb, and Shift-Space on
// the right. To the left of the left big thumb is LT1-Del and to the right of
// the right big thumb is LT1-Bspc. My other two outermost thumb keys are
// Win/Sys on the left and Alt on the right. My LT2 is actually on a pinky,
// along with CTRL. I have thought about moving CTRL to one of the big thumbs
// but I like having shift on both."


/*
 * My take on thumb keys:
 * tappable: tab, ent, del, bkspc, esc, shift-ins, space, AltGr, Leader
 * holdable: shift, L1, L2 (also, alt, ctrl, win)
 * sometimes holdable: ent, del, bkspc
 *
 * right side: ent, bkpsc, del, shift-ins, altgr
 * left side:  space, esc, tab, ctrl/alt/win
 *
 * Here's what I'll likely do, esp with a Kyria:
 *        L1/Esc  Space  Win  ||  AltGr     Enter  L2/Bksp
 *   L1:                      || Shift-Ins            Bksp
 *   L2: no change, need Space/Tab during L2
 * with home row mods.
 * Win on home mod is annoying as I use it for dragging windows and I need to
 * hold-and-delay for several hundreds ms before it registers :/
 *
 * Update: I moved Win off the home row, to get ä/ö on them with long presses.
 * TODO: Produce a proper Hyper key (as in xmodmap, not QMK!) and use window
 * movement in Openbox with that.
 */
  ),

// OSMs
#define OSM_ALT OSM(MOD_LALT)
#define OSM_SFT OSM(MOD_LSFT)
#define OSM_CTL OSM(MOD_LCTL)

  // Updated with inspiration from https://forum.colemak.com/topic/2014-extend-extra-extreme/
  // I like the AltGr trick from https://stevep99.github.io/seniply/ and should probably incorporate some stuff from it.
  [L_EXTD] = LAYOUT_5x6(
     KC_F1  , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 ,                                     KC_F7  , KC_F8 , KC_F9 , KC_F10,KC_F11 ,KC_F12,
   _______,WIN_PREV,TM_PREV,KC_PGUP,TM_NEXT,WIN_NEXT,                                     KC_HOME,KC_PGDN,KC_PGUP,KC_END ,KC_INS ,INS_HARD,
     _______,OSM_GUI,OSM_ALT,OSM_SFT,OSM_CTL,KC_RALT,                                     KC_LEFT,KC_DOWN, KC_UP, KC_RGHT,KC_DEL ,KC_BSPC ,
     _______,ALT_TAB,KC_SCTAB,KC_CTAB,KC_PGDN, KC_NO,                                   WIN_LEFT,WIN_DN,WIN_UP,WIN_RGHT,KC_PSTE,KC_ENTER,  // KC_PSTE works in XTerm to emulate middle-click
                     MS_WHUP,MS_WHDN,                                                                     MS_WHLEFT,MS_WHRGHT,
                                     _______,_______,                                     _______,KC_BSPC,
                                     _______,_______,                                     _______,_______,
                                     _______,_______,                                     _______,_______
                                  /* ^^^^ can't be used */                             /* use these ^^^^ */

  ),

  // Num/Symbol. This works somehow without turning on Numlock first. I've not
  // managed to get Alt-codes working under Windows though, might be an
  // artefact of using US Intl (nope, I'm using USCmpse custom layout)?
  // TODO: maybe swap # with ;, that way I can roll :w or :wq which I need often ...
  [L_NUM] = LAYOUT_5x6(
     _______, KC_NO , KC_NO , KC_NO , KC_NO , KC_NO ,                                     KC_NUMLOCK,KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,
     _______,KC_EXLM, KC_AT ,KC_HASH,KC_DLR, KC_PERC,                                     KC_KP_EQUAL, KC_KP_7,KC_KP_8,KC_KP_9,KC_KP_PLUS,_______,
     _______,KC_SCLN,KC_COLN,KC_LCBR,KC_LPRN,KC_LBRC,                                     KC_KP_ASTERISK,KC_KP_4,KC_KP_5,KC_KP_6,MINS_UNDSCR,_______,
     _______,KC_CIRC,KC_AMPR,KC_RCBR,KC_RPRN,KC_RBRC,                                     KC_COMM,KC_KP_1,KC_KP_2,KC_KP_3,KC_KP_SLASH,KC_KP_ENTER,  // Enter here, because thumb is occupied
                     KC_GRV,KC_TILDE,                                                                        KC_KP_0,KC_KP_DOT,
                                     KC_ESC ,KC_SPC ,                                        _______,_______,
                                     KC_KP_0,_______,                                        _______,_______,
                                     _______,_______,                                        _______,_______
                                  /* ^^^^ use these */                                      /* ^^^^ can't be used */
  ),

  [L_FUNC] = LAYOUT_5x6(
     _______, KC_NO , KC_NO , KC_NO , KC_NO , KC_NO ,                                      KC_NO , KC_NO , KC_NO , KC_NO , KC_NO , KC_NO ,
     _______, KC_NO ,KC_MUTE,KC_VOLD,KC_VOLU, KC_NO ,                                     KC_PSCR, KC_F7 , KC_F8 , KC_F9 , KC_F10,DF(L_WASD),
     _______,KC_LGUI,KC_LALT,KC_LSFT,KC_LCTL, KC_NO ,                                     KC_SLCK, KC_F4 , KC_F5 , KC_F6 , KC_F11,DF(L_QWER),
     _______, KC_NO , KC_NO , KC_NO , KC_NO , KC_NO ,                                     KC_PAUS, KC_F1 , KC_F2 , KC_F3 , KC_F12,DF(L_COLM),
                      KC_NO , KC_NO ,                                                                         KC_NO , KC_NO ,
                                     _______,_______,                                        _______,_______,
                                     _______,_______,                                        _______,_______,
                                     _______,_______,                                        _______,_______
  ),

  [L_MOUSE] = LAYOUT_5x6(
     RGB_TOG,_______,_______,_______,_______,_______,                                       _______,_______,_______,_______,_______,_______,
     RGB_MOD,RGB_HUI,KC_MUTE,KC_VOLD,KC_VOLU,RGB_SAI,                                       _______,_______ ,KC_MS_UP, MS_WHUP, _______,_______,
    RGB_RMOD,RGB_HUD,KC_ACL0,KC_ACL1,KC_ACL2,RGB_SAD,                                       MS_WHLEFT,KC_MS_LEFT,KC_MS_DOWN,KC_MS_RIGHT,MS_WHRGHT,_______,
     /* Plain, Breath, Rainbow, Swirl, Snake, KnightRider, Xmas, Gradient */
     RGB_M_P,RGB_M_B,RGB_M_R,RGB_M_SW,RGB_M_SN,RGB_M_K,                                     _______,MS_WHDN,KC_MS_BTN3,KC_MS_BTN4,KC_MS_BTN5,_______,
                     RGB_M_X,RGB_M_G,                                                                       _______,_______,
                                     _______,_______,                                       KC_MS_BTN1,KC_MS_BTN2,
                                     _______,_______,                                       RGB_VAI,KC_BRIU,
                                     _______,_______,                                       RGB_VAD,KC_BRID
  ),
};

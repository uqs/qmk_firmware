// vi:et sw=4:

#include QMK_KEYBOARD_H
#include <tmk_core/common/wait.h>
#include "uqs.h"

// Note to self: I should really try Colemak DHm, like https://www.reddit.com/r/MechanicalKeyboards/comments/9weri6/nuclear_data_in_colemak_dhm/
// Data on it being pretty good: https://github.com/bclnr/kb-layout-evaluation
// "Hands Down" might be good for minimal split keebs, https://sites.google.com/alanreiser.com/handsdown
// But what about VIM? Use Layers+Cursor instead maybe?
//
// LOG:
// late Jan 2020, got Dactyl Manuform 5x6
// https://play.typeracer.com/  shows about 75-80wpm (en) or ~400cpm (de) on my classic keeb.
// Never did proper touch typing, basically didn't use ring finger much, mostly index/middle and pinky (only to hold down modifiers, really).
// Feb 2020, switching to Colemak DH after 30 years of Qwerty, uh oh...
// mid Feb, 20wpm/87% on monkeytype.com (no punct, numbers)
// early March, 28wpm/90% on MT (plus punct./numbers); 25wpm on typeracer
// early April, 35wpm/92% on MT; 41wpm on typeracer
// early May, 45wpm/96% on MT; 46wpm on typeracer; my qwerty is deteriorating, I need to look at the keys more and more o_O
// early June, 49wpm/95% on MT (sigh ...); 50wpm on typeracer;
// early July, 50wpm/96% on MT (...); 52wpm/96% on typeracer;


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
    [L_QWER] = {{0, RGBLED_NUM, HSV_WHITE},  RGBLIGHT_END_SEGMENTS},
    [L_WASD] = {{0,  RGBLED_NUM/2, HSV_RED}, {RGBLED_NUM/2, RGBLED_NUM/2, HSV_OFF}, RGBLIGHT_END_SEGMENTS},
    [L_COLM] = {{0, RGBLED_NUM, HSV_GREEN},  RGBLIGHT_END_SEGMENTS},
    [L_EXTD] = {{0, RGBLED_NUM, HSV_BLUE},   RGBLIGHT_END_SEGMENTS},
    [L_NUM] =  {{0, RGBLED_NUM, HSV_ORANGE}, RGBLIGHT_END_SEGMENTS},
    [L_FUNC] = {{0, RGBLED_NUM, HSV_YELLOW}, RGBLIGHT_END_SEGMENTS},
    [L_MOUSE]= {{0, RGBLED_NUM, HSV_PURPLE}, RGBLIGHT_END_SEGMENTS},
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
#if 0
        // immediately let go of WIN to have proper and regular use of the layer.
        unregister_mods(MOD_BIT(KC_LWIN));
        // never triggered Win+MSWheel under Linux, but FreeBSD seems to need a
        // bit more time to let go of the modifier.
        wait_ms(10);
#endif
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
        /* Looks like PERMISSIVE_HOLD on LT and OSM doesn't work properly. This
         * is probaby https://github.com/qmk/qmk_firmware/issues/8971
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
        // Feck, this breaks holding down CTRL in this layer to do CTRL+Arrow movements.
        //if (record->event.pressed) {
        //    add_oneshot_mods(MOD_BIT(KC_LCTL));
        //} else {
        //}
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

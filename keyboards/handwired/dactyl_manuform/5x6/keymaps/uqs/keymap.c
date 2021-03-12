/* A standard layout for the Dactyl Manuform 5x6 Keyboard */

#include QMK_KEYBOARD_H

// Note to self: I should really try Colemak DHm, like https://www.reddit.com/r/MechanicalKeyboards/comments/9weri6/nuclear_data_in_colemak_dhm/
// Data on it being pretty good: https://github.com/bclnr/kb-layout-evaluation
// "Hands Down" might be good for minimal split keebs, https://sites.google.com/alanreiser.com/handsdown
// But what about VIM? Use Layers+Cursor instead maybe?
//
// LOG:
// Jan 2020, got Dactyl Manuform
// https://play.typeracer.com/  shows about 75-80wpm (en) or ~400cpm (de) on my classic keeb.
// Feb 2020, switching to Colemak DH
// mid Feb, 20wpm/87% on monkeytype.com (no punct, numbers)
// early March, 28wpm/90% on MT (plus punct./numbers); 25wpm on typeracer

enum layers {
    L_COLM = 0,  // Colemak DHm
    L_QWER,
    L_WASD,  // wasd gaming
    L_EXTD,
    L_NUM,
    L_MOUSE,
    L_LAST, // unused
};

#ifdef VIA_ENABLE
_Static_assert(DYNAMIC_KEYMAP_LAYER_COUNT >= L_LAST, "VIA enabled, but not enough DYNAMIC_KEYMAP_LAYER_COUNT for all layers");
#endif

#ifdef RGBLIGHT_LAYERS
layer_state_t default_layer_state_set_user(layer_state_t state) {
  rgblight_set_layer_state(L_COLM, layer_state_cmp(state, L_COLM));
  rgblight_set_layer_state(L_QWER, layer_state_cmp(state, L_QWER));
  rgblight_set_layer_state(L_WASD, layer_state_cmp(state, L_WASD));
  return state;
}
#endif

// NOTE have a look at keyboards/jones/v03/keymaps/default_jp/keymap.c for default layers and put WASD there and NUM?
layer_state_t layer_state_set_user(layer_state_t state) {
  // defining layer L_MOUSE when both keys are pressed
  state = update_tri_layer_state(state, L_EXTD, L_NUM, L_MOUSE);
#ifdef RGBLIGHT_LAYERS
  rgblight_set_layer_state(L_EXTD, layer_state_cmp(state, L_EXTD));
  rgblight_set_layer_state(L_NUM, layer_state_cmp(state, L_NUM));
  rgblight_set_layer_state(L_MOUSE, layer_state_cmp(state, L_MOUSE));
#else
#endif
  return state;
}

#ifdef RGBLIGHT_LAYERS
// NOTE: at most 2 elements, last one needs to be RGBLIGHT_END_SEGMENTS
typedef rgblight_segment_t rgblight_layer_t[3];

const rgblight_layer_t PROGMEM my_rgb_segments[] = {
  [L_COLM] = {{0, 12, HSV_GREEN},  RGBLIGHT_END_SEGMENTS},
  [L_QWER] = {{0, 12, HSV_WHITE},  RGBLIGHT_END_SEGMENTS},
  [L_WASD] = {{0,  6, HSV_RED}, {6, 6, HSV_OFF}, RGBLIGHT_END_SEGMENTS},
  [L_EXTD] = {{0, 12, HSV_BLUE},   RGBLIGHT_END_SEGMENTS},
  [L_NUM] =  {{0, 12, HSV_ORANGE}, RGBLIGHT_END_SEGMENTS},
  [L_MOUSE]= {{0, 12, HSV_PURPLE}, RGBLIGHT_END_SEGMENTS},
};

// This array needs pointers, :/
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = {
    [L_COLM] = my_rgb_segments[L_COLM],
    [L_QWER] = my_rgb_segments[L_QWER],
    [L_WASD] = my_rgb_segments[L_WASD],
    [L_EXTD] = my_rgb_segments[L_EXTD],
    [L_NUM]  = my_rgb_segments[L_NUM],
    [L_MOUSE]= my_rgb_segments[L_MOUSE],
};
#endif

void keyboard_post_init_user(void) {
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
    ALTGR_QUOT,
    TM_NEXT,
    TM_PREV,
    WIN_NEXT,
    WIN_PREV,
    WIN_LEFT,
    WIN_RGHT,
    WIN_UP,
    WIN_DN,
    LT_EXTD_ESC,
    ALT_TAB,
    ALT_STAB,
};

uint16_t key_timer;
bool delkey_registered = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // TODO: why not use key_timer here? is it dynamic or not?
    static uint16_t ext_layer_timer;
    uint8_t mod_state = get_mods();
    switch (keycode) {
        // From https://github.com/qmk/qmk_firmware/issues/6053
    case LT_EXTD_ESC:
        if (record->event.pressed){
            ext_layer_timer = timer_read();
            layer_on(L_EXTD);
        } else {
            layer_off(L_EXTD);
            unregister_mods(MOD_BIT(KC_LALT));
            if (timer_elapsed(ext_layer_timer) < TAPPING_TERM) {
                tap_code(KC_ESC);
            }
        }
        break;
        // From https://github.com/precondition/dactyl-manuform-keymap/blob/main/keymap.c
        // TODO: doesn't work with my LT(L_NUM, KC_BSPC) tap action, need to do the LT_EXTD_ESC treatment.
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
};

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
#define KC_A_I LALT_T(KC_I)
#define KC_G_O RGUI_T(KC_O)

  [L_COLM] = LAYOUT_5x6(
     KC_GESC, KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                        KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_BSPC,
     KC_TAB , KC_Q  , KC_W  , KC_F  , KC_P  , KC_B  ,                        KC_J  , KC_L  , KC_U  , KC_Y  ,KC_SCLN,KC_BSLS,
     KC_LCTL, KC_G_A, KC_A_R, KC_S_S, KC_C_T, KC_G  ,                        KC_M  , KC_C_N, KC_S_E, KC_A_I, KC_G_O,KC_QUOT,
     KC_LSFT, KC_Z  , KC_X  , KC_C  , KC_D  , KC_V  ,                        KC_K  , KC_H  ,KC_COMM,KC_DOT ,KC_SLSH,RSFT_T(KC_GRV),
                      KC_LBRC, KC_RBRC,                                                     KC_MINS, KC_EQL,
                          LT_EXTD_ESC, KC_SPC,                        KC_ENT, LT(L_NUM, KC_BSPC),
                                  /* Order is TR, BR                     Order is BL, TL,
                                              TL, BL                              BR, TR */
                                ALT_SHIFT_INS, KC_LEAD,                      KC_LEAD, SHIFT_INS,
                                      KC_LGUI, KC_LALT,                      KC_RALT, KC_APP
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
 *
 * right side: ent, bkpsc, del, shift-ins, altgr
 * left side:  space, esc, tab, ctrl/alt/win
 *
 * Here's what I'll likely do, esp with a Kyria:
 *   L1/Esc  Space  Tab  ||  AltGr  Enter  L2/Bksp
 *   L1:                 || Shift-Ins
 * with home row mods.
 * Win on home mod is annoying as I use it for dragging windows and I need to
 * hold-and-delay for several hundreds ms before it registers :/
 */
  ),
  [L_QWER] = LAYOUT_5x6(
     KC_GESC, KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                        KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_BSPC,
     KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                        KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_BSLS,
     KC_LCTL, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                        KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
     KC_LSFT, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                        KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,RSFT_T(KC_GRV),
                      KC_LBRC, KC_RBRC,                                                     KC_MINS, KC_EQL,
                          LT(L_EXTD, KC_ESC), KC_SPC,                        KC_ENT, LT(L_NUM, KC_BSPC),
                                  /* Order is TR, BR, TL, BL             Order is BL, TL, BR, TR */
                                ALT_SHIFT_INS, KC_LEAD,                      KC_LEAD, SHIFT_INS,
                                      KC_LGUI, KC_LALT,                      KC_RALT, KC_APP
  /* Should also consider tapping on some of them, e.g. in keyboards/handwired/dactyl_manuform/6x6/keymaps/happysalada/keymap.c
                     LT(_RIGHT_UP,KC_BSPC),LSFT_T(KC_ESC),         LT(_LEFT, KC_ENT),LT(_LEFT_UP,KC_SPC),
                                   _______,LCTL_T(KC_DEL),         LGUI_T(KC_TAB),_______,
                                          _______,_______,         _______,_______
  */
  ),

  [L_WASD] = LAYOUT_5x6(
     KC_ESC ,  KC_1  , KC_2  , KC_3  , KC_4  , KC_5 ,                        KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_BSPC,
     KC_T   , KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R ,                        KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_BSLS,
     KC_G   , KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F ,                        KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
     KC_B   , KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V ,                        KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,RSFT_T(KC_GRV),
                      KC_LBRC, KC_RBRC,                                                     KC_MINS, KC_EQL,
                                  KC_LCTL,KC_SPC,                                _______,_______,
                                  /* Order is TR, BR, TL, BL             Order is BL, TL, BR, TR */
                                      KC_LSFT, KC_NO,                        KC_LGUI,DF(L_COLM),
                                      KC_LGUI, KC_LALT,                      KC_RALT, KC_APP

  ),

  // Updated with inspiration from https://forum.colemak.com/topic/2014-extend-extra-extreme/
  // TODO: move DEL to thumb for everything? same for INS maybe?
  [L_EXTD] = LAYOUT_5x6(
     KC_F1  , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 ,                        KC_F7  , KC_F8 , KC_F9 , KC_F10,KC_F11 ,KC_F12 ,
   _______,WIN_PREV,TM_PREV,WIN_NEXT,TM_NEXT,KC_PGUP,                        KC_PGUP,KC_HOME, KC_UP ,KC_END ,KC_INS ,KC_BSPC,
     _______,KC_LGUI,KC_LALT,KC_LSFT,KC_LCTL,KC_PGDN,                        KC_PGDN,KC_LEFT,KC_DOWN,KC_RGHT,KC_DEL ,ALTGR_QUOT ,
     //_______,KC_UNDO,KC_CUT ,KC_COPY,KC_PSTE, KC_NO ,                        KC_NO  ,KC_SATAB,KC_ATAB ,KC_SCTAB,KC_CTAB,_______,
     _______,KC_NO,KC_SCTAB,KC_CTAB,ALT_TAB,ALT_STAB,                        KC_NO  ,KC_NO,KC_NO ,KC_NO,KC_NO,_______,
                     MS_WHUP,MS_WHDN,                                                        MS_WHLEFT,MS_WHRGHT,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,KC_PSTE,  // works in XTerm to emulate middle-click
                                     _______,_______,                        _______,_______

  ),

  // Numpad. This only works when NumLock is turned on. TODO: turn it on when
  // entering layer? Or switch to KC_1, etc instead of KC_KP_1 ... This then
  // looses the Alt-number unicode stuff.
  [L_NUM] = LAYOUT_5x6(
     _______,_______,_______,_______,_______,TG(L_NUM),                   KC_NUMLOCK,KC_KP_SLASH,KC_KP_ASTERISK,KC_EQL,KC_NO,KC_BSPC,
     _______,_______,TM_PREV,WIN_UP,TM_NEXT ,KC_VOLU,                        KC_LPRN, KC_KP_7,KC_KP_8,KC_KP_9,KC_KP_MINUS,_______,
     _______,KC_MSEL,WIN_LEFT,WIN_DN,WIN_RGHT,KC_VOLD,                        KC_RPRN, KC_KP_4,KC_KP_5,KC_KP_6,KC_KP_PLUS,_______,
     _______,KC_MPRV,KC_MPLY,KC_MNXT,_______,KC_MUTE,                        KC_COMM, KC_KP_1,KC_KP_2,KC_KP_3,KC_KP_ENTER,_______,
                     KC_PSCR,KC_PAUS,                                                        KC_KP_0,KC_KP_DOT,
                                     _______,KC_SPC ,                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     _______,_______,                        _______,_______
  ),

  [L_MOUSE] = LAYOUT_5x6(
     RGB_TOG,DF(L_COLM),DF(L_QWER),DF(L_WASD),_______,TG(L_NUM),           MS_WHLEFT,MS_WHUP,   KC_MS_BTN3,    MS_WHDN,MS_WHRGHT,KC_MS_BTN4,
     RGB_MOD,_______,_______,_______,_______,_______,                        _______,KC_MS_BTN1,KC_MS_UP,   KC_MS_BTN2,  _______,KC_MS_BTN5,
    RGB_RMOD,_______,KC_ACL0,KC_ACL1,KC_ACL2,_______,                        _______,KC_MS_LEFT,KC_MS_DOWN,KC_MS_RIGHT,  _______,_______,
     /* Plain, Breath, Rainbow, Swirl, Snake, KnightRider, Xmas, Gradient */
     RGB_M_P,RGB_M_B,RGB_M_R,RGB_M_SW,RGB_M_SN,RGB_M_K,                      _______,_______,_______,_______,_______,_______,
                     RGB_M_X,RGB_M_G,                                                        _______,_______,
                                     _______,_______,                        _______,_______,
                                     RGB_HUI,RGB_SAI,                        RGB_VAI,KC_BRIU,
                                     RGB_HUD,RGB_SAD,                        RGB_VAD,KC_BRID
  ),
};

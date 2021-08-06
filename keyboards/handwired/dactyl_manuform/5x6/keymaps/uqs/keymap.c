/* A standard layout for the Dactyl Manuform 5x6 Keyboard */
// vi:et sw=4:

#include "uqs.h"

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

  // TODO: implement CAPS_WORD on the left pinky shift on tap (maybe?) But it
  // can't actually set caps lock, as I'm rebinding that for a saner laptop
  // keyboard. See drashna's keymap.
  [L_COLM] = LAYOUT_5x6(
     KC_NO , KC_NO , KC_NO , KC_NO , KC_NO , KC_NO ,                                          KC_NO , KC_NO , KC_NO , KC_NO , KC_NO , KC_NO,
     KC_NO , KC_Q  , KC_W  , KC_F  , KC_P  , KC_B  ,                                          KC_J  , KC_L  ,KC_U_UE, KC_Y  ,KC_QUOT, KC_NO,
     KC_NO ,KC_A_AE, KC_A_R, KC_S_S, KC_C_T, KC_G  ,                                          KC_M  , KC_C_N, KC_S_E, KC_A_I,KC_O_OE, KC_NO,
     KC_NO , KC_Z  , KC_X  , KC_C  , KC_D  , KC_V  ,                                          KC_K  , KC_H  ,KC_COMM,KC_DOT ,KC_SLSH, KC_NO,
                     KC_GRV,KC_RBRC,                                                                   KC_MINUS,KC_BSLS,
                    /* This ] ^^^^  is here for Gmail hotkeys only  */
                            LT_EXTD_ESC, SFT_T(KC_SPC),                                        KC_ENT, LT_NUM_BSPC,
                                  /* Order is TR, BR                                       Order is BL, TL,
                                              TL, BL                                                BR, TR */
                          LT(L_MOUSE, KC_TAB), KC_LEAD,                                        KC_LEAD, LT_FUNC_SHIFT_INS,
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

  // Updated with inspiration from https://forum.colemak.com/topic/2014-extend-extra-extreme/
  // I like the AltGr trick from https://stevep99.github.io/seniply/ and should probably incorporate some stuff from it.
  [L_EXTD] = LAYOUT_5x6(
     KC_F1  , KC_F2 , KC_F3 , KC_F4 , KC_F5 , KC_F6 ,                                     KC_F7  , KC_F8 , KC_F9 , KC_F10,KC_F11 ,KC_F12,
   _______,WIN_PREV,TM_PREV,KC_PGUP,TM_NEXT,WIN_NEXT,                                     KC_HOME,KC_PGDN,KC_PGUP,KC_END ,KC_INS ,_______,
     _______,OSM_GUI,OSM_ALT,OSM_SFT,OSM_CTL,KC_RALT,                                     KC_LEFT,KC_DOWN, KC_UP, KC_RGHT,KC_DEL ,_______,
     _______,ALT_TAB,KC_SCTAB,KC_CTAB,KC_PGDN,LSFT(KC_INS),                               WIN_LEFT,WIN_DN,WIN_UP,WIN_RGHT,KC_PSTE,_______,  // KC_PSTE works in XTerm to emulate middle-click
                     MS_WHUP,MS_WHDN,                                                                     INS_HARD,KC_ENTER,
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
     _______,KC_EXLM, KC_AT ,KC_HASH,KC_DLR, KC_PERC,                                     KC_KP_EQUAL, KC_7,KC_8,KC_9,KC_KP_PLUS,_______,
     _______,KC_SCLN,KC_COLN,KC_LCBR,KC_LPRN,KC_LBRC,                                   KC_KP_ASTERISK,KC_4,KC_5,KC_6,MINS_UNDSCR,_______,
     _______,KC_CIRC,KC_AMPR,KC_RCBR,KC_RPRN,KC_RBRC,                                     KC_COMM,     KC_1,KC_2,KC_3,KC_KP_SLASH,_______,
                     KC_GRV,KC_TILDE,                                                                        KC_0,KC_KP_DOT,
                                     KC_ESC ,KC_SPC ,                                        _______,_______,
                                     KC_0 ,  _______,                                        _______,_______,
                                     _______,_______,                                        _______,_______
                                  /* ^^^^ use these */                                      /* ^^^^ can't be used */
  ),

  [L_FUNC] = LAYOUT_5x6(
     _______, KC_NO , KC_NO , KC_NO , KC_NO , KC_NO ,                                      KC_NO , KC_NO , KC_NO , KC_NO , KC_NO , KC_NO ,
     _______, KC_NO ,KC_MUTE,KC_VOLD,KC_VOLU,DF(L_WASD),                                   KC_PSCR, KC_F7 , KC_F8 , KC_F9 , KC_F10,DF(L_WASD),
     _______,KC_LGUI,KC_LALT,KC_LSFT,KC_LCTL,DF(L_QWER),                                   KC_SLCK, KC_F4 , KC_F5 , KC_F6 , KC_F11,DF(L_QWER),
     _______, KC_NO , KC_NO , KC_NO , KC_NO ,DF(L_COLM),                                   KC_PAUS, KC_F1 , KC_F2 , KC_F3 , KC_F12,DF(L_COLM),
                      KC_NO , KC_NO ,                                                                         KC_NO , KC_NO ,
                                     _______,_______,                                        _______,_______,
                                     _______,_______,                                        _______,_______,
                                     _______,_______,                                        _______,_______
                                  /* ^^^^ use these */                                      /* ^^^^ can't be used */
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
                                  /* ^^^^ can't be used */                             /* use these ^^^^ */
  ),
};

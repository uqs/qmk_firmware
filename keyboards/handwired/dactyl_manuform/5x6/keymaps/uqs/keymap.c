// vi:et sw=4 cc=5,15,25,35,45,55,65:
#include "uqs.h"

/*
 * My custom layout macro that allows for easy diff(1) operations between
 * various keyboards. It probably doesn't make sense to you.
 */
#define LAYOUT_uqs(               \
    L00, L01, L02, L03, L04, L05, \
    L10, L11, L12, L13, L14, L15, \
    L20, L21, L22, L23, L24, L25, \
    L30, L31, L32, L33, L34, L35, \
              L42, L43,           \
         L44, L45, L54,           \
         L55, L52, L53, /*XXX*/   \
    R00, R01, R02, R03, R04, R05, \
    R10, R11, R12, R13, R14, R15, \
    R20, R21, R22, R23, R24, R25, \
    R30, R31, R32, R33, R34, R35, \
              R42, R43,           \
         R40, R41, R50,           \
         R51, R52, R53 /*XXX*/    \
)                                 \
LAYOUT_5x6(                       \
  L00, L01, L02, L03, L04, L05,                          R00, R01, R02, R03, R04, R05, \
  L10, L11, L12, L13, L14, L15,                          R10, R11, R12, R13, R14, R15, \
  L20, L21, L22, L23, L24, L25,                          R20, R21, R22, R23, R24, R25, \
  L30, L31, L32, L33, L34, L35,                          R30, R31, R32, R33, R34, R35, \
            L42, L43,                                              R42, R43,           \
                      L44, L45,                          R40, R41,                     \
                                L54, L55,      R50, R51,                               \
                                L52, L53,      R52, R53                                \
)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_QWER] = LAYOUT_uqs(
     KC_GESC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,
     KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
     KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,
                         KC_LBRC,  KC_RBRC,
     /*thumb*/ LT_EXTD_ESC, KC_SPC, LT_MOUSE_ALT_SHIFT_INS,
      /*aux*/  KC_LEAD,  OSM_GUI,  KC_LALT,
     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_BSPC,
     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  RSFT_T(KC_GRV),
                         KC_MINS,  KC_EQL,
     /*thumb*/ LT_FUNC_SHIFT_INS, KC_ENT, LT_NUM_BSPC,
      /*aux*/  KC_RALT,  KC_APP,   KC_LEAD
  ),

  [L_WASD] = LAYOUT_uqs(
     KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,
     KC_T,     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,
     KC_G,     KC_LSFT,  KC_A,     KC_S,     KC_D,     KC_F,
     KC_B,     KC_GRV,   KC_Z,     KC_X,     KC_C,     KC_V,
                         KC_LBRC,  KC_RBRC,
     /*thumb*/ KC_LCTL,  KC_SPC,   KC_R,
      /*aux*/  A(KC_M),  KC_LGUI,  KC_LALT,
     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_BSPC,
     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSLS,
     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  RSFT_T(KC_GRV),
                         KC_MINS,  KC_EQL,
     /*thumb*/ DF(L_COLM), KC_ENT, KC_BSPC,
      /*aux*/  KC_F5,    KC_RALT,  KC_APP
  ),

  // TODO: implement CAPS_WORD on the left pinky shift on tap (maybe?) But it
  // can't actually set caps lock, as I'm rebinding that for a saner laptop
  // keyboard. See drashna's keymap.
  [L_COLM] = LAYOUT_uqs(
     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
     KC_NO,    KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,
     KC_NO,    KC_A_AE,  KC_A_R,   KC_S_S,   KC_C_T,   KC_G,
     KC_NO,    KC_Z,     KC_X,     KC_C,     KC_D,     KC_V,
                         KC_GRV,   KC_RBRC,
                         /* This ] ^^^^  is here for Gmail hotkeys only */
     /*thumb*/ LT_EXTD_ESC, SFT_T(KC_SPC), LT(L_MOUSE, KC_TAB),
      /*aux*/  KC_LEAD,  OSM_GUI,  KC_LALT,
     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
     KC_J,     KC_L,     KC_U_UE,  KC_Y,     KC_QUOT,  KC_NO,
     KC_M,     KC_C_N,   KC_S_E,   KC_A_I,   KC_O_OE,  KC_NO,
     KC_K,     KC_H,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_NO,
                         KC_MINUS, KC_BSLS,
     /*thumb*/ LT_FUNC_SHIFT_INS, KC_ENT, LT_NUM_BSPC,
      /*aux*/  KC_LEAD,  KC_RALT,  KC_APP
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
  [L_EXTD] = LAYOUT_uqs(
     KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,
     _______,  WIN_PREV, TM_PREV,  KC_PGUP,  TM_NEXT,  WIN_NEXT,
     _______,  OSM_GUI,  OSM_ALT,  OSM_SFT,  OSM_CTL,  KC_RALT,
     _______,  ALT_TAB,  KC_SCTAB, KC_CTAB,  KC_PGDN,  LSFT(KC_INS),
                         MS_WHUP,  MS_WHDN,
     /*thumb*/ _______,  _______,  _______,
      /*aux*/  _______,  _______,  _______,
               /* ^^^^ can't be used */
     KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,
     KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   KC_INS,   KC_NO,
     KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_DEL,   KC_NO,
     WIN_LEFT, WIN_DN,   WIN_UP,   WIN_RGHT, KC_PSTE,  KC_NO,  // KC_PSTE works in XTerm to emulate middle-click
                         INS_HARD, KC_ENTER,
     /*thumb*/ _______,  KC_BSPC,  _______,
      /*aux*/  _______,  _______,  _______
               /* ^^^^ use these */
  ),

  // Num/Symbol. This works somehow without turning on Numlock first. I've not
  // managed to get Alt-codes working under Windows though, might be an
  // artefact of using US Intl (nope, I'm using USCmpse custom layout)?
  // TODO: maybe swap # with ;, that way I can roll :w or :wq which I need often ...
  [L_NUM] = LAYOUT_uqs(
     _______,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
     _______,  KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,
     _______,  KC_SCLN,  KC_COLN,  KC_LCBR,  KC_LPRN,  KC_LBRC,
     _______,  KC_CIRC,  KC_AMPR,  KC_RCBR,  KC_RPRN,  KC_RBRC,
                         KC_GRV,   KC_TILDE,
     /*thumb*/ KC_ESC,   KC_SPC,   KC_0,
      /*aux*/  _______,  _______,  _______,
               /* ^^^^ use these */
     KC_NUMLOCK,KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
     KC_KP_EQUAL, KC_7,  KC_8,     KC_9,     KC_KP_PLUS,_______,
     KC_KP_ASTERISK,KC_4,KC_5,     KC_6,     MINS_UNDSCR,_______,
     KC_COMM,  KC_1,     KC_2,     KC_3,     KC_KP_SLASH,KC_KP_ENTER,  // Enter here, because thumb is occupied
                         KC_0,     KC_KP_DOT,
     /*thumb*/ _______,  _______,  _______,
      /*aux*/  _______,  _______,  _______
               /* ^^^^ can't be used */
  ),

  [L_FUNC] = LAYOUT_uqs(
     _______,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
     _______,  KC_NO,    KC_MUTE,  KC_VOLD,  KC_VOLU,  DF(L_WASD),
     _______,  KC_LGUI,  KC_LALT,  KC_LSFT,  KC_LCTL,  DF(L_QWER),
     _______,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    DF(L_COLM),
                         KC_NO,    KC_NO,
     /*thumb*/ _______,  _______,  _______,
      /*aux*/  _______,  _______,  _______,
               /* ^^^^ use these */
     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
     KC_PSCR,  KC_F7,    KC_F8,    KC_F9,    KC_F10,   DF(L_WASD),
     KC_SLCK,  KC_F4,    KC_F5,    KC_F6,    KC_F11,   DF(L_QWER),
     KC_PAUS,  KC_F1,    KC_F2,    KC_F3,    KC_F12,   DF(L_COLM),
                         KC_NO,    KC_NO,
     /*thumb*/ _______,  _______,  _______,
      /*aux*/  _______,  _______,  _______
               /* ^^^^ can't be used */

  ),

  [L_MOUSE] = LAYOUT_uqs(
     RGB_TOG,  _______,  _______,  _______,  _______,  _______,
     RGB_MOD,  RGB_HUI,  KC_MUTE,  KC_VOLD,  KC_VOLU,  RGB_SAI,
     RGB_RMOD, RGB_HUD,  KC_ACL0,  KC_ACL1,  KC_ACL2,  RGB_SAD,
     /* Plain, Breath, Rainbow, Swirl, Snake, KnightRider, Xmas, Gradient */
     RGB_M_P,  RGB_M_B,  RGB_M_R,  RGB_M_SW, RGB_M_SN, RGB_M_K,
                         RGB_M_X,  RGB_M_G,
     /*thumb*/ _______,  _______,  _______,
      /*aux*/  _______,  _______,  _______,
               /* ^^^^ can't be used */
     _______,  _______,  _______,  _______,  _______,  _______,
     _______,  _______,  KC_MS_UP, MS_WHUP,  _______,  _______,
     MS_WHLEFT,KC_MS_L,  KC_MS_D,  KC_MS_R,  MS_WHRGHT,_______,
     _______,  MS_WHDN,  KC_BTN3,  KC_BTN4,  KC_BTN5,  _______,
                         _______,  _______,
     /*thumb*/ KC_BRIU,  KC_BTN1,  KC_BTN2,
      /*aux*/  RGB_VAI,  RGB_VAD,  KC_BRID
               /* use these ^^^^ */
  ),
};

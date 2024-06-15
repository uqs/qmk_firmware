
#ifdef COMBO_ENABLE
enum combo_events {
  C_AUML,
  C_OUML,
  C_UUML,
  C_SZ,
  C_CBR,
  C_PRN,
  C_BRC,
  C_QUOT,
};

// Maybe use this?
// #define COMBO_ONLY_FROM_LAYER L_COLM

// The official way has way too much duplication and intermediate names for my taste...
const uint16_t PROGMEM my_action_combos[][3] = {
#ifndef KEYBOARD_preonic_rev3
    [C_AUML] = {KC_G_A, KC_W, COMBO_END},
    [C_OUML] = {KC_G_O, KC_Y, COMBO_END},
    [C_UUML] = {KC_C_N, KC_U, COMBO_END},
    [C_SZ]   = {KC_S_S, KC_Z, COMBO_END},
#else
    [C_AUML] = {COMBO_END},
    [C_OUML] = {COMBO_END},
    [C_UUML] = {COMBO_END},
    [C_SZ]   = {COMBO_END},
#endif
    [C_CBR]  = {KC_COLN, KC_LCBR, COMBO_END},
    [C_PRN]  = {KC_LCBR, KC_LPRN, COMBO_END},
    [C_BRC]  = {KC_LPRN, KC_LBRC, COMBO_END},
    [C_QUOT] = {KC_COMM, KC_DOT, COMBO_END},
};
const uint16_t PROGMEM my_combos[][4] = {
#ifndef KEYBOARD_preonic_rev3
    {KC_RCBR, KC_X, KC_C, COMBO_END},  // hotkey for gmail archive
    {KC_TAB,  KC_G_A, KC_A_R, COMBO_END},
    {KC_BSLS, KC_F, KC_U, COMBO_END},
    {LSFT(KC_BSLS), KC_P, KC_L, COMBO_END},
    {KC_MINUS, KC_C_T, KC_C_N, COMBO_END},
    {KC_MINUS, KC_LPRN, KC_4, COMBO_END},  // dupe to work on NUM layer
    {KC_0, KC_4, KC_5, COMBO_END},
    {KC_0, KC_5, KC_6, COMBO_END},
    {KC_DOT, KC_2, KC_3, COMBO_END},
    {LSFT(KC_MINUS), KC_D, KC_H, COMBO_END},
    {LSFT(KC_MINUS), KC_RPRN, KC_1, COMBO_END},  // dupe to work on NUM
    {KC_GRV,  KC_Q, KC_W, COMBO_END},  // remove this? turn into esc:wq?
    {KC_GRV,  KC_C, KC_COMM, COMBO_END},
    {LSFT(KC_GRV), KC_G, KC_M, COMBO_END},
#else
    {COMBO_END},
    {COMBO_END},
    {KC_BSLS, KC_E, KC_I, COMBO_END},
    {LSFT(KC_BSLS), KC_R, KC_U, COMBO_END},
    {KC_MINUS, KC_F, KC_J, COMBO_END},
    {KC_MINUS, KC_LPRN, KC_4, COMBO_END},  // dupe to work on NUM layer
    {KC_0, KC_4, KC_5, COMBO_END},
    {KC_0, KC_5, KC_6, COMBO_END},
    {KC_DOT, KC_2, KC_3, COMBO_END},
    {LSFT(KC_MINUS), KC_V, KC_M, COMBO_END},
    {LSFT(KC_MINUS), KC_RPRN, KC_1, COMBO_END},  // dupe to work on NUM
    {COMBO_END},
    {KC_GRV,  KC_C, KC_COMM, COMBO_END},
    {LSFT(KC_GRV), KC_G, KC_H, COMBO_END},
#endif
    {KC_BTN3, KC_BTN1, KC_BTN2, COMBO_END},
    {KC_BTN1, KC_BTN2, KC_BTN3, COMBO_END},
};

const uint16_t COMBO_LEN = ARRAY_SIZE(my_action_combos) + ARRAY_SIZE(my_combos);

#define MY_ACTION_COMBO(ck) \
    [ck] = { .keys = &(my_action_combos[ck][0]) }
#define MY_COMBO(ck) \
    { .keys = &(my_combos[ck][1]), .keycode = my_combos[ck][0] }

// NOTE: while my_combos can live in PROGMEM, the key_combos data also
// contains state that is tweaked at runtime, so we need to indirect. Ugh.
#define COMBO_STATICALLY
#ifdef COMBO_STATICALLY
// TODO: fill this at runtime with a loop?
combo_t key_combos[] = {
  MY_ACTION_COMBO(0),
  MY_ACTION_COMBO(1),
  MY_ACTION_COMBO(2),
  MY_ACTION_COMBO(3),
  MY_ACTION_COMBO(4),
  MY_ACTION_COMBO(5),
  MY_ACTION_COMBO(6),
  MY_ACTION_COMBO(7),
  MY_ACTION_COMBO(8),
  MY_COMBO(0),
  MY_COMBO(1),
  MY_COMBO(2),
  MY_COMBO(3),
  MY_COMBO(4),
  MY_COMBO(5),
  MY_COMBO(6),
  MY_COMBO(7),
  MY_COMBO(8),
  MY_COMBO(9),
  MY_COMBO(10),
  MY_COMBO(11),
  MY_COMBO(12),
  MY_COMBO(13),
  MY_COMBO(14),
};

_Static_assert(ARRAY_SIZE(key_combos) ==
               (ARRAY_SIZE(my_action_combos) + ARRAY_SIZE(my_combos)),
               "Number of combo definitions does not match up!");
#else
combo_t key_combos[ARRAY_SIZE(my_action_combos) + ARRAY_SIZE(my_combos)];
#endif

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case C_AUML:
            if (pressed) {
                tap_code16(KC_RALT);
                tap_code16(LSFT(KC_QUOT));
                tap_code16(KC_A);
            }
            break;
        case C_OUML:
            if (pressed) {
                tap_code16(KC_RALT);
                tap_code16(LSFT(KC_QUOT));
                tap_code16(KC_O);
            }
            break;
        case C_UUML:
            if (pressed) {
                tap_code16(KC_RALT);
                tap_code16(LSFT(KC_QUOT));
                tap_code16(KC_U);
            }
            break;
        case C_SZ:
            if (pressed) {
                tap_code16(KC_RALT);
                tap_code16(KC_S);
                tap_code16(KC_S);
            }
            break;
        case C_CBR:
            if (pressed) {
                tap_code16(KC_LCBR);
                tap_code16(KC_RCBR);
                tap_code16(KC_LEFT);
            }
            break;
        case C_PRN:
            if (pressed) {
                tap_code16(KC_LPRN);
                tap_code16(KC_RPRN);
                tap_code16(KC_LEFT);
            }
            break;
        case C_BRC:
            if (pressed) {
                tap_code16(KC_LBRC);
                tap_code16(KC_RBRC);
                tap_code16(KC_LEFT);
            }
            break;
        case C_QUOT:
            if (pressed) {
                tap_code16(KC_QUOT);
                tap_code16(KC_QUOT);
                // need to drop shift here, if present!
                unregister_mods(MOD_BIT(KC_LSFT));
                unregister_mods(MOD_BIT(KC_RSFT));
                tap_code16(KC_LEFT);
            }
            break;
    }
}
#endif


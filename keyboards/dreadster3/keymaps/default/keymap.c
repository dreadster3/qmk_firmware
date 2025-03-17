// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_macros { M_NDESK = SAFE_RANGE, M_PDESK };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static keyrecord_t *previous_record  = NULL;
    static uint16_t     previous_keycode = 0;

    switch (keycode) {
        case M_NDESK:
            // Debounces events
            if (record->event.pressed) {
                if (previous_record != NULL) {
                    if (previous_record->event.time == record->event.time && previous_keycode == keycode) {
                        break;
                    }
                }
                SEND_STRING(SS_LCTL(SS_LGUI(SS_TAP(X_RGHT))));
                previous_record = record;
            }
            break;
        case M_PDESK:
            // Debounces events
            if (record->event.pressed) {
                if (previous_record != NULL) {
                    if (previous_record->event.time == record->event.time && keycode == previous_keycode) {
                        break;
                    }
                }
                SEND_STRING(SS_LCTL(SS_LGUI(SS_TAP(X_LEFT))));
                previous_record = record;
            }
            break;
    }

    return true;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_EQL,         KC_1,         KC_2,         KC_3,  KC_4,  KC_5,                          KC_6,    KC_7,    KC_8,    KC_9,           KC_0,            KC_MINS,
    KC_TAB,         KC_Q,         KC_W,         KC_E,  KC_R,  KC_T,                          KC_Y,    KC_U,    KC_I,    KC_O,           KC_P,            KC_BSLS,
    LGUI_T(KC_ESC), KC_A,         KC_S,         KC_D,  KC_F,  KC_G,                          KC_H,    KC_J,    KC_K,    KC_L,           LT(2, KC_SCLN),  RGUI_T(KC_QUOT),
    KC_LSFT,        LCTL_T(KC_Z), LALT_T(KC_X), KC_C,  KC_V,  KC_B,   KC_MUTE,      KC_MPLY, KC_N,    KC_M,    KC_COMM, RALT_T(KC_DOT), RCTL_T(KC_SLSH), KC_RSFT,
                                  KC_GRV,       KC_NO, MO(1), KC_SPC, KC_BSPC,      KC_TAB,  KC_ENT,   MO(2), KC_LBRC, KC_RBRC
  ),
  [1] = LAYOUT(
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_F12,
    _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                          KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
    _______, KC_EQL,  KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______,        _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
                      _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______
  ),
  [2] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______ ,QK_BOOT,
    _______, KC_INS,  KC_PSCR, KC_APP,  XXXXXXX, XXXXXXX,                          KC_PGUP, XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX, KC_BSPC,
    _______, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_CAPS,                          KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,  XXXXXXX,
    _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX, _______,        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                      _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______
  )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
[0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(M_NDESK, M_PDESK)},
[1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_MNXT, KC_MPRV)},
[2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_MNXT, KC_MPRV)},
};
#endif
// clang-format on

// Copyright 2025 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

#ifdef OLED_ENABLE
static void render_logo(void) {
    static const char PROGMEM raw_logo[] = {
        0,   0,   0,   66,  195, 195, 195, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 195, 195, 195, 66,  0,   0,   0,   0,   0,   112, 240, 240, 240, 224, 0, 0,   0,   0,   0,   0,   0,   0, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0,  0,  0,  0,  0,   0,   0,   0,   0,   96,  112, 240, 240, 224, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   240, 240, 240, 0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 224, 240, 112, 240, 224, 128, 128, 0, 0, 0, 0, 24, 24, 24, 24, 255, 255, 255, 255, 255, 239, 135, 63, 124, 252, 255, 255, 255, 255, 252, 124, 63, 135, 255, 255, 255, 255, 255, 255, 24, 24, 24, 24, 0, 252, 252, 255, 255, 135, 255, 255, 255, 255, 130, 135, 255, 255, 252, 254, 143, 7, 15, 31, 252, 255, 255, 247, 191, 191, 188, 60, 0,
        204, 255, 255, 183, 255, 255, 252, 252, 128, 252, 254, 255, 183, 135, 255, 255, 255, 128, 140, 190, 191, 191, 255, 247, 247, 247, 192, 199, 7,   255, 255, 255, 135, 199, 199, 194, 252, 252, 255, 255, 183, 191, 191, 188, 0,   2, 135, 255, 255, 252, 255, 143, 7, 15, 207, 193, 205, 205, 140, 255, 255, 247, 243, 0, 0, 0, 0, 99, 99, 99, 99, 127, 127, 127, 127, 127, 127, 127, 126, 126, 126, 126, 126, 126, 126, 126, 126, 127, 127, 127, 127, 127, 127, 127, 127, 99,  99, 99, 99, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0,   0,   3,   3,   3,   3,   3,   3,   3,   0, 3, 3, 3, 3,  3,  3,  3,  3,   3,   0,   1,   3,   3,   3,   3,  3,   3,   3,   3,   3,   3,   3,   3,   3,  3,   3,   0,   0,   0,   0,   3,   3,  3,  3,  3,  1, 0,   0,   3,   3,   3,   3,   3,   3,   0,   1,   3,   3,   3,   3,   3,   3,   0, 0,  0,  0,   3,   3,   3,   3,   3,   0,   0,  0,
    };
    oled_write_raw_P(raw_logo, sizeof(raw_logo));
}

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180;
    }

    return OLED_ROTATION_270;
}

static const char *depad_str(const char *depad_str, char depad_char) {
    while (*depad_str == depad_char)
        ++depad_str;
    return depad_str;
}

const char *wpm_to_str(void) {
    return depad_str(get_u8_str(get_current_wpm(), ' '), ' ');
}

static void render_spacer(uint8_t char_length) {
    static const char PROGMEM spacer_char[] = {8, 8, 8, 8, 8, 8, 8};
    if (char_length > 5) {
        char_length = 5;
    }
    for (uint8_t i = 0; i < char_length; i++) {
        oled_write_raw_P(spacer_char, sizeof(spacer_char));
        oled_advance_char();
    }
}

const char *layer_string(layer_state_t layer_state) {
    uint32_t layer = get_highest_layer(layer_state);
    switch (layer) {
        case 0:
            return "Zero\0";
        case 1:
            return "One\0";
        case 2:
            return "Two\0";
        case 3:
            return "Three\0";
        case 4:
            return "Four\0";
        case 5:
            return "Five\0";
        case 6:
            return "Six\0";
        case 7:
            return "Seven\0";
        default:
            return get_u16_str(layer, ' ');
    }
}

bool process_detected_host_os_kb(os_variant_t detected_os) {
    if (!process_detected_host_os_user(detected_os)) {
        return false;
    }

    oled_set_cursor(0, 7);
    switch (detected_os) {
        case OS_MACOS:
            oled_write_ln("MacOS", false);
            break;
        case OS_IOS:
            oled_write_ln("Apple", false);
            break;
        case OS_WINDOWS:
            oled_write_ln("Win", false);
            break;
        case OS_LINUX:
            oled_write_ln("Linux", false);
            break;
        default:
            oled_write_ln("Unkno", false);
            break;
    }

    return true;
}

void init_master(void) {
    // Render Layer
    oled_set_cursor(0, 0);
    oled_write_ln("Layer", false);
    render_spacer(5);
    oled_write_ln(layer_string(layer_state), false);

    // Render OS
    oled_set_cursor(0, 5);
    oled_write_ln("OS", false);
    render_spacer(5);
    oled_write_ln("Wait", false);

    // Render WPM
    oled_set_cursor(0, 9);
    oled_write_ln("WPM", false);
    render_spacer(5);
    oled_write_ln(wpm_to_str(), false);
}

layer_state_t layer_state_set_kb(layer_state_t state) {
    state = layer_state_set_user(state);
    oled_set_cursor(0, 3);
    oled_write_ln(layer_string(state), false);
    return state;
}

// Runs during keyboard initialization
void keyboard_post_init_kb(void) {
    if (!is_keyboard_master()) {
        render_logo();
        oled_scroll_right();
        return;
    }

    init_master();

    keyboard_post_init_user();
}

// Runs every loop
bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }

    if (!is_keyboard_master()) {
        render_logo();
        oled_scroll_right();
        return false;
    }

    oled_set_cursor(0, 11);
    oled_write_ln(wpm_to_str(), false);

    return false;
}
#endif

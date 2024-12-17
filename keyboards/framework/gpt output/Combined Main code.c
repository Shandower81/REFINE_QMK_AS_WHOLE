#2
RGB_MATRIX_ENABLE = yes

#2
#pragma once

#define RGB_MATRIX_KEYPRESSES // Enable reactive effects based on key presses
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS // Enable framebuffer effects for animations

#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150 // Maximum brightness for LEDs
#define RGB_DISABLE_WHEN_USB_SUSPENDED // Turn off RGB when USB is suspended
#define RGB_MATRIX_LED_PROCESS_LIMIT 16 // Limit the number of LEDs processed per cycle
#define RGB_MATRIX_LED_FLUSH_LIMIT 16 // Limit LED flushes per cycle

// Define the size of your LED matrix
#define DRIVER_LED_TOTAL 24 // Total number of LEDs (4 rows x 6 columns)




#3
#pragma once

#include "quantum.h"

// Define matrix size
#define LED_MATRIX_ROWS 6
#define LED_MATRIX_COLS 4

#define DRIVER_LED_TOTAL (LED_MATRIX_ROWS * LED_MATRIX_COLS)

// Map LEDs to their matrix positions
#define LED_MATRIX_POSITIONS { \
    { 0,  1,  2,  3  }, \
    { 4,  5,  6,  7  }, \
    { 8,  9,  10, 11 }, \
    { 12, 13, 14, 15 }, \
    { 16, 17, 18, 19 }, \
    { 20, 21, 22, 23 }  \
}

// Define LED physical coordinates (optional for visual effects)
#define RGB_MATRIX_COORDINATES { \
    {0, 0}, {20, 0}, {40, 0}, {60, 0}, \
    {0, 20}, {20, 20}, {40, 20}, {60, 20}, \
    {0, 40}, {20, 40}, {40, 40}, {60, 40}, \
    {0, 60}, {20, 60}, {40, 60}, {60, 60}, \
    {0, 80}, {20, 80}, {40, 80}, {60, 80}, \
    {0, 100}, {20, 100}, {40, 100}, {60, 100} \
}






#include QMK_KEYBOARD_H

# 1
    enum custom_keycodes {
        KC_HUE_INC = SAFE_RANGE,
        KC_HUE_DEC,
        KC_SAT_INC,
        KC_SAT_DEC,
        KC_VAL_INC,
        KC_VAL_DEC,
        KC_KEY_SELECT
    };

#2
static uint8_t selected_row = 0;
static uint8_t selected_col = 0;
static uint16_t selected_led = 0;
static uint8_t hsv_hue = 0;
static uint8_t hsv_sat = 255;
static uint8_t hsv_val = 255;


#3
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_6x4(
        KC_HUE_INC, KC_HUE_DEC, KC_SAT_INC, KC_SAT_DEC, 
        KC_VAL_INC, KC_VAL_DEC, KC_KEY_SELECT, _______, 
        _______,    _______,    _______,    _______, 
        _______,    _______,    _______,    _______, 
        _______,    _______,    _______,    _______, 
        _______,    _______,    _______,    _______
    )
};


#2
void update_led_color(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val) {
    uint16_t led_index = row * LED_MATRIX_COLS + col;
    rgb_matrix_set_color(led_index, hue, sat, val);
}

void select_key(uint8_t row, uint8_t col) {
    selected_row = row;
    selected_col = col;
    selected_led = row * LED_MATRIX_COLS + col;
}







#2
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_HUE_INC:
                hsv_hue = (hsv_hue + 10) % 256;
                update_led_color(selected_row, selected_col, hsv_hue, hsv_sat, hsv_val);
                return false;
            case KC_HUE_DEC:
                hsv_hue = (hsv_hue - 10 + 256) % 256;
                update_led_color(selected_row, selected_col, hsv_hue, hsv_sat, hsv_val);
                return false;
            case KC_SAT_INC:
                hsv_sat = CLAMP(hsv_sat + 10, 0, 255);
                update_led_color(selected_row, selected_col, hsv_hue, hsv_sat, hsv_val);
                return false;
            case KC_SAT_DEC:
                hsv_sat = CLAMP(hsv_sat - 10, 0, 255);
                update_led_color(selected_row, selected_col, hsv_hue, hsv_sat, hsv_val);
                return false;
            case KC_VAL_INC:
                hsv_val = CLAMP(hsv_val + 10, 0, 255);
                update_led_color(selected_row, selected_col, hsv_hue, hsv_sat, hsv_val);
                return false;
            case KC_VAL_DEC:
                hsv_val = CLAMP(hsv_val - 10, 0, 255);
                update_led_color(selected_row, selected_col, hsv_hue, hsv_sat, hsv_val);
                return false;
            case KC_KEY_SELECT:
                // Select key based on keypress
                select_key(record->event.key.row, record->event.key.col);
                return false;
        }
    }
    return true;
}









#1
qmk compile -kb <keyboard_name> -km <keymap_name>
qmk flash -kb <keyboard_name> -km <keymap_name>

#2
qmk compile -kb <keyboard> -km <keymap>
qmk flash -kb <keyboard> -km <keymap>

#3
qmk compile -kb <your_keyboard> -km <your_keymap>
qmk flash -kb <your_keyboard> -km <your_keymap>

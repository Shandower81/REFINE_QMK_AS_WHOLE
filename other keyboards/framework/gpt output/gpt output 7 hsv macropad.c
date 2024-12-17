/* 4 columns x 6 rows




 
If your macropad has **4 columns and 6 rows** (4x6), the instructions remain
largely the same, but I’ll adjust key configuration details for clarity.

---

### 1. **Adjust LED Matrix Dimensions**

In `led_config.h`, update the dimensions and mappings to reflect your **4x6 matrix** (4 columns, 6 rows).
*/
//c
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

/*
---

### 2. **Update Keymap for 4x6 Layout**

In your `keymap.c`, structure the layout as a **4x6 matrix**:
*/
//c
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

/*
The `LAYOUT_ortho_6x4` corresponds to the **4x6 matrix layout**.

---

### 3. **Select Key and Adjust HSV**

The logic for selecting a key and adjusting its HSV remains the same. Use the row and column to identify the key and update its color using:
*/
//c
void update_led_color(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val) {
    uint16_t led_index = row * LED_MATRIX_COLS + col;
    rgb_matrix_set_color(led_index, hue, sat, val);
}

void select_key(uint8_t row, uint8_t col) {
    selected_row = row;
    selected_col = col;
    selected_led = row * LED_MATRIX_COLS + col;
}

/*
---

### 4. **Handle Custom Keycodes for HSV Adjustment**

The keycodes remain the same as before:
*/
//c
enum custom_keycodes {
    KC_HUE_INC = SAFE_RANGE,
    KC_HUE_DEC,
    KC_SAT_INC,
    KC_SAT_DEC,
    KC_VAL_INC,
    KC_VAL_DEC,
    KC_KEY_SELECT,
};


// Bind them to actions that use `process_record_user`:

//c
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
                select_key(record->event.key.row, record->event.key.col);
                return false;
        }
    }
    return true;
}

/*
---

### 5. **Test and Flash the Firmware**

Compile and flash the firmware for your 4x6 macropad:
*/
//bash
qmk compile -kb <your_keyboard> -km <your_keymap>
qmk flash -kb <your_keyboard> -km <your_keymap>

/*
### Summary

This configuration supports your 4x6 macropad with a per-key HSV adjustment system. If the layout, matrix, or any settings need further tuning, let me know!
*/

/* for a 4x6 macropad add the ability to select any key to adjust
hsv for that key only





 
To implement per-key HSV adjustment functionality for a 4x6 macropad,
you'll need to modify the QMK firmware to allow selecting a specific
key and adjusting its HSV (hue, saturation, and value) properties
independently. Here's how to achieve that:

---

### Steps to Add Per-Key HSV Adjustment

#### 1. **Set Up RGB Lighting in QMK**
Ensure that RGB lighting is enabled in your keyboard’s `config.h` and `rules.mk` files.

In `config.h`:
*/
//c
#pragma once

#define RGBLIGHT_ENABLE  // Enable RGB lighting
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17

#define RGBLIGHT_EFFECT_RGB_TESTING  // Allow RGB control for individual keys
#define RGBLIGHT_LAYERS // Enable RGB layers


// In `rules.mk`:

//makefile
RGBLIGHT_ENABLE = yes  # Enable RGB lighting

/*
---

#### 2. **Define Per-Key HSV Control**
To manage HSV for individual keys, you’ll use QMK’s `rgblight_sethsv_at` function. Create logic in your `keymap.c` file to track the selected key and apply HSV changes.

---

#### 3. **Track Selected Key**
You need a way to select the key whose HSV you want to adjust. Add a global variable to keep track of the selected key:
*/
//c
#include QMK_KEYBOARD_H

static uint8_t selected_row = 0;
static uint8_t selected_col = 0;
static uint8_t hsv_hue = 0;
static uint8_t hsv_sat = 255;
static uint8_t hsv_val = 255;

/*
---

#### 4. **Add Key Selection and HSV Adjustment Functions**
Create functions to change the selected key and adjust its HSV values. For example:
*/
//c
void adjust_hsv(uint8_t hue_step, uint8_t sat_step, uint8_t val_step) {
    hsv_hue = (hsv_hue + hue_step) % 256;
    hsv_sat = CLAMP(hsv_sat + sat_step, 0, 255);
    hsv_val = CLAMP(hsv_val + val_step, 0, 255);
    rgblight_sethsv_at(hsv_hue, hsv_sat, hsv_val, selected_row * MATRIX_COLS + selected_col);
}

void select_key(uint8_t row, uint8_t col) {
    selected_row = row;
    selected_col = col;
}

/*
---

#### 5. **Define Custom Keycodes**
Add custom keycodes for selecting a key and adjusting HSV. Define these in your `keymap.c`:
*/
//c
enum custom_keycodes {
    KC_HUE_INC = SAFE_RANGE,
    KC_HUE_DEC,
    KC_SAT_INC,
    KC_SAT_DEC,
    KC_VAL_INC,
    KC_VAL_DEC,
    KC_KEY_SELECT
};

/*
---

#### 6. **Handle Keycode Actions**
In `process_record_user`, handle your custom keycodes:
*/
//c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_HUE_INC:
                adjust_hsv(10, 0, 0);  // Increase hue
                return false;
            case KC_HUE_DEC:
                adjust_hsv(-10, 0, 0);  // Decrease hue
                return false;
            case KC_SAT_INC:
                adjust_hsv(0, 10, 0);  // Increase saturation
                return false;
            case KC_SAT_DEC:
                adjust_hsv(0, -10, 0);  // Decrease saturation
                return false;
            case KC_VAL_INC:
                adjust_hsv(0, 0, 10);  // Increase value
                return false;
            case KC_VAL_DEC:
                adjust_hsv(0, 0, -10);  // Decrease value
                return false;
            case KC_KEY_SELECT: {
                // Select a key using current layer's layout
                uint8_t row = record->event.key.row;
                uint8_t col = record->event.key.col;
                select_key(row, col);
                return false;
            }
        }
    }
    return true;
}

/*
---

#### 7. **Update Your Keymap**
Update your keymap to include these keycodes. For a 4x6 macropad:
*/
//c
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_4x6(
        KC_1, KC_2, KC_3, KC_4, KC_5, KC_6,
        KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL,
        KC_HUE_INC, KC_HUE_DEC, KC_SAT_INC, KC_SAT_DEC, KC_VAL_INC, KC_VAL_DEC,
        KC_KEY_SELECT, _______, _______, _______, _______, _______
    )
};

/*
---

#### 8. **Test Your Firmware**
Compile and flash the firmware onto your macropad:
*/
//bash
qmk compile -kb <keyboard_name> -km <keymap_name>
qmk flash -kb <keyboard_name> -km <keymap_name>

/*
Once flashed, use the `KC_KEY_SELECT` key to select a key, then adjust its HSV values using the custom keycodes (`KC_HUE_INC`, `KC_HUE_DEC`, etc.).

---

### Additional Enhancements
- **Feedback**: You can add visual feedback (e.g., a blinking LED) to indicate the currently selected key.
- **EEPROM Storage**: To retain per-key HSV settings after power-off, store the values in the keyboard's EEPROM.

This implementation allows you to adjust the HSV for any specific key on your macropad, giving you fine-grained control over lighting. Let me know if you need help with further customizations!
*/

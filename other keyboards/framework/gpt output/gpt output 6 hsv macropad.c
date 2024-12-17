/* rgblight is for a strip of leds in sequence.  my macropad has
a rp2040 microcontroller with a matrix of leds





 
Since your macropad uses an RP2040 microcontroller with a matrix
of individually addressable LEDs (not a sequential strip), you’ll
want to use QMK’s **RGB Matrix** feature instead of `rgblight`.
RGB Matrix is specifically designed to control LEDs arranged in
a keyboard matrix. Below are the steps to implement per-key HSV
control using the RGB Matrix feature.

---

### Steps to Add Per-Key HSV Adjustment with RGB Matrix

#### 1. **Enable RGB Matrix in `rules.mk`**
In your keyboard's `rules.mk` file, enable the `rgb_matrix` feature:
*/
//makefile
RGB_MATRIX_ENABLE = yes

/*
---

#### 2. **Configure RGB Matrix in `config.h`**
Add the necessary RGB Matrix configuration to your `config.h` file. Include options for fine-grained control and customization:
*/
//c
#pragma once

#define RGB_MATRIX_KEYPRESSES // Enable reactive effects based on key presses
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS // Enable framebuffer effects for animations

#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150 // Maximum brightness for LEDs
#define RGB_DISABLE_WHEN_USB_SUSPENDED // Turn off RGB when USB is suspended
#define RGB_MATRIX_LED_PROCESS_LIMIT 16 // Limit the number of LEDs processed per cycle
#define RGB_MATRIX_LED_FLUSH_LIMIT 16 // Limit LED flushes per cycle

// Define the size of your LED matrix
#define DRIVER_LED_TOTAL 24 // Total number of LEDs (4 rows x 6 columns)

/*
---

#### 3. **Define LED Positions in `led_config.h`**
Create or update the `led_config.h` file to map each LED to its corresponding key in the matrix. This is critical for controlling individual LEDs.

Here’s an example for a 4x6 macropad:
*/
//c
#pragma once

#include "quantum.h"

// Define the position of each LED in the matrix
#define LED_MATRIX_ROWS 4
#define LED_MATRIX_COLS 6

#define LED_MATRIX_TOTAL (LED_MATRIX_ROWS * LED_MATRIX_COLS)

// Map LEDs to their physical positions
#define LED_MATRIX_POSITIONS { \
    { 0,  1,  2,  3,  4,  5 }, \
    { 6,  7,  8,  9, 10, 11 }, \
    { 12, 13, 14, 15, 16, 17 }, \
    { 18, 19, 20, 21, 22, 23 } \
}

// Define physical coordinates for each LED for visual effects
#define RGB_MATRIX_COORDINATES { \
    {0, 0}, {16, 0}, {32, 0}, {48, 0}, {64, 0}, {80, 0}, \
    {0, 16}, {16, 16}, {32, 16}, {48, 16}, {64, 16}, {80, 16}, \
    {0, 32}, {16, 32}, {32, 32}, {48, 32}, {64, 32}, {80, 32}, \
    {0, 48}, {16, 48}, {32, 48}, {48, 48}, {64, 48}, {80, 48} \
}

/*
This maps your 4x6 matrix of keys to the corresponding LED indices and their physical coordinates.

---

#### 4. **Add Per-Key HSV Control Logic**
In `keymap.c`, implement logic to control the HSV values of individual LEDs. Use the `rgb_matrix_set_color` function to set the color for a specific LED.

Add variables to track the selected key and its HSV values:
*/
//c
static uint8_t selected_row = 0;
static uint8_t selected_col = 0;
static uint16_t selected_led = 0;
static uint8_t hsv_hue = 0;
static uint8_t hsv_sat = 255;
static uint8_t hsv_val = 255;


// Create functions to update HSV values and set the LED color:

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

#### 5. **Add Custom Keycodes**
Add custom keycodes to your keymap for selecting keys and adjusting HSV values:
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

/*
---

#### 6. **Process Keycodes for HSV Adjustment**
Handle these keycodes in `process_record_user` to update the selected key and adjust HSV values:
*/
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
                // Select key based on keypress
                select_key(record->event.key.row, record->event.key.col);
                return false;
        }
    }
    return true;
}

/*
---

#### 7. **Update Your Keymap**
Add the custom keycodes to your keymap for testing. For example:
*/
//c
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_4x6(
        KC_HUE_INC, KC_HUE_DEC, KC_SAT_INC, KC_SAT_DEC, KC_VAL_INC, KC_VAL_DEC,
        KC_KEY_SELECT, KC_NO,    KC_NO,     KC_NO,     KC_NO,     KC_NO,
        KC_NO,        KC_NO,    KC_NO,     KC_NO,     KC_NO,     KC_NO,
        KC_NO,        KC_NO,    KC_NO,     KC_NO,     KC_NO,     KC_NO
    )
};

/*
---

#### 8. **Compile and Flash the Firmware**
Compile the firmware and flash it to your macropad:
*/
//bash
qmk compile -kb <keyboard> -km <keymap>
qmk flash -kb <keyboard> -km <keymap>

/*
---

### Testing and Usage
- Use `KC_KEY_SELECT` to select a specific key (press the key you want to adjust).
- Use `KC_HUE_INC`/`KC_HUE_DEC` to adjust the hue of the selected key.
- Use `KC_SAT_INC`/`KC_SAT_DEC` to adjust the saturation.
- Use `KC_VAL_INC`/`KC_VAL_DEC` to adjust the brightness.

Let me know if you need further assistance with this!
*/

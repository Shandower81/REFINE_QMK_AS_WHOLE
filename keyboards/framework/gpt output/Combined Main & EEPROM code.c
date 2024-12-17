#2
done RGB_MATRIX_ENABLE = yes
#1
done EEPROM_ENABLE = yes

#2
#pragma once

done #define RGB_MATRIX_KEYPRESSES // Enable reactive effects based on key presses
done #define RGB_MATRIX_FRAMEBUFFER_EFFECTS // Enable framebuffer effects for animations

done #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150 // Maximum brightness for LEDs
done #define RGB_DISABLE_WHEN_USB_SUSPENDED // Turn off RGB when USB is suspended
done #define RGB_MATRIX_LED_PROCESS_LIMIT 16 // Limit the number of LEDs processed per cycle
done #define RGB_MATRIX_LED_FLUSH_LIMIT 16 // Limit LED flushes per cycle

// Define the size of your LED matrix
done #define DRIVER_LED_TOTAL 24 // Total number of LEDs (4 rows x 6 columns)




#3
#pragma once

#include "quantum.h"

// Define matrix size
#define MATRIX_ROWS 6
#define MATRIX_COLS 4

#define RGB_MATRIX_LED_COUNT (MATRIX_ROWS * MATRIX_COLS)

#define EEPROM_HSV_START 43 // Start storing per-key HSV data at address 43

// Structure for storing HSV values
    typedef struct {
    uint8_t h;
    uint8_t s;
    uint8_t v;
} key_hsv_t;

// Array to store HSV values for all keys
key_hsv_t key_hsv_data[RGB_MATRIX_LED_COUNT];








#include QMK_KEYBOARD_H

# 1
    enum custom_keycodes {
        KC_HUE_INC = SAFE_RANGE,
        KC_SAT_INC,
        KC_VAL_INC,
        KC_KEY_SELECT
    };

#2
static uint8_t selected_row = 0;
static uint8_t selected_col = 0;
static uint16_t selected_led = 0;
static uint8_t hsv_hue = 0;
static uint8_t hsv_sat = 255;
static uint8_t hsv_val = 255;

static uint8_t hsv_step = 10;





#2
void update_led_color(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val) {
    uint16_t led_index = row * MATRIX_COLS + col;

    // Update the in-memory HSV data
    key_hsv_data[led_index].h = hue;
    key_hsv_data[led_index].s = sat;
    key_hsv_data[led_index].v = val;

    // Update the LED color
    rgb_matrix_set_color(led_index, hue, sat, val);

    // Save the new HSV data to EEPROM
    save_hsv_to_eeprom();
}

# 2
void reset_hsv_to_defaults(void) {
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        key_hsv_data[i].h = 0;   // Default hue
        key_hsv_data[i].s = 255; // Default saturation
        key_hsv_data[i].v = 255; // Default value
    }
    save_hsv_to_eeprom();
}

void select_key(uint8_t row, uint8_t col) {
    selected_row = row;
    selected_col = col;
    selected_led = row * MATRIX_COLS + col;
}

// Function to load HSV data from EEPROM
void load_hsv_from_eeprom(void) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) { // int or uint8_t
        eeprom_read_block((void *)&key_hsv_data[i], (const void *)(EEPROM_HSV_START + (i * sizeof(key_hsv_t))), sizeof(key_hsv_t));
    }
}

// Function to save HSV data to EEPROM
void save_hsv_to_eeprom(void) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) { // int or uint8_t
        eeprom_update_block((const void *)&key_hsv_data[i], (void *)(EEPROM_HSV_START + (i * sizeof(key_hsv_t))), sizeof(key_hsv_t));
    }
}


# 1
void keyboard_post_init_user(void) {
    // Load saved HSV data from EEPROM
    load_hsv_from_eeprom();

    // Apply the saved HSV settings to the LEDs
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint16_t led_index = row * MATRIX_COLS + col;
            rgb_matrix_set_color(led_index, key_hsv_data[led_index].hue, key_hsv_data[led_index].sat, key_hsv_data[led_index].val);
        }
    }
}



#2
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_RESET_HSV:
                reset_hsv_to_defaults();
                return false;
            case KC_HUE_INC:
                hsv_hue = (hsv_hue + 10) % 256;
                update_led_color(selected_row, selected_col, hsv_hue, hsv_sat, hsv_val);
                return false;

            case KC_SAT_INC:
                hsv_sat = CLAMP(hsv_sat + 10, 0, 255);
                update_led_color(selected_row, selected_col, hsv_hue, hsv_sat, hsv_val);
                return false;

            case KC_VAL_INC:
                hsv_val = CLAMP(hsv_val + 10, 0, 255);
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

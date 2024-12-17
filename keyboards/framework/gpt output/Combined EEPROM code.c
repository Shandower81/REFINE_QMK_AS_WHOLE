#1
EEPROM_ENABLE = yes


#2
3 bytes/key * 24 keys = 72 bytes


#1
#include "eeprom.h"

#define MATRIX_ROWS 6
#define MATRIX_COLS 4
#define KEY_COUNT (MATRIX_ROWS * MATRIX_COLS)

#define EEPROM_HSV_START 40 // Start storing per-key HSV data at address 40

// Structure for storing HSV values
typedef struct {
    uint8_t hue;
    uint8_t sat;
    uint8_t val;
} key_hsv_t;

// Array to store HSV values for all keys
key_hsv_t key_hsv_data[KEY_COUNT];

// EEPROM address offset for HSV data
#define EEPROM_HSV_START 0

// Function to load HSV data from EEPROM
void load_hsv_from_eeprom(void) {
    for (int i = 0; i < KEY_COUNT; i++) { // int or uint8_t
        eeprom_read_block((void *)&key_hsv_data[i], (const void *)(EEPROM_HSV_START + (i * sizeof(key_hsv_t))), sizeof(key_hsv_t));
    }
}

// Function to save HSV data to EEPROM
void save_hsv_to_eeprom(void) {
    for (int i = 0; i < KEY_COUNT; i++) { // int or uint8_t
        eeprom_update_block((const void *)&key_hsv_data[i], (void *)(EEPROM_HSV_START + (i * sizeof(key_hsv_t))), sizeof(key_hsv_t));
    }
}


#1
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


#1
void update_led_color(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val) {
    uint16_t led_index = row * MATRIX_COLS + col;

    // Update the in-memory HSV data
    key_hsv_data[led_index].hue = hue;
    key_hsv_data[led_index].sat = sat;
    key_hsv_data[led_index].val = val;

    // Update the LED color
    rgb_matrix_set_color(led_index, hue, sat, val);

    // Save the new HSV data to EEPROM
    save_hsv_to_eeprom();
}


#2
void reset_hsv_to_defaults(void) {
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        key_hsv_data[i].hue = 0; // Default hue
        key_hsv_data[i].sat = 255; // Default saturation
        key_hsv_data[i].val = 255; // Default value
    }
    save_hsv_to_eeprom();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_RESET_HSV:
                reset_hsv_to_defaults();
                return false;
        }
    }
    return true;
}


#1
qmk compile -kb <your_keyboard> -km <your_keymap>
qmk flash -kb <your_keyboard> -km <your_keymap>

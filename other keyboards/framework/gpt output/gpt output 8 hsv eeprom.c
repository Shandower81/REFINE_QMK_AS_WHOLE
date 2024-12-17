/* add ability to store per key hsv information in eeprom for all keys.





 
To store per-key HSV information in EEPROM for all keys, you can
leverage QMK's EEPROM functions. This allows the macropad to retain
the HSV settings across reboots.

---

### Steps to Add EEPROM Support for Per-Key HSV Storage

#### 1. **Enable EEPROM Support in `rules.mk`**
Ensure that EEPROM support is enabled in your `rules.mk`:
*/
//makefile
EEPROM_ENABLE = yes

/*
---

#### 2. **Define EEPROM Storage Structure**
Create a structure to store HSV data for all keys in the matrix. For a 4x6 matrix, you'll need space for 24 keys (4 columns × 6 rows).

In your `keymap.c` or a separate `eeprom_config.c` file:
*/
//c
#include "eeprom.h"

#define MATRIX_ROWS 6
#define MATRIX_COLS 4
#define KEY_COUNT (MATRIX_ROWS * MATRIX_COLS)

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
    for (int i = 0; i < KEY_COUNT; i++) {
        eeprom_read_block((void *)&key_hsv_data[i], (const void *)(EEPROM_HSV_START + (i * sizeof(key_hsv_t))), sizeof(key_hsv_t));
    }
}

// Function to save HSV data to EEPROM
void save_hsv_to_eeprom(void) {
    for (int i = 0; i < KEY_COUNT; i++) {
        eeprom_update_block((const void *)&key_hsv_data[i], (void *)(EEPROM_HSV_START + (i * sizeof(key_hsv_t))), sizeof(key_hsv_t));
    }
}

/*
This setup:
- Reserves space in EEPROM for 24 HSV structures.
- Provides functions to read from and write to EEPROM.

---

#### 3. **Initialize EEPROM on Startup**
Modify your `keyboard_post_init_user` function to load the HSV values from EEPROM when the keyboard starts:
*/
//c
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

/*
---

#### 4. **Update HSV Data and Save to EEPROM**
Whenever HSV data for a key is updated, store the new values in the `key_hsv_data` array and save them to EEPROM.

Modify the `update_led_color` function to include EEPROM saving:
*/
//c
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

/*
---

#### 5. **Ensure Correct Memory Usage**
EEPROM has a finite number of write cycles. To minimize wear, write to EEPROM only when changes occur. The `eeprom_update_block` function checks if data is different before writing, reducing unnecessary writes.

---

#### 6. **Compile and Flash**
Compile and flash the firmware to include EEPROM support:
*/
//bash
qmk compile -kb <your_keyboard> -km <your_keymap>
qmk flash -kb <your_keyboard> -km <your_keymap>

/*
---

#### 7. **Test EEPROM Storage**
- Adjust HSV values for specific keys.
- Restart your macropad to verify that the settings persist.

---

### Additional Enhancements
- **Reset Functionality**: Add a keycode to reset all keys to default HSV values and save the defaults to EEPROM.
- **Validation**: On startup, validate the data in EEPROM to ensure it hasn’t been corrupted (e.g., add a checksum or magic number).
- **Efficiency**: Consider batching EEPROM writes if you frequently update multiple keys.

Let me know if you need help refining this approach or adding additional features!
*/

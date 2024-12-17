/* this is the last eeprom address already in eeconfig.h :
#define EECONFIG_RGBLIGHT_EXTENDED (uint8_t *)36






If the last used EEPROM address is `36` as defined in `eeconfig.h`,
you can allocate the next available space for your per-key HSV data.
Here's how to extend the EEPROM storage safely.

---

### Steps to Allocate EEPROM Space for Per-Key HSV

#### 1. **Determine Starting Address**
You can start storing your HSV data after the `EECONFIG_RGBLIGHT_EXTENDED` address. For example:
*/
//c
#define EEPROM_HSV_START 40 // Start storing per-key HSV data at address 40

/*
Adjust this starting point based on any additional reserved EEPROM addresses.

---

#### 2. **Calculate EEPROM Size for HSV Data**
Each key needs 3 bytes (1 for hue, 1 for saturation, and 1 for value). For a 4x6 matrix (24 keys), you’ll need:
*/

3 bytes/key * 24 keys = 72 bytes

/*
Ensure the total EEPROM usage does not exceed the available size on your microcontroller. For the RP2040, EEPROM is simulated in flash, so it has more capacity, but keep it organized and compact.

---

#### 3. **Update the EEPROM Read and Write Functions**
Modify the read and write functions to use the newly allocated space.
*/
//c
#include "eeprom.h"

#define MATRIX_ROWS 6
#define MATRIX_COLS 4
#define KEY_COUNT (MATRIX_ROWS * MATRIX_COLS)

#define EEPROM_HSV_START 40 // Starting address for HSV data

typedef struct {
    uint8_t hue;
    uint8_t sat;
    uint8_t val;
} key_hsv_t;

key_hsv_t key_hsv_data[KEY_COUNT];

// Function to load HSV data from EEPROM
void load_hsv_from_eeprom(void) {
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        eeprom_read_block((void *)&key_hsv_data[i], (const void *)(EEPROM_HSV_START + (i * sizeof(key_hsv_t))), sizeof(key_hsv_t));
    }
}

// Function to save HSV data to EEPROM
void save_hsv_to_eeprom(void) {
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        eeprom_update_block((const void *)&key_hsv_data[i], (void *)(EEPROM_HSV_START + (i * sizeof(key_hsv_t))), sizeof(key_hsv_t));
    }
}

/*
---

#### 4. **Integrate EEPROM Handling in the Keymap Code**
Update the `update_led_color` function to save changes to EEPROM:
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

#### 5. **Apply HSV Values on Startup**
During initialization, read the saved HSV values from EEPROM and apply them:
*/
//c
void keyboard_post_init_user(void) {
    // Load HSV data from EEPROM
    load_hsv_from_eeprom();

    // Apply saved HSV data to LEDs
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint16_t led_index = row * MATRIX_COLS + col;
            rgb_matrix_set_color(led_index, key_hsv_data[led_index].hue, key_hsv_data[led_index].sat, key_hsv_data[led_index].val);
        }
    }
}

/*
---

#### 6. **Reset Functionality**
Optionally, add a keycode to reset the HSV settings to default and store the defaults in EEPROM:
*/
//c
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

/*
Add the `KC_RESET_HSV` keycode to your keymap to trigger this reset functionality.

---

#### 7. **Compile and Test**
Compile and flash the firmware:
*/
//bash
qmk compile -kb <your_keyboard> -km <your_keymap>
qmk flash -kb <your_keyboard> -km <your_keymap>

/*
Test the functionality:
1. Adjust per-key HSV values.
2. Restart the keyboard to confirm that settings persist.
3. Use the reset keycode to revert to defaults.

---

### Summary
- **Start EEPROM Address**: Begin storing at address `40` (after `EECONFIG_RGBLIGHT_EXTENDED`).
- **EEPROM Allocation**: Reserve 72 bytes for a 4x6 matrix.
- **Functions**: Implement `load_hsv_from_eeprom`, `save_hsv_to_eeprom`, and apply HSV values on startup.
- **Customization**: Add reset functionality if needed.

Let me know if you need further assistance!
*/

// can we remove the hsv_to_rgb() as that conversion slows down the firmware.
// create a hsv_matrix_set_color(led_index, hue, sat, val)






// Creating a custom `hsv_matrix_set_color()` function allows for direct handling of HSV
// without relying on a separate conversion function every time. By precomputing and
// storing the RGB values when HSV is set, you can optimize performance while retaining
// the flexibility of HSV-based configuration.


// Implementation of `hsv_matrix_set_color()`

// 1. **Precompute and Store RGB Values**
//Instead of converting HSV to RGB on every call, precompute RGB values when
// HSV data is updated and store the result in a custom matrix. 

// Define the RGB matrix in addition to the HSV matrix.

// c
#include "rgb_matrix.h"

#define MATRIX_ROWS 6
#define MATRIX_COLS 4
#define KEY_COUNT (MATRIX_ROWS * MATRIX_COLS)

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

typedef struct {
    uint8_t hue;
    uint8_t sat;
    uint8_t val;
} hsv_t;

// Storage for HSV and RGB matrices
hsv_t hsv_matrix[KEY_COUNT];
rgb_t rgb_matrix[KEY_COUNT];

// EEPROM address for HSV data
#define EEPROM_HSV_START 40

// Functions to load and save HSV data from/to EEPROM
void load_hsv_from_eeprom(void);
void save_hsv_to_eeprom(void);


// 2. **Define `hsv_matrix_set_color()`**
// This function updates both the HSV matrix and the precomputed RGB matrix
// and immediately sets the LED color.

// c
void hsv_matrix_set_color(uint16_t led_index, uint8_t hue, uint8_t sat, uint8_t val) {
    if (led_index >= KEY_COUNT) return;

    // Update HSV matrix
    hsv_matrix[led_index].hue = hue;
    hsv_matrix[led_index].sat = sat;
    hsv_matrix[led_index].val = val;

    // Precompute RGB values and update RGB matrix
    rgb_config_t rgb = hsv_to_rgb((HSV) { hue, sat, val });
    rgb_matrix[led_index].r = rgb.r;
    rgb_matrix[led_index].g = rgb.g;
    rgb_matrix[led_index].b = rgb.b;

    // Update the LED color
    rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
}


// 3. **Optimize EEPROM Integration**
// The HSV matrix is loaded from and saved to EEPROM, ensuring persistence across reboots.

// c
void load_hsv_from_eeprom(void) {
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        eeprom_read_block((void *)&hsv_matrix[i], (const void *)(EEPROM_HSV_START + (i * sizeof(hsv_t))), sizeof(hsv_t));

        // Precompute RGB matrix
        rgb_config_t rgb = hsv_to_rgb((HSV) { hsv_matrix[i].hue, hsv_matrix[i].sat, hsv_matrix[i].val });
        rgb_matrix[i].r = rgb.r;
        rgb_matrix[i].g = rgb.g;
        rgb_matrix[i].b = rgb.b;
    }
}

void save_hsv_to_eeprom(void) {
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        eeprom_update_block((const void *)&hsv_matrix[i], (void *)(EEPROM_HSV_START + (i * sizeof(hsv_t))), sizeof(hsv_t));
    }
}


// 4. **Apply Settings at Startup**
// In your `keyboard_post_init_user` function, apply the precomputed RGB matrix to
// initialize the LEDs.

// c
void keyboard_post_init_user(void) {
    // Load HSV matrix from EEPROM
    load_hsv_from_eeprom();

    // Apply RGB values to LEDs
    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        rgb_matrix_set_color(i, rgb_matrix[i].r, rgb_matrix[i].g, rgb_matrix[i].b);
    }
}


// 5. **Update LED Colors Dynamically**
// Whenever a key’s HSV value changes, call `hsv_matrix_set_color()`:

// c
void update_key_hsv(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val) {
    uint16_t led_index = row * MATRIX_COLS + col;

    // Update HSV and RGB matrices, set LED color
    hsv_matrix_set_color(led_index, hue, sat, val);

    // Save changes to EEPROM
    save_hsv_to_eeprom();
}


// Benefits of This Approach
// 1. **Precomputed Efficiency**:
//    - RGB values are calculated once during updates, avoiding repeated conversions during LED refresh.
// 2. **Persistent Storage**:
//    - HSV settings persist across reboots via EEPROM integration.
// 3. **Modular Design**:
//    - `hsv_matrix_set_color()` simplifies updates and ensures consistency.


// Final Testing
// 1. **Compile and Flash**: 
//    Ensure everything compiles correctly and flash the firmware.
// 2. **Verify EEPROM Storage**:
//    - Change HSV settings for specific keys.
//    - Restart the keyboard to confirm persistence.
// 3. **Performance**:
//    - Confirm smooth operation without noticeable delays.

// Let me know if you need further refinements or assistance!

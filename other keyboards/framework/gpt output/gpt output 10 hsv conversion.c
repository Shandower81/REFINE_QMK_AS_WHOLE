// rgb_matrix_set_color() is coded so it is for rgb values not hsv.




// You're correct—`rgb_matrix_set_color()` uses RGB values, not HSV.
// To work with HSV data, you'll need to convert HSV to RGB before
// calling `rgb_matrix_set_color()`. QMK provides helper functions
// for this purpose in its RGB lighting system.



// Update: HSV to RGB Conversion

// Use QMK's built-in `hsv_to_rgb()` function to handle the conversion.
// Here's how to integrate it:

// 1. **Update `update_led_color` Function**

// Modify the `update_led_color` function to perform HSV-to-RGB conversion:

// c
#include "rgb_matrix.h" // Ensure you include this for the conversion functions

void update_led_color(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val) {
    uint16_t led_index = row * MATRIX_COLS + col;

    // Convert HSV to RGB
    rgb_config_t rgb = hsv_to_rgb((HSV) { hue, sat, val });

    // Set the LED color using RGB values
    rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);

    // Save the updated HSV values for the key
    key_hsv_data[led_index].hue = hue;
    key_hsv_data[led_index].sat = sat;
    key_hsv_data[led_index].val = val;

    // Save changes to EEPROM
    save_hsv_to_eeprom();
}



// 2. **Apply Saved HSV Values**

// When applying saved values during initialization, convert the HSV data
// to RGB before setting the color:

// c
void keyboard_post_init_user(void) {
    // Load saved HSV data from EEPROM
    load_hsv_from_eeprom();

    // Apply HSV settings to LEDs
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint16_t led_index = row * MATRIX_COLS + col;

            // Convert HSV to RGB
            rgb_config_t rgb = hsv_to_rgb((HSV) {
                key_hsv_data[led_index].hue,
                key_hsv_data[led_index].sat,
                key_hsv_data[led_index].val
            });

            // Set LED color
            rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
        }
    }
}


// 3. **Test the Implementation**

// After these changes, compile and test:
// 1. Adjust per-key HSV values using your keycodes.
// 2. Verify that colors display correctly after HSV-to-RGB conversion.
// 3. Confirm that settings persist across reboots (stored in EEPROM).


// Summary of Changes
// 1. **HSV-to-RGB Conversion**: Use `hsv_to_rgb()` for converting HSV to RGB.
// 2. **Integration**: Convert HSV values in `update_led_color` and during initialization.
// 3. **Storage**: Store HSV values in EEPROM, but convert them to RGB dynamically for LED updates.

// This approach ensures that you can work with HSV data internally while still using
// QMK’s RGB matrix functions for LED control. Let me know if you encounter any issues!

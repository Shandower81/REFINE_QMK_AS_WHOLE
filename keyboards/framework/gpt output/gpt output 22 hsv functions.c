// option 1.
// what about the ability to change the hsv values for foreground,
// background and the main hsv values for effects such as solid color
// with the same keycodes that you can use the .h, .s, .v functions
// but can use a matrix select key to decide which matrix will be updated?






// Adding the ability to change HSV values for foreground, background, and
// main effects dynamically, with matrix selection, requires a thoughtful
// approach. Here's how you can achieve this within **Option 1**, by
// integrating this functionality globally into the RGB Matrix system.


// Steps to Add HSV Adjustment Per Matrix

// 1. Extend the Effect Configuration
//    Enhance the RGB matrix logic to include the ability to dynamically
//    adjust HSV values for foreground and background matrices independently.
//    Use a "matrix select" key to decide which matrix will be updated.


// 1. Modify `rgb_matrix_effect_composite.c`
// This file will handle the custom effect and matrix selection.

// Add HSV Adjustment Logic
// c
#include "rgb_matrix.h"
#include "effects.h"

static uint8_t active_hsv_matrix = 0; // 0 = main, 1 = foreground, 2 = background

void set_active_hsv_matrix(uint8_t matrix_type) {
    if (matrix_type <= 2) {
        active_hsv_matrix = matrix_type;
    }
}

bool rgb_matrix_effect_composite(effect_params_t *params) {
    if (!rgb_matrix_config.enable) return false;

    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        composite_matrix_render(i);
    }

    return true;
}

bool hsv_adjust(uint8_t hue_delta, uint8_t sat_delta, uint8_t val_delta) {
    hsv_t *hsv_matrix;

    // Select the active HSV matrix
    switch (active_hsv_matrix) {
        case 1: hsv_matrix = hsv_foreground_matrix; break;
        case 2: hsv_matrix = hsv_background_matrix; break;
        default: return false; // Invalid matrix type
    }

    // Apply adjustments to the selected HSV matrix
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        hsv_matrix[i].hue += hue_delta;
        hsv_matrix[i].sat += sat_delta;
        hsv_matrix[i].val += val_delta;

        // Ensure the values stay within valid ranges
        hsv_matrix[i].hue %= 256;
        hsv_matrix[i].sat = hsv_matrix[i].sat > 255 ? 255 : hsv_matrix[i].sat;
        hsv_matrix[i].val = hsv_matrix[i].val > 255 ? 255 : hsv_matrix[i].val;

        // Precompute RGB values
        set_hsv_and_precompute_rgb(hsv_matrix, (active_hsv_matrix == 1 ? rgb_foreground_matrix : rgb_background_matrix), i,
                                   hsv_matrix[i].hue, hsv_matrix[i].sat, hsv_matrix[i].val);
    }

    return true;
}



// 2. Add New Keycodes
// Define custom keycodes to select the matrix and adjust HSV values in your `keymap.c`.

// Define Keycodes
// In your `keymap.c` or a separate header file:
// c
enum custom_keycodes {
    SEL_MAIN_MATRIX = SAFE_RANGE,
    SEL_FOREGROUND_MATRIX,
    SEL_BACKGROUND_MATRIX,
    HSV_HUE_UP,
    HSV_HUE_DOWN,
    HSV_SAT_UP,
    HSV_SAT_DOWN,
    HSV_VAL_UP,
    HSV_VAL_DOWN,
};


// Handle Keycodes
// In your `process_record_user()`:
// c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case SEL_MAIN_MATRIX:
                set_active_hsv_matrix(0);
                break;
            case SEL_FOREGROUND_MATRIX:
                set_active_hsv_matrix(1);
                break;
            case SEL_BACKGROUND_MATRIX:
                set_active_hsv_matrix(2);
                break;
            case HSV_HUE_UP:
                hsv_adjust(5, 0, 0); // Increase hue
                break;
            case HSV_HUE_DOWN:
                hsv_adjust(-5, 0, 0); // Decrease hue
                break;
            case HSV_SAT_UP:
                hsv_adjust(0, 5, 0); // Increase saturation
                break;
            case HSV_SAT_DOWN:
                hsv_adjust(0, -5, 0); // Decrease saturation
                break;
            case HSV_VAL_UP:
                hsv_adjust(0, 0, 5); // Increase value
                break;
            case HSV_VAL_DOWN:
                hsv_adjust(0, 0, -5); // Decrease value
                break;
            default:
                break;
        }
    }
    return true;
}



// 3. Register Custom Effect in the RGB Matrix System
// Follow the steps in **Option 1** to integrate the new effect into
// `quantum/rgb_matrix/animations`. Add the following to your custom effect:

// - Register the matrix selector and HSV adjuster.
// - Ensure your effect works independently for the foreground, background, or main matrix.


// 4. Assign Keycodes in the Keymap
// Update your keymap layer to include the custom matrix selection and HSV adjustment keycodes.

// c
[_LAYER] = LAYOUT(
    SEL_MAIN_MATRIX, SEL_FOREGROUND_MATRIX, SEL_BACKGROUND_MATRIX, HSV_HUE_UP,
    HSV_HUE_DOWN, HSV_SAT_UP, HSV_SAT_DOWN, HSV_VAL_UP,
    HSV_VAL_DOWN, XXXXXXX, XXXXXXX, XXXXXXX
),



// 5. Test and Debug
// Rebuild your firmware, flash it to your keyboard, and test the following:
// - Matrix Selection: Use `SEL_MAIN_MATRIX`, `SEL_FOREGROUND_MATRIX`, and
//   `SEL_BACKGROUND_MATRIX` to choose the active matrix.
// - HSV Adjustments: Use `HSV_HUE_UP`, `HSV_SAT_UP`, etc., to adjust the active matrix’s HSV values.


// Final Notes
// This approach dynamically adjusts the HSV values for each matrix type (foreground,
// background, or main) and integrates seamlessly into the RGB Matrix system. Let
// me know if you need further clarification or assistance in testing!

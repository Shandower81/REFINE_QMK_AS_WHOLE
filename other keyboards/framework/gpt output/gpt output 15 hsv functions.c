// yes show how to integrate this fully into the broader matrix handling logic.



// Here’s a complete integration of `rgb_matrix_get_current_effect()` into the
// broader matrix handling logic. This implementation ensures that color updates
// dynamically adapt to the currently active effect, determining whether to use
// the RGB, HSV, or HSV background matrix.


// Step 1: **Define the Effect-to-Matrix Mapping**

// Create a function to map the active RGB matrix effect to the appropriate
// color matrix type:

// c
#include "rgb_matrix.h"

// Map RGB effects to matrix types
effect_type_t get_effect_type(rgb_matrix_effect_t effect) {
    switch (effect) {
        case RGB_MATRIX_SOLID_COLOR:
        case RGB_MATRIX_RAINBOW_MOVING_CHEVRON:
        case RGB_MATRIX_CYCLE_ALL:
            return EFFECT_TYPE_RGB;

        case RGB_MATRIX_SOLID_REACTIVE_CROSS:
        case RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE:
            return EFFECT_TYPE_HSV;

        case RGB_MATRIX_BACKGROUND_COLOR:
            return EFFECT_TYPE_HSV_BACKGROUND;

        default:
            return EFFECT_TYPE_RGB; // Default to RGB
    }
}


// Step 2: **Enhance `color_matrix_set_color()`**

// Integrate the dynamic matrix type selection into the `color_matrix_set_color()` function:

// c
void color_matrix_set_color(uint16_t led_index, uint8_t p1, uint8_t p2, uint8_t p3) {
    if (led_index >= KEY_COUNT) return;

    // Get the current effect type
    rgb_matrix_effect_t current_effect = rgb_matrix_get_current_effect();
    effect_type_t effect_type = get_effect_type(current_effect);

    switch (effect_type) {
        case EFFECT_TYPE_RGB:
            // Update RGB matrix
            rgb_matrix[led_index] = (rgb_t){ .r = p1, .g = p2, .b = p3 };
            rgb_matrix_set_color(led_index, p1, p2, p3);
            break;

        case EFFECT_TYPE_HSV:
            // Update HSV matrix and compute RGB for immediate use
            hsv_matrix[led_index] = (hsv_t){ .hue = p1, .sat = p2, .val = p3 };
            rgb_t rgb = hsv_to_rgb(hsv_matrix[led_index]);
            rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
            break;

        case EFFECT_TYPE_HSV_BACKGROUND:
            // Update HSV background matrix and compute RGB for immediate use
            hsv_background_matrix[led_index] = (hsv_t){ .hue = p1, .sat = p2, .val = p3 };
            rgb_t bg_rgb = hsv_to_rgb(hsv_background_matrix[led_index]);
            rgb_matrix_set_color(led_index, bg_rgb.r, bg_rgb.g, bg_rgb.b);
            break;
    }
}


// Step 3: **Reactive Cross and Background Effects**

// Update the specific effects to use the enhanced `color_matrix_set_color()` function.

// Reactive Cross Effect
// c
void set_reactive_cross(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val) {
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        uint16_t led_index = r * MATRIX_COLS + col;
        color_matrix_set_color(led_index, hue, sat, val);
    }
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        uint16_t led_index = row * MATRIX_COLS + c;
        color_matrix_set_color(led_index, hue, sat, val);
    }
}


// Background Color Effect
// c
void set_background_color(uint8_t hue, uint8_t sat, uint8_t val) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        color_matrix_set_color(i, hue, sat, val);
    }
}


// Step 4: **Apply Colors During Initialization**

// Ensure the correct colors are applied at startup by integrating the logic into `keyboard_post_init_user`:

// c
void keyboard_post_init_user(void) {
    // Apply RGB matrix settings based on the stored values
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        rgb_t rgb = rgb_matrix[i]; // Default to RGB matrix
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
}


// Step 5: **Performance and Debugging**

// To ensure optimal performance:
// 1. **Test Effect Switching**: Confirm that switching effects dynamically
//    updates the correct matrix.
// 2. **Debug Logging** *(Optional)*:
//    Add debug prints to verify the active effect and matrix type:
// c
void debug_effect_type(void) {
    rgb_matrix_effect_t current_effect = rgb_matrix_get_current_effect();
    effect_type_t effect_type = get_effect_type(current_effect);
    dprintf("Current Effect: %d, Effect Type: %d\n", current_effect, effect_type);
}


// Call `debug_effect_type()` in a key combination or a debugging hook.

// Step 6: **Final Integration**

// Integrate `color_matrix_set_color()` throughout your custom effects.
// Use this function consistently to ensure all logic dynamically adapts
// to the active effect.

// Advantages
// - **Dynamic Handling**: Automatically determines the matrix type based on the current effect.
// - **Unified Logic**: One central function (`color_matrix_set_color()`) handles all color matrix updates.
// - **Future-Proof**: Adding new effects is simple—just update the `get_effect_type()` mapping.

// Let me know if you encounter any issues or need further clarification!

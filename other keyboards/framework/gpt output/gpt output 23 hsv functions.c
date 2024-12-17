//i gave a bad prompt.  the foreground hsv should be same as main hsv so do not
// need a different function for foreground hsv.  the background is the
// additional hsv values.  should be able to pick single color or other
// animations for background such as static rainbow or non-static animations
// or per-key-rgb.







// To revise the implementation based on your corrected prompt:

// - Foreground HSV will use the main HSV values. This simplifies the logic
//   since there's no need for separate handling of foreground HSV.
// - Background HSV will have independent settings, allowing for custom colors
//   or effects (like animations or static colors).
// - The background behavior can support static colors, static rainbow effects,
//   or dynamic animations, while the foreground (main HSV) continues to operate
//   as defined by the primary matrix effect.

// Here’s the adjusted implementation:


// Steps to Integrate the Revised Logic

// 1. Update `rgb_matrix_effect_composite.c`
// This file will now manage the main HSV for the foreground and independently
// handle the background HSV.

// c
#include "rgb_matrix.h"
#include "effects.h"

// Background HSV matrix, separate from the main (foreground) matrix
hsv_t hsv_background_matrix[DRIVER_LED_TOTAL];
rgb_t rgb_background_matrix[DRIVER_LED_TOTAL];

// Function to set background HSV (or animation configuration)
void set_background_hsv(uint8_t hue, uint8_t sat, uint8_t val) {
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        hsv_background_matrix[i].hue = hue;
        hsv_background_matrix[i].sat = sat;
        hsv_background_matrix[i].val = val;

        // Precompute the RGB values
        hsv_to_rgb(&hsv_background_matrix[i], &rgb_background_matrix[i]);
    }
}

// Example: Background animation (e.g., static rainbow effect)
void update_background_animation(void) {
    static uint8_t hue_offset = 0;
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        hsv_background_matrix[i].hue = (hue_offset + (i * 10)) % 256;
        hsv_background_matrix[i].sat = 255;
        hsv_background_matrix[i].val = 128;

        // Precompute the RGB values
        hsv_to_rgb(&hsv_background_matrix[i], &rgb_background_matrix[i]);
    }
    hue_offset += 1; // Gradually shift the hue for a dynamic rainbow effect
}

// Composite effect rendering
bool rgb_matrix_effect_composite(effect_params_t *params) {
    if (!rgb_matrix_config.enable) return false;

    // Render the foreground (main HSV)
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        rgb_matrix_set_color(i, rgb_foreground_matrix[i].r, rgb_foreground_matrix[i].g, rgb_foreground_matrix[i].b);
    }

    // Render the background (additive blending with foreground)
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        rgb_matrix_set_color(i, rgb_background_matrix[i].r, rgb_background_matrix[i].g, rgb_background_matrix[i].b);
    }

    return true;
}



// 2. Add Background Selection Keycodes
// Define keycodes for controlling the background mode and HSV values.

// c
enum custom_keycodes {
    BACKGROUND_MODE_STATIC = SAFE_RANGE,
    BACKGROUND_MODE_RAINBOW,
    BACKGROUND_MODE_ANIMATION,
    BACKGROUND_HUE_UP,
    BACKGROUND_HUE_DOWN,
    BACKGROUND_SAT_UP,
    BACKGROUND_SAT_DOWN,
    BACKGROUND_VAL_UP,
    BACKGROUND_VAL_DOWN,
};


// Handle Keycodes in `process_record_user`
// Adjust the background HSV and mode dynamically.

// c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case BACKGROUND_MODE_STATIC:
                set_background_hsv(128, 255, 128); // Example: Set static purple
                break;
            case BACKGROUND_MODE_RAINBOW:
                update_background_animation(); // Trigger rainbow animation
                break;
            case BACKGROUND_MODE_ANIMATION:
                // Custom logic for non-static animations
                break;
            case BACKGROUND_HUE_UP:
                set_background_hsv((hsv_background_matrix[0].hue + 5) % 256,
                                   hsv_background_matrix[0].sat,
                                   hsv_background_matrix[0].val);
                break;
            case BACKGROUND_HUE_DOWN:
                set_background_hsv((hsv_background_matrix[0].hue - 5) % 256,
                                   hsv_background_matrix[0].sat,
                                   hsv_background_matrix[0].val);
                break;
            case BACKGROUND_SAT_UP:
                set_background_hsv(hsv_background_matrix[0].hue,
                                   MIN(hsv_background_matrix[0].sat + 5, 255),
                                   hsv_background_matrix[0].val);
                break;
            case BACKGROUND_SAT_DOWN:
                set_background_hsv(hsv_background_matrix[0].hue,
                                   MAX(hsv_background_matrix[0].sat - 5, 0),
                                   hsv_background_matrix[0].val);
                break;
            case BACKGROUND_VAL_UP:
                set_background_hsv(hsv_background_matrix[0].hue,
                                   hsv_background_matrix[0].sat,
                                   MIN(hsv_background_matrix[0].val + 5, 255));
                break;
            case BACKGROUND_VAL_DOWN:
                set_background_hsv(hsv_background_matrix[0].hue,
                                   hsv_background_matrix[0].sat,
                                   MAX(hsv_background_matrix[0].val - 5, 0));
                break;
        }
    }
    return true;
}



// 3. Add Effect to RGB Matrix
// Follow the steps in **Option 1** to integrate the `rgb_matrix_effect_composite` into the system.


// 4. Assign Keycodes in Keymap
// In your `keymap.c`, assign the new keycodes for background control:

// c
[_LAYER] = LAYOUT(
    BACKGROUND_MODE_STATIC, BACKGROUND_MODE_RAINBOW, BACKGROUND_HUE_UP, BACKGROUND_HUE_DOWN,
    BACKGROUND_SAT_UP, BACKGROUND_SAT_DOWN, BACKGROUND_VAL_UP, BACKGROUND_VAL_DOWN, XXXXXXX
);



// 5. Dynamic Background Selection
// Integrate dynamic selection of background animations in `rgb_matrix_effect_composite.c`.

// - Use an enum to store the current background mode.
// - Call the appropriate update function based on the mode.

// c
enum background_modes {
    BACKGROUND_STATIC,
    BACKGROUND_RAINBOW,
    BACKGROUND_ANIMATION
};

static enum background_modes current_background_mode = BACKGROUND_STATIC;

void update_background_mode(void) {
    switch (current_background_mode) {
        case BACKGROUND_STATIC:
            set_background_hsv(128, 255, 128); // Example: Static purple
            break;
        case BACKGROUND_RAINBOW:
            update_background_animation();
            break;
        case BACKGROUND_ANIMATION:
            // Call other custom animation functions
            break;
    }
}


// Call `update_background_mode()` in your matrix scan logic to keep animations dynamic.


// Summary
// - Foreground HSV uses the main HSV logic; no additional functions are needed.
// - Background HSV supports independent colors or animations (like rainbow effects).
// - Keycodes allow users to select background modes and adjust HSV values dynamically.
// - The `rgb_matrix_effect_composite` manages both foreground and background rendering.

// Would you like to test or refine specific animations or background modes?

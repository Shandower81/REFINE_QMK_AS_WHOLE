// the reactive effects have a color for the response to key press and a second
// color that is either black or something like the rainbow matrix colors for
// background color.  the RGB_MATRIX_SOLID_REACTIVE_CROSS and others have both.
// so how can background color be a separate rgb effect type?



// You 're absolutely correct—reactive effects like `RGB_MATRIX_SOLID_REACTIVE_CROSS`
// involve both a foreground color (the color that appears when a key is pressed)
// and a background color (which might be static, dynamic, or a result of another
// effect). This means the background color isn' t truly a separate effect type;
// instead, it's an integral part of the effect itself.

// To handle this, the color matrix logic must support **layered effects** where:
// 1. **Foreground**: Reacts dynamically to input, e.g., key presses.
// 2. **Background**: Displays independently of input, e.g., rainbow waves or a static background color.

// Here’s how to adapt the design to incorporate both foreground and background behavior:

// 1. **Dual-Matrix System**
// We treat the reactive effects as foreground actions layered on top of a background effect.

// - **Background Matrix**: Maintains the HSV or RGB values for the static/dynamic background.
// - **Foreground Matrix**: Reacts dynamically to input.

// Define these matrices explicitly:
// c
hsv_t hsv_background_matrix[KEY_COUNT]; // Background layer
hsv_t hsv_foreground_matrix[KEY_COUNT]; // Foreground layer




// 2. * *Composite Rendering Function **Instead of choosing between HSV or RGB,
//   render the LED colors as a combination of the foreground and background matrices.Use foreground colors where active;
//   otherwise, fall back to the background matrix.

// c
void composite_matrix_render(uint16_t led_index) {
    if (led_index >= KEY_COUNT) return;

    hsv_t fg_color = hsv_foreground_matrix[led_index];
    hsv_t bg_color = hsv_background_matrix[led_index];

    // Check if foreground is active (non-zero value)
    if (fg_color.val > 0) {
        rgb_t rgb = hsv_to_rgb(fg_color);
        rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
    } else {
        rgb_t rgb = hsv_to_rgb(bg_color);
        rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
    }
}


// 3. **Reactive Cross Example** Update the `set_reactive_cross()` function
// to handle both the foreground and background :

// c
void set_reactive_cross(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val, uint8_t bg_hue, uint8_t bg_sat, uint8_t bg_val) {
    // Set the foreground cross
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        uint16_t led_index               = r * MATRIX_COLS + col;
        hsv_foreground_matrix[led_index] = (hsv_t){.hue = hue, .sat = sat, .val = val};
    }
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        uint16_t led_index               = row * MATRIX_COLS + c;
        hsv_foreground_matrix[led_index] = (hsv_t){.hue = hue, .sat = sat, .val = val};
    }

    // Set the background color for the rest
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        hsv_background_matrix[i] = (hsv_t){.hue = bg_hue, .sat = bg_sat, .val = bg_val};
    }

    // Render the updated colors
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        composite_matrix_render(i);
    }
}


// 4. **Dynamic Background Effects **For dynamic effects like a rainbow background,
// update the `hsv_background_matrix` periodically(e.g., in a timer interrupt or task loop).

// Rainbow Background
// c
void update_rainbow_background(void) {
    static uint8_t hue = 0;
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        hsv_background_matrix[i] = (hsv_t){.hue = (hue + i * 10) % 256, .sat = 255, .val = 128};
    }
    hue = (hue + 1) % 256; // Advance hue
}



// 5. **Integration with RGB Matrix Effects** Update the main RGB matrix rendering loop
// to combine the foreground and background :

// c
void rgb_matrix_indicators_user(void) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        composite_matrix_render(i);
    }
}


// 6. **Clearing Foreground Effects** Reactive effects should fade out after a key press.
// Use a decay mechanism for the foreground matrix:

// c
void decay_foreground_effects(void) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        hsv_t* fg = &hsv_foreground_matrix[i];
        if (fg->val > 0) {
            fg->val = fg->val > 10 ? fg->val - 10 : 0; // Gradually reduce brightness
        }
    }
}


//    Call this function periodically, e.g., in a timer task.


// Summary This design achieves :
//   1. **Foreground and Background Separation ** : Foreground handles reactive effects,
//      while the background supports dynamic or static effects.
//   2. **Dynamic Layer Composition ** : LEDs render as a composite of the active
//      foreground and background colors.
//   3. **Performance Optimization ** : HSV is converted to RGB only once per render cycle, minimizing                                                                                                                                                                                                                                              overhead.

// Let me know if you'd like further clarifications or enhancements!

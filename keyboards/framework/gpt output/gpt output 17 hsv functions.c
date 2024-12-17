// precompute HSV to RGB so that no runtime conversions that will slow down the firmware.






// To optimize performance by avoiding runtime conversions, you can precompute the HSV-to-RGB
// values during matrix updates and store them in a separate RGB matrix. Here's how to achieve this:


// 1. **Precomputed RGB Storage**  Add a dedicated RGB storage matrix for both foreground and background layers:

// c
rgb_t rgb_foreground_matrix[KEY_COUNT]; // Precomputed RGB for foreground
rgb_t rgb_background_matrix[KEY_COUNT]; // Precomputed RGB for background



// 2. **Update HSV and Precompute RGB **When updating HSV values, convert them to RGB immediately
//    and store the results in the respective RGB matrix.

// Update Function
// c
void set_hsv_and_precompute_rgb(hsv_t* hsv_matrix, rgb_t* rgb_matrix, uint16_t led_index, uint8_t hue, uint8_t sat, uint8_t val) {
    if (led_index >= KEY_COUNT) return;

    // Update HSV
    hsv_matrix[led_index] = (hsv_t){.hue = hue, .sat = sat, .val = val};

    // Precompute RGB
    rgb_matrix[led_index] = hsv_to_rgb(hsv_matrix[led_index]);
}


// 3. **Foreground and Background Updates** Use the precompute function to handle both foreground and background updates.

// Example : Reactive Cross
// c
void set_reactive_cross(uint8_t row, uint8_t col, uint8_t fg_hue, uint8_t fg_sat, uint8_t fg_val, uint8_t bg_hue, uint8_t bg_sat, uint8_t bg_val) {
    // Update foreground (cross)
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        uint16_t led_index = r * MATRIX_COLS + col;
        set_hsv_and_precompute_rgb(hsv_foreground_matrix, rgb_foreground_matrix, led_index, fg_hue, fg_sat, fg_val);
    }
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        uint16_t led_index = row * MATRIX_COLS + c;
        set_hsv_and_precompute_rgb(hsv_foreground_matrix, rgb_foreground_matrix, led_index, fg_hue, fg_sat, fg_val);
    }

    // Update background
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        set_hsv_and_precompute_rgb(hsv_background_matrix, rgb_background_matrix, i, bg_hue, bg_sat, bg_val);
    }
}


// 4. **Composite Render Function** Render the LED colors using the precomputed RGB values :

// c
void composite_matrix_render(uint16_t led_index) {
    if (led_index >= KEY_COUNT) return;

    // Use precomputed RGB values
    if (hsv_foreground_matrix[led_index].val > 0) {
        rgb_t rgb = rgb_foreground_matrix[led_index];
        rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
    } else {
        rgb_t rgb = rgb_background_matrix[led_index];
        rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
    }
}


// 5. **Dynamic Effects **For dynamic effects like a rainbow background, update the HSV matrix
//    and recompute RGB at the same time :

// c
void update_rainbow_background(void) {
    static uint8_t hue = 0;
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        uint8_t dynamic_hue = (hue + i * 10) % 256;
        set_hsv_and_precompute_rgb(hsv_background_matrix, rgb_background_matrix, i, dynamic_hue, 255, 128);
    }
    hue = (hue + 1) % 256; // Advance hue
}


// 6. **Decaying Foreground Effects **For reactive effects that fade over time,
//    adjust the HSV matrix, then update the RGB matrix :

// c
void decay_foreground_effects(void) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        hsv_t* fg_hsv = &hsv_foreground_matrix[i];
        if (fg_hsv->val > 0) {
            fg_hsv->val              = fg_hsv->val > 10 ? fg_hsv->val - 10 : 0;
            rgb_foreground_matrix[i] = hsv_to_rgb(*fg_hsv); // Update precomputed RGB
        }
    }
}


// 7. **Integration** Update the main rendering loop and dynamic tasks :

// Main Render Loop
// c
void rgb_matrix_indicators_user(void) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        composite_matrix_render(i);
    }
}


// Periodic Tasks
// Add tasks for dynamic updates and decay:
// c
void matrix_scan_user(void) {
    static uint32_t last_update = 0;
    if (timer_elapsed32(last_update) > 20) { // Update every 20ms
        update_rainbow_background();
        decay_foreground_effects();
        last_update = timer_read32();
    }
}


// Key Benefits
// - **Zero Runtime Conversions** : HSV - to - RGB is done once during updates, not during rendering.
// - **Optimized Rendering * * : Rendering uses precomputed RGB values, minimizing computational overhead.
// - **Flexible Effects** : Supports layered foreground and background with precomputed efficiency.

// Let me know if you’d like further assistance implementing this into your project !

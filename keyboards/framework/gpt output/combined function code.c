// c
#include "rgb_matrix.h"

// Add this function to return the current effect
rgb_matrix_effect_t rgb_matrix_get_current_effect(void) {
    return rgb_matrix_config.mode;
}




rgb_t rgb_foreground_matrix[KEY_COUNT]; // Precomputed RGB for foreground
rgb_t rgb_background_matrix[KEY_COUNT]; // Precomputed RGB for background






// c
void set_hsv_and_precompute_rgb(hsv_t* hsv_matrix, rgb_t* rgb_matrix, uint16_t led_index, uint8_t hue, uint8_t sat, uint8_t val) {
    if (led_index >= KEY_COUNT) return;

    // Update HSV
    hsv_matrix[led_index] = (hsv_t){.hue = hue, .sat = sat, .val = val};

    // Precompute RGB
    rgb_matrix[led_index] = hsv_to_rgb(hsv_matrix[led_index]);
}






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



// c
void update_rainbow_background(void) {
    static uint8_t hue = 0;
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        uint8_t dynamic_hue = (hue + i * 10) % 256;
        set_hsv_and_precompute_rgb(hsv_background_matrix, rgb_background_matrix, i, dynamic_hue, 255, 128);
    }
    hue = (hue + 1) % 256; // Advance hue
}



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


// c
void rgb_matrix_indicators_user(void) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        composite_matrix_render(i);
    }
}


// c
void matrix_scan_user(void) {
    static uint32_t last_update = 0;
    if (timer_elapsed32(last_update) > 20) { // Update every 20ms
        update_rainbow_background();
        decay_foreground_effects();
        last_update = timer_read32();
    }
}


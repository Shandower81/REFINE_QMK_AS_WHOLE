// yes, modularize




// To modularize the functionality, we will split the code into separate files
// for better maintainability. Here's how to organize the files and their contents:


// 1. Create New Files for Effects
// In your keyboard's folder (`keyboards/<your_keyboard>/`), create the following files:
// - `effects.c`
// - `effects.h`


// 2. File Structure Overview

// `effects.c`
// This file contains the implementation of all functions related to HSV precomputation,
// reactive effects, and background updates.

// `effects.h`
// This header file declares the functions and variables used in `effects.c`.

// `keymap.c`
// The main `keymap.c` file will only invoke these modularized functions, keeping it clean.

// 3. Code for `effects.h`
// c
#ifndef EFFECTS_H
#define EFFECTS_H

#include "quantum.h"

// Number of keys in the matrix
#define KEY_COUNT (MATRIX_ROWS * MATRIX_COLS)

// HSV and RGB matrices
extern hsv_t hsv_foreground_matrix[KEY_COUNT];
extern hsv_t hsv_background_matrix[KEY_COUNT];
extern rgb_t rgb_foreground_matrix[KEY_COUNT];
extern rgb_t rgb_background_matrix[KEY_COUNT];

// Function prototypes
void set_hsv_and_precompute_rgb(hsv_t *hsv_matrix, rgb_t *rgb_matrix, uint16_t led_index, uint8_t hue, uint8_t sat, uint8_t val);
void composite_matrix_render(uint16_t led_index);
void set_reactive_cross(uint8_t row, uint8_t col, uint8_t fg_hue, uint8_t fg_sat, uint8_t fg_val, uint8_t bg_hue, uint8_t bg_sat, uint8_t bg_val);
void update_rainbow_background(void);
void decay_foreground_effects(void);

#endif // EFFECTS_H


//4. Code for `effects.c`
// c
#include "effects.h"

// Precomputed matrices
hsv_t hsv_foreground_matrix[KEY_COUNT];
hsv_t hsv_background_matrix[KEY_COUNT];
rgb_t rgb_foreground_matrix[KEY_COUNT];
rgb_t rgb_background_matrix[KEY_COUNT];

void set_hsv_and_precompute_rgb(hsv_t *hsv_matrix, rgb_t *rgb_matrix, uint16_t led_index, uint8_t hue, uint8_t sat, uint8_t val) {
    if (led_index >= KEY_COUNT) return;

    // Update HSV
    hsv_matrix[led_index] = (hsv_t){ .hue = hue, .sat = sat, .val = val };

    // Precompute RGB
    rgb_matrix[led_index] = hsv_to_rgb(hsv_matrix[led_index]);
}

void composite_matrix_render(uint16_t led_index) {
    if (led_index >= KEY_COUNT) return;

    if (hsv_foreground_matrix[led_index].val > 0) {
        rgb_t rgb = rgb_foreground_matrix[led_index];
        rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
    } else {
        rgb_t rgb = rgb_background_matrix[led_index];
        rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
    }
}

void set_reactive_cross(uint8_t row, uint8_t col, uint8_t fg_hue, uint8_t fg_sat, uint8_t fg_val, uint8_t bg_hue, uint8_t bg_sat, uint8_t bg_val) {
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        uint16_t led_index = r * MATRIX_COLS + col;
        set_hsv_and_precompute_rgb(hsv_foreground_matrix, rgb_foreground_matrix, led_index, fg_hue, fg_sat, fg_val);
    }
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        uint16_t led_index = row * MATRIX_COLS + c;
        set_hsv_and_precompute_rgb(hsv_foreground_matrix, rgb_foreground_matrix, led_index, fg_hue, fg_sat, fg_val);
    }

    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        set_hsv_and_precompute_rgb(hsv_background_matrix, rgb_background_matrix, i, bg_hue, bg_sat, bg_val);
    }
}

void update_rainbow_background(void) {
    static uint8_t hue = 0;
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        uint8_t dynamic_hue = (hue + i * 10) % 256;
        set_hsv_and_precompute_rgb(hsv_background_matrix, rgb_background_matrix, i, dynamic_hue, 255, 128);
    }
    hue = (hue + 1) % 256;
}

void decay_foreground_effects(void) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        hsv_t *fg_hsv = &hsv_foreground_matrix[i];
        if (fg_hsv->val > 0) {
            fg_hsv->val = fg_hsv->val > 10 ? fg_hsv->val - 10 : 0;
            rgb_foreground_matrix[i] = hsv_to_rgb(*fg_hsv);
        }
    }
}


// 5. Changes to `keymap.c`
// In `keymap.c`, include the effects module and call the appropriate functions.

// Include the Header
// At the top of `keymap.c`, include `effects.h`:
// c
#include "effects.h"


// Override `rgb_matrix_indicators_user`
// c
void rgb_matrix_indicators_user(void) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        composite_matrix_render(i);
    }
}


// Update `matrix_scan_user`
// c
void matrix_scan_user(void) {
    static uint32_t last_update = 0;
    if (timer_elapsed32(last_update) > 20) { // Update every 20ms
        update_rainbow_background();
        decay_foreground_effects();
        last_update = timer_read32();
    }
}


// 6. Update `rules.mk`
// Add the new `effects.c` file to your build by updating `rules.mk` in your keyboard’s folder:
// makefile
SRC += effects.c


// Final Notes
// - With this structure, you can keep all effect-related logic in `effects.c` while leaving `keymap.c` clean and focused on key definitions.
// - Adjust paths as needed for your specific keyboard.
// - If you'd like further assistance with testing or refining this, let me know!

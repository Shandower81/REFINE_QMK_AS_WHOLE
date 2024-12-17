/* Key configuration and includes */
#include "quantum.h"
#include "rgb_matrix.h"
#include "rgb_matrix_types.h"

/* Global variables */
static uint8_t  active_mode                     = COLOR_MATRIX_FOREGROUND; // COLOR_MATRIX_FOREGROUND or COLOR_MATRIX_BACKGROUND
static uint8_t  current_effect_index_foreground = 0;                       // Foreground index
static uint8_t  current_effect_index_background = 0;                       // Background index
static uint16_t input_digit_timer               = 0;                       // Timer for direct input single digit
static uint8_t  first_digit                     = 0xFF;                    // Placeholder for the first digit in direct input mode

/* Apply effects to all non-numeric keys */
void apply_effect_to_non_numeric_keys(uint8_t effect_index, uint8_t mode) {
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t led_index = g_led_config.matrix_co[row][col];

            if (led_index == NO_LED || is_numeric_key(row, col)) {
                continue;
            }

            if (mode == COLOR_MATRIX_FOREGROUND) {
                rgb_matrix_set_color_all_from_effect(effect_index, COLOR_MATRIX_FOREGROUND);
            } else if (mode == COLOR_MATRIX_BACKGROUND) {
                rgb_matrix_set_color_all_from_effect(effect_index, COLOR_MATRIX_BACKGROUND);
            }
        }
    }
}

/* Check if the key is numeric */
bool is_numeric_key(uint8_t row, uint8_t col) {
    return (row == 0 && col >= 0 && col <= 9); // Assuming the numeric keys are in row 0, columns 0-9
}

/* Render numeric keys for direct input mode */
void render_numeric_keys_direct_input(uint8_t mode, uint8_t first_digit, uint8_t second_digit) {
    for (uint8_t i = 0; i <= 9; ++i) {
        uint8_t led_index = g_led_config.matrix_co[0][i];
        if (led_index == NO_LED) {
            continue;
        }

        if (mode == COLOR_MATRIX_FOREGROUND) {
            if (i == first_digit) {
                rgb_matrix_set_color(led_index, 0x00, 0x00, 0xFF); // Blue for tens place
            } else if (i == second_digit) {
                rgb_matrix_set_color(led_index, 0xFF, 0x00, 0x00); // Red for units place
            } else {
                rgb_matrix_set_color(led_index, 0x00, 0x00, 0x00); // Off for other keys
            }
        } else if (mode == COLOR_MATRIX_BACKGROUND) {
            if (i == first_digit || i == second_digit) {
                rgb_matrix_set_color(led_index, 0x00, 0x00, 0x00); // Off for active digits
            } else if (first_digit == second_digit) {
                // Blink all other keys alternating tens and units colors for repeated digits
                rgb_matrix_slow_blink_color(led_index, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00);
            } else {
                // Proximity-based colors for non-repeating digits
                uint8_t proximity_color[3];
                calculate_proximity_color(i, first_digit, second_digit, proximity_color);
                rgb_matrix_set_color(led_index, proximity_color[0], proximity_color[1], proximity_color[2]);
            }
        }
    }
}

/* Main rendering logic */
void render_current_effect(uint8_t effect_index, uint8_t mode) {
    if (mode == COLOR_MATRIX_FOREGROUND) {
        render_numeric_keys_direct_input(COLOR_MATRIX_FOREGROUND, first_digit, effect_index % 10);
        apply_effect_to_non_numeric_keys(effect_index, COLOR_MATRIX_FOREGROUND);
    } else if (mode == COLOR_MATRIX_BACKGROUND) {
        render_numeric_keys_direct_input(COLOR_MATRIX_BACKGROUND, effect_index / 10, effect_index % 10);
        apply_effect_to_non_numeric_keys(effect_index, COLOR_MATRIX_BACKGROUND);
    }
}

/* Direct input mode */
void handle_direct_input(uint8_t keycode) {
    if (first_digit == 0xFF) {
        first_digit       = keycode - KC_1;
        input_digit_timer = timer_read();
    } else {
        uint8_t second_digit = keycode - KC_1;
        uint8_t effect_index = first_digit * 10 + second_digit;

        render_current_effect(effect_index, active_mode);
        first_digit = 0xFF; // Reset for next input
    }
}

/* Handle step mode */
void handle_step_mode(bool forward) {
    uint8_t *current_index = (active_mode == COLOR_MATRIX_FOREGROUND) ? &current_effect_index_foreground : &current_effect_index_background;
    *current_index         = (*current_index + (forward ? 1 : -1)) % EFFECT_COUNT;

    render_current_effect(*current_index, active_mode);
}

/* Timer-based operations */
void handle_direct_input_timer() {
    if (first_digit != 0xFF && timer_elapsed(input_digit_timer) > INPUT_TIMEOUT) {
        render_current_effect(first_digit, active_mode);
        first_digit = 0xFF;
    }
}

/* Proximity-based color calculation */
void calculate_proximity_color(uint8_t key, uint8_t tens, uint8_t units, uint8_t *color_out) {
    int dist_tens  = abs(tens - key);
    int dist_units = abs(units - key);

    if (dist_tens == dist_units) {
        color_out[0] = (0x00 + 0xFF) / 2; // Blend Red and Blue
        color_out[1] = 0x00;
        color_out[2] = (0xFF + 0x00) / 2; // Blend Blue and Red
    } else if (dist_tens < dist_units) {
        color_out[0] = 0x00; // Blue
        color_out[1] = 0x00;
        color_out[2] = 0xFF;
    } else {
        color_out[0] = 0xFF; // Red
        color_out[1] = 0x00;
        color_out[2] = 0x00;
    }
}

/* Integration into QMK core */
void matrix_scan_user(void) {
    handle_direct_input_timer();
}

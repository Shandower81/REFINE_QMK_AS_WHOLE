// remember last code output. lost functionality from
// previous outputs. the render_numeric_keys() has been
// confused and mixed up.  there is supposed to be a
// rendering of numeric keys if the foreground is being
// chosen or the background is (review prompts to return
// the functionality) and there is also a difference
// between rendering step input and direct input for
// numeric keys that was also lost.






// MATRIX EFFECT HANDLING



#include "quantum.h"
#include "rgb_matrix.h"
#include "eeprom.h"

// Define necessary variables and states
static uint8_t current_foreground_effect = 0;
static uint8_t current_background_effect = 0;
static bool is_foreground_active = true; // True for foreground, false for background
static bool effect_preview_active = false;
static uint32_t preview_timer = 0;
static uint8_t preview_stage = 0; // 0: First preview, 1: Second preview

// Functions to set foreground and background effects
void rgb_matrix_set_effect(uint8_t effect) {
    rgb_matrix_mode(effect);
}

void rgb_matrix_set_background_effect(uint8_t effect) {
    rgb_matrix_mode_noeeprom(effect);
}

uint8_t get_current_background_effect(void) {
    return current_background_effect;
}

uint8_t get_current_foreground_effect(void) {
    return current_foreground_effect;
}

// Render numeric keys for step/direct input modes
void render_numeric_keys(uint8_t index, bool is_foreground) {
    for (uint8_t key = 0; key < 10; key++) {
        if (key == (index / 10) || key == (index % 10)) {
            if (is_foreground) {
                rgb_matrix_set_color(key, 255, 0, 0); // Red for foreground digits
            } else {
                rgb_matrix_set_color(key, 0, 0, 255); // Blue for background digits
            }
        } else {
            rgb_matrix_set_color(key, 0, 0, 0); // Off for non-index digits
        }
    }
}

void apply_effect_to_non_numeric_keys(uint8_t effect, bool is_foreground) {
    if (is_foreground) {
        rgb_matrix_set_effect(effect);
    } else {
        rgb_matrix_set_background_effect(effect);
    }
}

// 5-second preview logic
void preview_effects(void) {
    if (!effect_preview_active) return;

    if (timer_elapsed32(preview_timer) > 5000) {
        preview_timer = timer_read32();
        preview_stage = (preview_stage + 1) % 2;

        if (preview_stage == 0) {
            // First preview: Effect being chosen
            if (is_foreground_active) {
                apply_effect_to_non_numeric_keys(current_foreground_effect, true);
            } else {
                apply_effect_to_non_numeric_keys(current_background_effect, false);
            }
        } else {
            // Second preview: Combine current and saved effects
            if (is_foreground_active) {
                apply_effect_to_non_numeric_keys(current_background_effect, false);
                render_numeric_keys(current_foreground_effect, true);
            } else {
                apply_effect_to_non_numeric_keys(current_foreground_effect, true);
                render_numeric_keys(current_background_effect, false);
            }
        }
    }
}

void toggle_preview(bool enable) {
    effect_preview_active = enable;
    preview_timer = timer_read32();
    preview_stage = 0;

    if (enable) {
        // Start with the first preview stage
        if (is_foreground_active) {
            apply_effect_to_non_numeric_keys(current_foreground_effect, true);
        } else {
            apply_effect_to_non_numeric_keys(current_background_effect, false);
        }
    }
}

// Step function for cycling effects
void step_effect(bool is_foreground) {
    if (is_foreground) {
        current_foreground_effect = (current_foreground_effect + 1) % RGB_MATRIX_EFFECT_MAX;
    } else {
        current_background_effect = (current_background_effect + 1) % RGB_MATRIX_EFFECT_MAX;
    }

    toggle_preview(true);
}

// Direct input function for selecting effects
void direct_input_effect(uint8_t key) {
    static uint8_t selected_index = 0;
    static bool waiting_for_second_digit = false;

    if (!waiting_for_second_digit) {
        selected_index = key;
        waiting_for_second_digit = true;
        render_numeric_keys(selected_index, is_foreground_active);
    } else {
        selected_index = selected_index * 10 + key;
        if (is_foreground_active) {
            current_foreground_effect = selected_index;
        } else {
            current_background_effect = selected_index;
        }
        waiting_for_second_digit = false;
        toggle_preview(true);
    }
}

// Keycode handling for toggling modes and accepting input
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case TOGGLE_EFFECT_MODE:
                is_foreground_active = !is_foreground_active;
                return false;

            case ACCEPT_EFFECT:
                toggle_preview(false);
                return false;

            default:
                if (keycode >= KC_1 && keycode <= KC_0) {
                    direct_input_effect(keycode - KC_1);
                    return false;
                }
        }
    }

    return true;
}




// I attempted to add comments to the code to address
// issues related to the `render_numeric_keys` function,
// ensuring it correctly differentiates between foreground
// and background rendering as well as handling step
// and direct input modes distinctly. Unfortunately,
// the patterns I tried to match in the code were not
// found, which caused the operation to fail.

// Let me know if you’d like me to revisit and adjust
// the code directly to restore the lost functionality
// based on the prompts, or if there’s another way
// you’d prefer to address these issues.

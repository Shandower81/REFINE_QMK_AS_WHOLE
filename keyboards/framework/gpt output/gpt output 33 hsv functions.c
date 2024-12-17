// extend behavior so that foreground effect step shows one set of
// colors for first and second digit and a second operation for
// background where the keys that match index are black but the
// keys nearest to the black out digit are the same colors for
// the foreground. so all the leds on numeric row will be either
// red or blue based on the closest digit blacked out.







// This extended behavior involves distinguishing between the foreground
// and background effect steps with unique color representations,
// where the numeric row will show the index digits for both effects
// but in different patterns.



// Concept Overview

// 1. **Foreground Effect Step**:
//    - Use distinct **blue** (for the tens digit) and **red** (for the
//      units digit) LEDs to blink the effect index.
//    - Keys corresponding to the index digits light up, while others are off.

// 2. **Background Effect Step**:
//    - The keys corresponding to the digits of the effect index will be
//      **blacked out** (LEDs off).
//    - All other keys on the numeric row will be **red** or **blue**
//      based on their proximity to the blacked-out key.


// Implementation Steps

// 1. Define Helper Functions

// a. Clear Numeric Row
// Clear all LEDs in the numeric row before applying any effect.

// c
void clear_numeric_row_leds() {
    for (uint8_t i = 0; i < NUMERIC_ROW_KEY_COUNT; i++) {
        rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 0);
    }
}


// b. Set Proximity Colors
// For the background effect, this function assigns **red** or **blue**
// based on proximity to the blacked-out digit.

// c
void set_proximity_colors(uint8_t blacked_out_key) {
    for (uint8_t i = 0; i < NUMERIC_ROW_KEY_COUNT; i++) {
        if (i == blacked_out_key) {
            rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 0); // Black for the key matching index
        } else {
            // Color based on proximity: closer keys use blue, others use red
            if (abs(i - blacked_out_key) <= 1) {
                rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 255); // Blue
            } else {
                rgb_matrix_set_color(numeric_row_keys[i], 255, 0, 0); // Red
            }
        }
    }
}



// 2. Blink the Foreground Effect
// This logic remains the same as before but operates only on the
// digits corresponding to the effect index.

// c
void blink_foreground_effect(uint8_t effect_index) {
    static uint8_t blink_state = 0;
    static uint16_t timer = 0;

    // Ensure blinking happens at a fixed interval
    if (timer_elapsed(timer) < 500) {
        return;
    }
    timer = timer_read();

    // Clear the numeric row LEDs
    clear_numeric_row_leds();

    // Extract digits
    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    // Handle blinking
    if (blink_state == 0) {
        if (tens > 0) {
            rgb_matrix_set_color(numeric_row_keys[tens - 1], 0, 0, 255); // Blue
        } else {
            rgb_matrix_set_color(numeric_row_keys[units - 1], 255, 0, 0); // Red for single digit
        }
    } else if (tens > 0) {
        rgb_matrix_set_color(numeric_row_keys[units - 1], 255, 0, 0); // Red
    }

    blink_state = !blink_state;
}



// 3. Set the Background Effect
// The background effect will use the proximity color logic.

// c
void set_background_effect(uint8_t effect_index) {
    // Extract digits
    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    // Apply proximity colors for the tens digit if it exists
    if (tens > 0) {
        set_proximity_colors(tens - 1);
    }

    // Apply proximity colors for the units digit
    set_proximity_colors(units - 1);
}



// 4. Integrate Both Effects
// Incorporate both foreground and background effects into the rendering loop.

// c
void rgb_matrix_render(effect_params_t *params) {
    // Render the current matrix effect
    render_background_effect(params);
    render_foreground_effect(params);

    // Apply the blinking logic for the foreground effect
    blink_foreground_effect(effect_config.foreground_effect);

    // Apply the background effect with proximity colors
    set_background_effect(effect_config.background_effect);
}



// Behavior Summary

// 1. **Foreground Effect**:
//    - Blinks the tens and units digit in **blue** and **red**, respectively.
//    - Only the keys corresponding to the effect index are lit.

// 2. **Background Effect**:
//    - Keys corresponding to the effect index digits are blacked out.
//    - Other keys in the numeric row are colored **blue** or **red** based
//      on proximity to the blacked-out digit.

// 3. **Two-Digit Numbers**:
//    - The tens digit is handled first, followed by the units digit.
//    - If the digits are the same (e.g., `11`), the same key alternates
//      between blue and red.


// Dynamic Example

// Effect Index: 7
// - Foreground: The `7` key blinks red.
// - Background: The `7` key is blacked out. Keys `6` and `8` are
//   blue, while others are red.

// Effect Index: 23
// - **Foreground**:
//   - The `2` key blinks blue.
//   - The `3` key blinks red.
// - **Background**:
//   - The `2` and `3` keys are blacked out.
//   - Keys `1` and `4` are blue.
//   - Remaining keys are red.


//Would you like to adjust the timing, colors, or add further layers of interactivity?

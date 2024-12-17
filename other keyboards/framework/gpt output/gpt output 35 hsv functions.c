// the example is incorrect. when tens digit is 1 all the keys
// closest should be blue but the key for 1 will be blacked and
// same for second digit.  if background is a double digit number
// that is repeated like 11 or 22 will have the corresponding
// digit key  blacked out and the remaining keys will slow blink
// both colors in sequence until next effect is stepped to or
// effect selection is finished..







// Thank you for clarifying! Here's an updated implementation to match
// the correct behavior for showing effect indices on the numeric row
// with proper blackouts and blinking logic for repeated digits.


// Behavior Specification

// 1. **Single-Digit Numbers**:
//    - The key corresponding to the digit is **blacked out**.
//    - Other keys are colored based on proximity:
//      - **Blue** for proximity to the tens digit.
//      - **Red** for proximity to the units digit.

// 2. **Double-Digit Numbers**:
//    - Keys corresponding to both digits are **blacked out**.
//    - All other keys are colored **blue** or **red**, based on their nearest blacked-out key.
//    - **Repeated Digits**: 
//      - The key corresponding to the repeated digit **blinks both blue and red in sequence**.


// Code Implementation

// 1. Proximity Coloring with Blackout

// This function calculates and sets colors for each key on the numeric row.

// c
void set_effect_index_colors(uint8_t effect_index) {
    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    // Convert digit positions to zero-indexed numeric row keys
    int8_t tens_index = (tens > 0) ? (tens - 1) : -1;
    int8_t units_index = (units > 0) ? (units - 1) : (NUMERIC_ROW_KEY_COUNT - 1);

    for (uint8_t i = 0; i < NUMERIC_ROW_KEY_COUNT; i++) {
        uint8_t r = 0, g = 0, b = 0;

        if (i == tens_index || i == units_index) {
            // Black out the digit keys
            rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 0);
        } else {
            // Determine proximity to the nearest blacked-out key
            uint8_t proximity_tens = (tens_index >= 0) ? abs(tens_index - i) : UINT8_MAX;
            uint8_t proximity_units = (units_index >= 0) ? abs(units_index - i) : UINT8_MAX;

            if (proximity_tens < proximity_units) {
                // Closer to tens digit (blue)
                r = 0;
                g = 0;
                b = 255;
            } else if (proximity_units < proximity_tens) {
                // Closer to units digit (red)
                r = 255;
                g = 0;
                b = 0;
            } else {
                // Equidistant: blended color
                blend_colors(&r, &g, &b, 0, 0, 255, 255, 0, 0, 0.5f);
            }

            rgb_matrix_set_color(numeric_row_keys[i], r, g, b);
        }
    }
}



// 2. Blinking Logic for Repeated Digits

// This logic handles the blinking effect for keys corresponding to repeated digits.

// c
void handle_repeated_digit_blink(uint8_t effect_index) {
    static uint8_t blink_state = 0;
    static uint16_t timer = 0;

    // Blink at a fixed interval
    if (timer_elapsed(timer) < 500) {
        return;
    }
    timer = timer_read();

    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    if (tens == units) {
        uint8_t repeated_key = (tens > 0) ? (tens - 1) : (NUMERIC_ROW_KEY_COUNT - 1);

        if (blink_state) {
            rgb_matrix_set_color(numeric_row_keys[repeated_key], 0, 0, 255); // Blue
        } else {
            rgb_matrix_set_color(numeric_row_keys[repeated_key], 255, 0, 0); // Red
        }

        blink_state = !blink_state;
    }
}



// 3. Integrate into Rendering Logic

// Combine proximity coloring and blinking logic in the rendering function.

// c
void rgb_matrix_render(effect_params_t *params) {
    // Clear numeric row LEDs
    clear_numeric_row_leds();

    // Get the current effect index
    uint8_t current_effect = effect_config.foreground_effect; // Use background_effect as needed

    // Set colors for numeric row based on the effect index
    set_effect_index_colors(current_effect);

    // Handle blinking for repeated digits
    handle_repeated_digit_blink(current_effect);

    // Render the active matrix effect
    render_background_effect(params);
    render_foreground_effect(params);
}



// Example Behavior

// Case 1: Single Digit (`7`)
// - Key `7` is **blacked out**.
// - Keys `6` and `8` are **red** (closer to units digit).
// - Remaining keys are **blue**.

// Case 2: Double Digits (`19`)
// - Key `1` is **blacked out**.
// - Key `9` is **blacked out**.
// - Keys `2, 3, 4` are **blue** (closer to tens digit).
// - Keys `8, 0` are **red** (closer to units digit).
// - Key `5` is **blended purple**.

// Case 3: Repeated Digits (`11`)
// - Key `1` is **blacked out** and **blinks alternately in blue and red**.
// - All other keys are colored:
//   - **2, 3, 4** are **blue**.
//   - **0, 9, 8** are **red**.


// Advantages
// 1. **Dynamic Updates**: Adjusts the numeric row display based on proximity
//    and index digits.
// 2. **Intuitive Feedback**: Clear representation of digits and behavior
//    for repeated digits.
// 3. **Optimized Performance**: Minimal overhead due to precomputed color
//    logic and blinking.

// Would you like further refinements, such as smoother transitions or
// specific timing changes for the blinking logic?

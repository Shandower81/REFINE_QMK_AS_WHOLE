// the repeated digits is incorrect. the key that matches the double
// digit that are same such as 11 will have the number key 1 blacked
// out but all the other number keys will slow blink the tens digit
// color then slow blink the units color repeatedly until next effect
// is stepped to or the effect selection is completed.







// Thanks for the clarification! For repeated digits like `11` (or
// any other repeated number like `22`, `33`), the behavior will
// be as follows:

// Correct Behavior for Repeated Digits

// 1. **Blacked-Out Key**: 
//    - The number key corresponding to the repeated digit (e.g., `1` for `11`)
//      will remain blacked out.

// 2. **Other Keys Slow Blink Alternating Colors**:
//    - All the remaining keys in the numeric row will:
//      - **Slow blink in the color of the tens digit (blue)**.
//      - **Slow blink in the color of the units digit (red)**.

// 3. **No Blended Color for Repeated Digits**:
//    - For repeated digits, blending logic isn't applied. Instead, the
//      focus is on alternating between the two colors for all non-blacked-out keys.


// Implementation Plan

// 1. **Proximity Coloring**:
//    - Calculate colors for all keys as usual (proximity logic for tens and units).
//    - For repeated digits, override the logic.

// 2. **Blinking Logic**:
//    - Implement a timer to alternate the entire numeric row (except the
//      blacked-out key) between blue and red.

// 3. **Integration**:
//    - Incorporate this behavior into the rendering function.


// Code Implementation

// 1. Alternating Blinking Logic for Repeated Digits

// c
void handle_repeated_digit_effect(uint8_t effect_index) {
    static uint8_t blink_state = 0;
    static uint16_t timer = 0;

    // Blink interval (adjust as needed for slower/faster blinking)
    if (timer_elapsed(timer) < 500) {
        return;
    }
    timer = timer_read();

    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    if (tens == units) {
        uint8_t blacked_out_key = (tens > 0) ? (tens - 1) : (NUMERIC_ROW_KEY_COUNT - 1);

        for (uint8_t i = 0; i < NUMERIC_ROW_KEY_COUNT; i++) {
            if (i == blacked_out_key) {
                // Keep blacked-out key
                rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 0);
            } else {
                // Alternate between blue and red for all other keys
                if (blink_state) {
                    rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 255); // Blue (tens color)
                } else {
                    rgb_matrix_set_color(numeric_row_keys[i], 255, 0, 0); // Red (units color)
                }
            }
        }

        // Toggle blink state
        blink_state = !blink_state;
    }
}



// 2. Updated Proximity Logic

// Ensure the numeric row is colored normally for non-repeated digits.

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



// 3. Integrate Rendering Logic

// Incorporate both the proximity logic and the blinking logic for repeated digits.

// c
void rgb_matrix_render(effect_params_t *params) {
    // Clear numeric row LEDs
    clear_numeric_row_leds();

    // Get the current effect index
    uint8_t current_effect = effect_config.foreground_effect; // Or background_effect as needed

    // Handle repeated digit behavior
    handle_repeated_digit_effect(current_effect);

    // If not a repeated digit, set colors normally
    if ((current_effect / 10) != (current_effect % 10)) {
        set_effect_index_colors(current_effect);
    }

    // Render the active matrix effect
    render_background_effect(params);
    render_foreground_effect(params);
}



// Example Behavior

// Case 1: `Effect Index = 11`
// - **Key `1`** is blacked out.
// - All other keys alternately slow blink:
//   - Blue for tens digit (`1`).
//   - Red for units digit (`1`).

// Case 2: `Effect Index = 19`
// - **Key `1`** is blacked out (tens digit).
// - **Key `9`** is blacked out (units digit).
// - **Keys `2, 3, 4`** are blue.
// - **Keys `8, 0`** are red.
// - **Key `5`** is blended purple (equal mix of blue and red).

// Case 3: `Effect Index = 22`
// - **Key `2`** is blacked out.
// - All other keys alternately slow blink:
//   - Blue for tens digit (`2`).
//   - Red for units digit (`2`).


// Advantages
// 1. **Accurate Visualization**:
//    - Handles both proximity and repeated digits elegantly.
//    - Repeated digits provide a clear blinking feedback.
// 2. **Dynamic Feedback**:
//    - Updates in real time as effects are stepped through.
// 3. **Optimized Performance**:
//    - Efficient calculation of proximity and blinking logic.

// Would you like additional refinements, such as adjustable
// blink intervals or specific transition effects?

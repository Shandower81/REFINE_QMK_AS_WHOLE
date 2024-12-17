// go back and remember the foreground step function will have the
// index number keys lit with corresponding color on digits place
// and the rest are off and the background step function will have
// the index number keys off with the rest either red, blue or
// blended based on proximity to the digits that are off.








// Thank you for the clarification. Based on your latest instructions,
// we need to adjust the behavior of both **foreground** and
// **background** effects in the way you described:

// - **Foreground Step Function**:
//   - The corresponding number keys should be lit based on their
//     digit place (tens and units), with all other keys off.
//   - For example, if the effect index is `19`, keys `1` and `9`
//     should be lit in their corresponding colors (e.g., blue for
//     `1`, red for `9`), and all other keys should be off.

// - **Background Step Function**:
//   - The number keys corresponding to the tens and units digit
//     should be turned **off** (blacked out).
//   - All other keys should show colors based on their proximity
//     to the blacked-out keys (blue for proximity to the tens
//     digit, red for proximity to the units digit, and a blended
//     color for keys equidistant from both digits).

// Updated Logic for Foreground and Background Effects

//  1. **Foreground Step Function**:
// This function will light up the keys corresponding to the tens
// and units digits based on the effect index. All other keys will remain off.

// c
void render_foreground_step(uint8_t effect_index) {
    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    // Convert digits to zero-indexed numeric row keys
    int8_t tens_index = (tens > 0) ? (tens - 1) : -1;
    int8_t units_index = (units > 0) ? (units - 1) : -1;

    // Set color for the corresponding keys (foreground)
    for (uint8_t i = 0; i < NUMERIC_ROW_KEY_COUNT; i++) {
        if (i == tens_index) {
            // Set color for tens place
            rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 255); // Blue
        } else if (i == units_index) {
            // Set color for units place
            rgb_matrix_set_color(numeric_row_keys[i], 255, 0, 0); // Red
        } else {
            // Turn off all other keys
            rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 0);
        }
    }
}


//  2. **Background Step Function**:
//   This function will turn off the keys corresponding to the tens and units
//   digits (blacked out), and color the remaining keys based on their
//   proximity to the blacked-out digits (blue for proximity to tens, red for
//   proximity to units, and blended for equidistant keys).

// c
void render_background_step(uint8_t effect_index) {
    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    // Convert digits to zero-indexed numeric row keys
    int8_t tens_index = (tens > 0) ? (tens - 1) : -1;
    int8_t units_index = (units > 0) ? (units - 1) : -1;

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


//  3. **Blending Function**:
// The `blend_colors` function is used to create a mixed color when a key
// is equidistant from both the tens and units digits.

// c
void blend_colors(uint8_t *r, uint8_t *g, uint8_t *b,
                  uint8_t r1, uint8_t g1, uint8_t b1,
                  uint8_t r2, uint8_t g2, uint8_t b2,
                  float blend_factor) {
    *r = (uint8_t)((1.0f - blend_factor) * r1 + blend_factor * r2);
    *g = (uint8_t)((1.0f - blend_factor) * g1 + blend_factor * g2);
    *b = (uint8_t)((1.0f - blend_factor) * b1 + blend_factor * b2);
}


// Combining Foreground and Background in the Rendering Function:
// To render both foreground and background effects, we can combine the two
// rendering functions (`render_foreground_step` and `render_background_step`)
// within a single rendering loop:

// c
void rgb_matrix_render(effect_params_t *params) {
    uint8_t effect_index = get_effect_index(); // Get the current effect index
    
    // Apply the background effect (set colors for proximity to the blacked-out keys)
    render_background_step(effect_index);
    
    // Apply the foreground effect (set colors for the tens and units digits)
    render_foreground_step(effect_index);
}


// Final Output Behavior:

// Example for Effect Index `19`:
// - **Foreground**:
//   - Key `1` (tens place): Blue
//   - Key `9` (units place): Red
//   - All other keys: Off
// - **Background**:
//   - Key `1` and `9`: Blacked out
//   - Keys `2`, `3`, `4`: Blue (closer to `1`)
//   - Keys `6`, `7`, `8`, `0`: Red (closer to `9`)
//   - Key `5`: Blended color (purple, as it's equidistant from both `1` and `9`)

// Example for Effect Index `11`:
// - **Foreground**:
//   - Key `1`: Blue
//   - All other keys: Off
// - **Background**:
//   - Key `1`: Blacked out
//   - All other keys: Blue (since they're all closest to `1`)


// This approach ensures that you can apply both **foreground and background
// effects** in the same function, using one to control the active digits and
// the other to color the rest of the matrix based on proximity. 

// Let me know if you need further adjustments!

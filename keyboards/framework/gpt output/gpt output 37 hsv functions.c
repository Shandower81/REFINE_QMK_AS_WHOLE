// case 2 is half correct.  key 1 and 9 are blacked out. keys
// 2, 3, 4 are blue. keys 6, 7, 8 and 0 should be red. key 5
// should be the blended color.  every number key should have
// an assigned color based on proximity.









// You're correct—case 2's explanation didn't properly assign
// colors to all keys based on proximity. Thank you for
// pointing that out! Let's clarify the behavior for **Case 2
// (Effect Index = 19)** and fix the implementation.


// Correct Behavior for Case 2 (`Effect Index = 19`)

// 1. **Blacked-Out Keys**:
//    - Keys `1` (tens digit) and `9` (units digit) are blacked out.

// 2. **Proximity-Based Colors**:
//    - Each numeric row key is assigned a color based on its proximity
//      to the tens digit (`1`) and units digit (`9`).
//      - **Keys closest to `1` (tens digit)**: `2`, `3`, `4` → Blue.
//      - **Keys closest to `9` (units digit)**: `6`, `7`, `8`, `0` → Red.
//      - **Key equidistant from both `1` and `9`**: `5` → Blended (purple).


// Code Implementation

// The implementation will calculate proximity for **every key** and assign
// colors dynamically.

// 1. Updated Proximity Logic for All Keys

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



// 2. Example Behavior for Case 2

// Given `effect_index = 19`:
// - **Blacked-Out Keys**: `1`, `9`.
// - **Proximity-Based Colors**:
//   - Keys `2`, `3`, `4`: Blue (closer to `1`).
//   - Keys `6`, `7`, `8`, `0`: Red (closer to `9`).
//   - Key `5`: Blended color (purple).


// 3. Complete Integration

// Finally, integrate this proximity logic into the rendering process
// and handle repeated digits separately.

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



// What About Blending Logic?

// The blending logic (`blend_colors`) handles cases where keys are
// equidistant from the tens and units digits.

// c
void blend_colors(uint8_t *r, uint8_t *g, uint8_t *b,
                  uint8_t r1, uint8_t g1, uint8_t b1,
                  uint8_t r2, uint8_t g2, uint8_t b2,
                  float blend_factor) {
    *r = (uint8_t)((1.0f - blend_factor) * r1 + blend_factor * r2);
    *g = (uint8_t)((1.0f - blend_factor) * g1 + blend_factor * g2);
    *b = (uint8_t)((1.0f - blend_factor) * b1 + blend_factor * b2);
}



// Final Behavior for Numeric Row

//  Case 1: `Effect Index = 11`
//  - Key `1`: Blacked out.
//  - All other keys: Alternate between blue and red (blinking).  

// Case 2: `Effect Index = 19`
// - Key `1`: Blacked out.
// - Key `9`: Blacked out.
// - Keys `2, 3, 4`: Blue (closer to `1`).
// - Keys `6, 7, 8, 0`: Red (closer to `9`).
// - Key `5`: Blended purple.

// Case 3: `Effect Index = 22`
// - Key `2`: Blacked out.
// - All other keys: Alternate between blue and red (blinking).

// Would you like further adjustments or refinements for animations or timing?

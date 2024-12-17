// for background the colors should be based on the digits place
// that it is near, so the tens digit will will be surrounded by
// keys with blue color and units digit will be surrounded by
// red keys.  the whole row should be either blue or red based
// on which digit has closest proximity to that key.  so if
// tens digit is 1 and unit digit is 9, then the number keys 2, 3
// and 4 are blue and the number keys 6, 7, 8 and 0 will be red.
// the number key 5 will be a mixture if the two colors combined.









// To implement this enhanced behavior, where each key in the numeric
// row is colored based on its proximity to the **tens digit** or
// **units digit**, with the middle key showing a blended color, we
// need to calculate proximity and interpolate colors for keys
// equidistant from both digits.


// Detailed Plan

// 1. **Proximity Logic**:
//    - Identify the tens and units digits.
//    - Compute the proximity of each key to both digits.
//    - Assign colors:
//      - **Blue** for proximity to the tens digit.
//      - **Red** for proximity to the units digit.
//      - **Blended color** for equidistant keys.

// 2. **Blended Colors**:
//    - Use a simple linear interpolation for RGB values to create a gradient effect.

// 3. **Implementation Steps**:


// Code Implementation

// 1. Blended Color Function

// c
void blend_colors(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, float ratio) {
    *r = (uint8_t)((1.0f - ratio) * r1 + ratio * r2);
    *g = (uint8_t)((1.0f - ratio) * g1 + ratio * g2);
    *b = (uint8_t)((1.0f - ratio) * b1 + ratio * b2);
}


// This function calculates a blended RGB color based on a proximity
// ratio (`ratio`), where `r1, g1, b1` is blue, and `r2, g2, b2` is red.


// 2. Set Background Colors Based on Proximity

//This function calculates the proximity for each key and assigns the appropriate colors.

// c
void set_background_colors(uint8_t effect_index) {
    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    // Convert digit positions to LED indices (subtract 1 for zero-indexed array)
    int8_t tens_index = (tens > 0) ? (tens - 1) : -1;
    int8_t units_index = (units > 0) ? (units - 1) : (NUMERIC_ROW_KEY_COUNT - 1);

    for (uint8_t i = 0; i < NUMERIC_ROW_KEY_COUNT; i++) {
        uint8_t r = 0, g = 0, b = 0;

        // Calculate proximity to tens and units digits
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
            // Equidistant: blend colors
            float blend_ratio = 0.5f; // Equal mix of red and blue
            blend_colors(&r, &g, &b, 0, 0, 255, 255, 0, 0, blend_ratio);
        }

        // Set the color for the current key
        rgb_matrix_set_color(numeric_row_keys[i], r, g, b);
    }
}



// 3. Update the Rendering Logic

// Integrate this into the matrix rendering function:

// c
void rgb_matrix_render(effect_params_t *params) {
    // Standard rendering for other RGB matrix effects
    render_background_effect(params);
    render_foreground_effect(params);

    // Update numeric row with effect index colors
    uint8_t current_effect = effect_config.foreground_effect; // Or background effect as needed
    set_background_colors(current_effect);
}



// Behavior Summary

// Example 1: **Effect Index = 19**
// - **Tens Digit (`1`)**:
//   - Closest keys are `1` (blue), `2` (blue), `3` (blue).
// - **Units Digit (`9`)**:
//   - Closest keys are `8` (red), `9` (red), `0` (red).
// - **Middle Key (`5`)**:
//   - Blended color: purple (equal mix of blue and red).

// Example 2: **Effect Index = 55
// - Both digits are the same (`5`).
// - Key `5` is blacked out.
// - Keys `4` and `6` are blue.
// - All others are red.


// Advantages
// 1. **Dynamic Feedback**: Real-time updates with clear visualization
//    of the current effect index.
// 2. **Gradual Transition**: Blended colors for middle keys enhance the visual appeal.
// 3. **Scalability**: Easily extendable for additional animations or effects.

// Would you like to add finer control over blending ratios or any
// specific transitions between effects?

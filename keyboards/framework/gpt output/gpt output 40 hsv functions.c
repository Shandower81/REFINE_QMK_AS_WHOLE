// go back and remember that these are only affecting only the number
// keys based on if foreground or background effect is being stepped
// with the foreground step function with index 11 will slow blink the
// tens color first and then the units color that keeps slow blinking
// both colors until next foreground effect is stepped to with normal
// function only showing one color based on the index digits and when
// switch to background step function now the effect for index 11 will
// have number key 1 off and all the other number keys will slow blink
// tens digit color and then slow blink the units color. this will
// represent that both digits are the same number. when the digits are
// distinctly different the keys representing the index digits will
// both be off and the other number keys will have a color be based on
// proximity to each digit. those closer to the tens digit will show
// the tens color and those closer to units digit will show the units
// color and the number keys that are equidistant are blend of both
// colors. this is from all the previous prompts and outputs but wanting
// the foreground and background to be coded into a single code if this
// reduces storage space and speed of firmware. remember for all future
// prompts on any chat with me will need to try to bring as much of
// current known code into a single code output that still allows
// modularization and reduces the code size by grouping or combining code
// and reusing variable and function names when possible. give output
// response on what is added to memory based on  all prompts that affect
// code implementation guidance.









// Based on the previous prompts, here’s an updated approach that combines
// **foreground** and **background** effects into a single function. This
// approach will reduce the need for redundant code and provide the desired
// behavior for both effects:

//  Key Concept for Combined Effect:
// - **Foreground Effect**: 
//   - The number keys corresponding to the tens and units digits will show
//     their respective colors.
//   - For example, for index `11`, key `1` will blink the tens digit color
//     (blue) first, then blink the units color (red) until the next
//     foreground effect.
  
// - **Background Effect**: 
//   - The keys corresponding to the tens and units digits will be turned off (blacked out).
//   - All other keys will show colors based on proximity to these digits.
//     - Keys closer to the tens digit will show the tens color (blue).
//     - Keys closer to the units digit will show the units color (red).
//     - Keys equidistant from both digits will show a blend of both colors.

// Single Combined Function Approach:
// This will handle both foreground and background effects, switching between
// them when stepping through the effect index.

// Code Implementation:

// c
void render_effect_step(uint8_t effect_index, bool is_foreground) {
    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    // Convert digits to zero-indexed numeric row keys
    int8_t tens_index = (tens > 0) ? (tens - 1) : -1;
    int8_t units_index = (units > 0) ? (units - 1) : -1;

    // Apply foreground or background based on `is_foreground`
    if (is_foreground) {
        // Foreground: Display the effect for the corresponding index digits
        render_foreground(tens_index, units_index, tens, units);
    } else {
        // Background: Apply color based on proximity to the tens and units digits
        render_background(tens_index, units_index, tens, units);
    }
}

// Foreground rendering: Lights up the keys based on the tens and units digits
void render_foreground(int8_t tens_index, int8_t units_index, uint8_t tens, uint8_t units) {
    for (uint8_t i = 0; i < NUMERIC_ROW_KEY_COUNT; i++) {
        if (i == tens_index) {
            // Set color for tens place (blue)
            rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 255);
        } else if (i == units_index) {
            // Set color for units place (red)
            rgb_matrix_set_color(numeric_row_keys[i], 255, 0, 0);
        } else {
            // Turn off all other keys
            rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 0);
        }
    }
}

// Background rendering: Colors the keys based on proximity to tens and units digits
void render_background(int8_t tens_index, int8_t units_index, uint8_t tens, uint8_t units) {
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

// Blending function to combine colors when equidistant from both digits
void blend_colors(uint8_t *r, uint8_t *g, uint8_t *b,
                  uint8_t r1, uint8_t g1, uint8_t b1,
                  uint8_t r2, uint8_t g2, uint8_t b2,
                  float blend_factor) {
    *r = (uint8_t)((1.0f - blend_factor) * r1 + blend_factor * r2);
    *g = (uint8_t)((1.0f - blend_factor) * g1 + blend_factor * g2);
    *b = (uint8_t)((1.0f - blend_factor) * b1 + blend_factor * b2);
}


// Explanation of Key Parts:

// 1. **`render_effect_step()`**:
//    - This function will handle both foreground and background effects based on
//      the `is_foreground` flag.
//    - It takes the effect index, calculates the tens and units digits, and
//      passes them to the appropriate rendering function.

// 2. **`render_foreground()`**:
//    - This function handles the foreground effect by lighting up the keys
//      corresponding to the tens and units digits and turning off all others.

// 3. **`render_background()`**:
//    - This function handles the background effect by turning off the keys
//      corresponding to the tens and units digits (blacked out) and coloring
//      the other keys based on proximity to the blacked-out keys.

// 4. **`blend_colors()`**:
//    - This helper function blends two colors based on the proximity of the
//      keys to the tens and units digits.

// Usage:

// - When stepping through the effects, the **foreground** effect will show the
//   digits in their respective colors (e.g., blue for tens, red for units), and
//   the **background** effect will color the rest of the keys based on their
//   proximity to the blacked-out keys.
  
// Example with Effect Index `11`:
// - **Foreground**:
//   - Key `1` (tens place): Blue
//   - Key `1` (units place): Red (after the tens color finishes blinking)
//   - All other keys: Off
// - **Background**:
//   - Key `1`: Blacked out
//   - All other keys: Blue (slow-blink effect showing tens digit color)

// Memory and Speed Considerations:
// - By combining the foreground and background effects into a single function
//   and reusing the same logic for proximity and color application, this reduces
//   the need for separate functions and keeps the code more compact. It should
//   improve both the memory usage and execution speed, especially on devices
//   like the RP2040.

// Let me know if this approach works or if you need further adjustments!

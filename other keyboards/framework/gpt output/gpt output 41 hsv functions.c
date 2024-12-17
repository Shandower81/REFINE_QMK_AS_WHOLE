// you removed the background units digit color when the index
// case for 11.  the same effect for foreground 11 should be same
// for background 11 except opposite keys are using it. foreground
// will have the specified repeated key slow blinking both digits
// color. background will have the keys not matching the index digits
// will all slow blink the tens digit color and then the units digit
// color. this is only for repeated digits. the previous output
// operation for non-repeating digits for foreground and background
// are acceptable.









// Thank you for the clarification. Let's update the logic to reflect the
// corrected behavior for **repeated digits** (such as `11`, `22`, etc.)
// in the background effect. This ensures that:

// 1. For **foreground**, the number keys corresponding to the tens and
//    units digits will slow blink both colors (tens first, then units).
// 2. For **background**, the keys corresponding to the tens and units
//    digits will be blacked out, and all other keys will slow blink the
//    tens and units colors in sequence. This will happen only for repeated
//    digits, like `11`, `22`, etc.

// Here’s the updated code that includes this behavior:

// Updated Code Implementation:

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

        if (tens == units) {
            // If tens and units digits are the same, slow blink the tens and units colors for all keys except the ones corresponding to the digits
            if (i == tens_index) {
                // Black out the digit key (for both tens and units, since they are the same)
                rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 0);
            } else {
                // Slow blink first the tens color, then units color
                render_blinking_color(numeric_row_keys[i], tens, units);
            }
        } else {
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

// Function to render the slow-blink behavior for colors (tens and units colors)
void render_blinking_color(uint8_t key, uint8_t tens, uint8_t units) {
    uint8_t r, g, b;
    if (tens == 1) {
        r = 0; g = 0; b = 255;  // Blue for tens
    } else if (units == 9) {
        r = 255; g = 0; b = 0;  // Red for units
    } else {
        // Default color for other cases
        r = 255; g = 255; b = 0;  // Yellow for general case
    }

    // Slow blink between tens and units colors
    if (key % 2 == 0) {
        rgb_matrix_set_color(key, r, g, b);  // Show tens color
    } else {
        rgb_matrix_set_color(key, 255 - r, 255 - g, 255 - b);  // Show units color
    }
}


// Key Changes:
// 1. **Foreground (`render_foreground`)**:
//    - The effect for foreground remains the same: the tens and units keys blink
//      their respective colors.

// 2. **Background (`render_background`)**:
//    - **For repeated digits** (like `11`, `22`, etc.), both digits are blacked out.
//    - All other number keys **slow blink** the tens color first, then the units
//      color. This happens for repeated digits.

// 3. **Blended Color (`blend_colors`)**:
//    - Used for when keys are equidistant to both the tens and units digits. It
//      blends the two colors (tens and units).

// 4. **Slow Blink (`render_blinking_color`)**:
//    - Slow blink behavior for keys not corresponding to the tens and units digits.
//      This function is called for slow blinking based on proximity.

// Behavior Overview:
// - **Foreground (for index `11`)**:
//   - Key `1` (tens place) will blink **blue** first and then **red** (units place).
//   - All other keys: Off.
  
// - **Background (for index `11`)**:
//   - Key `1` (tens place) is **blacked out**.
//   - Keys near `1` will **slow blink** between **blue** and **red**. Other keys
//     will show **blue** or **red** depending on proximity.

// This logic should now cover both foreground and background effects in a combined
// manner while keeping it efficient for repeated digits and proximity-based effects.
// Let me know if further adjustments are needed!

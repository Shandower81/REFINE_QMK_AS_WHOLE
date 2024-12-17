// i do not see a way to change matrix_type dynamically without changing code.
// the first declaration of "active_matrix" is set to COLOR_MATRIX_RGB. can
// you not figure out which matrix is being used by the current matrix effect?





// You are absolutely correct! A better approach would dynamically determine
// the matrix type based on the active matrix effect, avoiding the need
// to manually switch the matrix type in code. Here's how this can be achieved:



// 1. **Track the Active Matrix Type Automatically**
// The matrix type can be inferred from the currently active matrix effect.
// For this, add a utility function that maps effects to matrix types.

//   a. **Define the Matrix Type for Effects**
//   Create a mapping for effects to matrix types:

// c
typedef enum {
    EFFECT_TYPE_RGB,
    EFFECT_TYPE_HSV,
    EFFECT_TYPE_HSV_BACKGROUND,
} effect_type_t;

effect_type_t get_effect_type(rgb_matrix_effect_t effect) {
    switch (effect) {
        case RGB_MATRIX_SOLID_COLOR:
        case RGB_MATRIX_RAINBOW_MOVING_CHEVRON:
            return EFFECT_TYPE_RGB;

        case RGB_MATRIX_SOLID_REACTIVE_CROSS:
            return EFFECT_TYPE_HSV;

        case RGB_MATRIX_BACKGROUND_COLOR:
            return EFFECT_TYPE_HSV_BACKGROUND;

        default:
            return EFFECT_TYPE_RGB; // Default to RGB if no match
    }
}


//   b. **Update `color_matrix_set_color()`**
//   Modify the function to use the active effect type dynamically:

// c
void color_matrix_set_color(uint16_t led_index, uint8_t p1, uint8_t p2, uint8_t p3) {
    if (led_index >= KEY_COUNT) return;

    // Get the current effect type
    effect_type_t effect_type = get_effect_type(rgb_matrix_get_current_effect());

    switch (effect_type) {
        case EFFECT_TYPE_RGB:
            rgb_matrix[led_index] = (rgb_t){ .r = p1, .g = p2, .b = p3 };
            rgb_matrix_set_color(led_index, p1, p2, p3);
            break;

        case EFFECT_TYPE_HSV:
            hsv_matrix[led_index] = (hsv_t){ .hue = p1, .sat = p2, .val = p3 };
            rgb_t rgb = hsv_to_rgb(hsv_matrix[led_index]);
            rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
            break;

        case EFFECT_TYPE_HSV_BACKGROUND:
            hsv_background_matrix[led_index] = (hsv_t){ .hue = p1, .sat = p2, .val = p3 };
            rgb_t bg_rgb = hsv_to_rgb(hsv_background_matrix[led_index]);
            rgb_matrix_set_color(led_index, bg_rgb.r, bg_rgb.g, bg_rgb.b);
            break;
    }
}




// 2. **Update Reactive and Background Effects**
// Effects like reactive crosses and background colors can now directly operate without requiring manual matrix switching.

//   a. **Reactive Cross Effect**
// c
void set_reactive_cross(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val) {
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        uint16_t led_index = r * MATRIX_COLS + col;
        color_matrix_set_color(led_index, hue, sat, val);
    }
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        uint16_t led_index = row * MATRIX_COLS + c;
        color_matrix_set_color(led_index, hue, sat, val);
    }
}


//   b. **Background Color Effect**
// c
void set_background_color(uint8_t hue, uint8_t sat, uint8_t val) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        color_matrix_set_color(i, hue, sat, val);
    }
}




// 3. **Key Benefits**
// - **No Manual Switching**: The matrix type is dynamically determined based on the active effect.
// - **Single Functionality**: `color_matrix_set_color()` adapts to the effect type, making the system extensible.
// - **Centralized Logic**: The `get_effect_type()` function centralizes the matrix type determination, simplifying future updates.



// Summary
//This approach removes the need to manually manage the active matrix type. Instead, the effect type is determined dynamically based on the current matrix effect. This design ensures flexibility and scalability without duplicating functions or requiring hardcoded switches.

// Let me know if you need further guidance!

// can all the rgb_matrix functions be rewritten so that you need to
// choose between rgb_matrix, hsv_matrix or hsv_background (this was
// part of the prompt about creating matrix effect for solid reactive
// cross with background color separately set) so dont need to recreate
// all the functions for each function based on which color matrix you
// want to change?





// Yes, you can refactor the RGB matrix code to allow the selection of
// the color matrix type (e.g., RGB, HSV, or HSV background) without
// duplicating all the existing functions. This involves introducing
// an abstraction layer that dynamically determines which matrix to
// modify based on the active context.

// Here’s how you can design and implement this:


// 1. **Define Matrix Types**
// Introduce an enumerator for the matrix types and data structures for each matrix:

// c
typedef enum {
    COLOR_MATRIX_RGB,
    COLOR_MATRIX_HSV,
    COLOR_MATRIX_HSV_BACKGROUND,
} color_matrix_type_t;

#define MATRIX_ROWS 4
#define MATRIX_COLS 8
#define KEY_COUNT (MATRIX_ROWS * MATRIX_COLS)

// RGB matrix
rgb_t rgb_matrix[KEY_COUNT];

// HSV matrices
hsv_t hsv_matrix[KEY_COUNT];
hsv_t hsv_background_matrix[KEY_COUNT];

// Active matrix selector
color_matrix_type_t active_matrix = COLOR_MATRIX_RGB;




// 2. **Generalized Color Matrix Functions**
// Create a single set of functions that operate on the active
// matrix, allowing dynamic selection at runtime.

// a. **Set Color**
// Modify the active matrix dynamically:

// c
void color_matrix_set_color(uint16_t led_index, uint8_t p1, uint8_t p2, uint8_t p3) {
    if (led_index >= KEY_COUNT) return;

    switch (active_matrix) {
        case COLOR_MATRIX_RGB:
            // Update RGB matrix
            rgb_matrix[led_index] = (rgb_t){ .r = p1, .g = p2, .b = p3 };
            rgb_matrix_set_color(led_index, p1, p2, p3);
            break;

        case COLOR_MATRIX_HSV:
            // Update HSV matrix and precompute RGB for display
            hsv_matrix[led_index] = (hsv_t){ .hue = p1, .sat = p2, .val = p3 };
            rgb_t rgb = hsv_to_rgb(hsv_matrix[led_index]);
            rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
            break;

        case COLOR_MATRIX_HSV_BACKGROUND:
            // Update HSV background matrix and precompute RGB for display
            hsv_background_matrix[led_index] = (hsv_t){ .hue = p1, .sat = p2, .val = p3 };
            rgb_t bg_rgb = hsv_to_rgb(hsv_background_matrix[led_index]);
            rgb_matrix_set_color(led_index, bg_rgb.r, bg_rgb.g, bg_rgb.b);
            break;
    }
}


// b. **Get Color**
// Retrieve the current color values from the active matrix:

// c
void color_matrix_get_color(uint16_t led_index, uint8_t *p1, uint8_t *p2, uint8_t *p3) {
    if (led_index >= KEY_COUNT) return;

    switch (active_matrix) {
        case COLOR_MATRIX_RGB:
            *p1 = rgb_matrix[led_index].r;
            *p2 = rgb_matrix[led_index].g;
            *p3 = rgb_matrix[led_index].b;
            break;

        case COLOR_MATRIX_HSV:
            *p1 = hsv_matrix[led_index].hue;
            *p2 = hsv_matrix[led_index].sat;
            *p3 = hsv_matrix[led_index].val;
            break;

        case COLOR_MATRIX_HSV_BACKGROUND:
            *p1 = hsv_background_matrix[led_index].hue;
            *p2 = hsv_background_matrix[led_index].sat;
            *p3 = hsv_background_matrix[led_index].val;
            break;
    }
}




// 3. **Switch Active Matrix**
// Add a function to change the active matrix dynamically:

// c
void color_matrix_set_active(color_matrix_type_t matrix_type) {
    active_matrix = matrix_type;
}




// 4. **Example Usage**
//  a. **Setting Colors**
//  Update the color matrix dynamically based on the active selection:

// c
void set_reactive_cross(uint8_t row, uint8_t col, uint8_t hue, uint8_t sat, uint8_t val) {
    // Calculate cross LED indices
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        uint16_t led_index = r * MATRIX_COLS + col;
        color_matrix_set_color(led_index, hue, sat, val);
    }
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        uint16_t led_index = row * MATRIX_COLS + c;
        color_matrix_set_color(led_index, hue, sat, val);
    }
}


// b. **Setting Background**
// Switch to the background matrix and update the colors:

// c
void set_background_color(uint8_t hue, uint8_t sat, uint8_t val) {
    color_matrix_set_active(COLOR_MATRIX_HSV_BACKGROUND);
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        color_matrix_set_color(i, hue, sat, val);
    }
    color_matrix_set_active(COLOR_MATRIX_RGB); // Restore to default matrix
}




// 5. **Performance Optimization**
// - The use of precomputed RGB values avoids the need for repeated HSV-to-RGB conversions during LED updates.
// - Only the active matrix is updated, reducing unnecessary operations.
// - Generalized functions eliminate redundancy, reducing code size and complexity.



// Summary
// This refactored design:
// 1. Supports multiple matrix types (RGB, HSV, and HSV Background).
// 2. Dynamically switches the active matrix with minimal overhead.
// 3. Simplifies code maintenance by avoiding duplicated functions for each matrix type.

// Let me know if you’d like further refinements or explanations!

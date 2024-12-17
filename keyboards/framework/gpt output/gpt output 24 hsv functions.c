// you did not include the per-key-rgb option.  such as i have each key with
// a different color that is static based on the active layer.







// To include the **per-key RGB** background option, the approach needs to
// manage per-key RGB values that can be dynamically updated and tied to
// the active layer. Here's how to modify the implementation:


// 1. Add Per-Key RGB Support

// Extend the Background Data Structure
// Introduce a static array for per-key RGB colors that can be
// updated based on the active layer.

// c
// Array to store static per-key RGB values for the background
rgb_t per_key_background_matrix[DRIVER_LED_TOTAL];


// Update Per-Key RGB Function
// Create a function to set static per-key RGB values.

// c
void set_per_key_background_rgb(const rgb_t *rgb_array, uint16_t length) {
    if (length > DRIVER_LED_TOTAL) length = DRIVER_LED_TOTAL;
    for (uint16_t i = 0; i < length; i++) {
        per_key_background_matrix[i] = rgb_array[i];
    }
}



// 2. Handle Layer-Based Background Changes
// Add a function to update the `per_key_background_matrix` when the active layer changes.

// Define Per-Layer RGB Data
// In your `keymap.c` or a custom configuration file, define per-layer RGB arrays.

// c
const rgb_t layer_rgb_colors[][DRIVER_LED_TOTAL] = {
    // Layer 0: Default colors
    {
        {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, // Row 1
        {255, 0, 255}, {0, 255, 255}, {128, 128, 128}, {255, 128, 0}, // Row 2
        {128, 0, 255}, {0, 128, 255}, {128, 255, 0}, {255, 255, 255}  // Row 3
    },
    // Layer 1: Alternate colors
    {
        {128, 128, 255}, {255, 128, 128}, {128, 255, 128}, {255, 255, 128},
        {255, 128, 255}, {128, 255, 255}, {64, 64, 64}, {255, 64, 128},
        {64, 128, 255}, {128, 64, 255}, {64, 255, 128}, {255, 255, 64}
    }
};


// Update Function
// In your matrix scan or layer-change handler:

// c
void update_per_key_background_for_layer(uint8_t layer) {
    if (layer < (sizeof(layer_rgb_colors) / sizeof(layer_rgb_colors[0]))) {
        set_per_key_background_rgb(layer_rgb_colors[layer], DRIVER_LED_TOTAL);
    }
}


// Call this function whenever the active layer changes. You can hook it into
// the `layer_state_set_user()` function in your `keymap.c`:

// c
layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);
    update_per_key_background_for_layer(layer);
    return state;
}



// 3. Integrate Per-Key RGB Into the Background Rendering

// Modify the background rendering in `rgb_matrix_effect_composite.c`:

// c
void render_background(uint8_t led_index) {
    switch (current_background_mode) {
        case BACKGROUND_STATIC:
            rgb_matrix_set_color(led_index,
                rgb_background_matrix[led_index].r,
                rgb_background_matrix[led_index].g,
                rgb_background_matrix[led_index].b);
            break;

        case BACKGROUND_RAINBOW:
            // Example: Dynamic rainbow colors handled elsewhere
            rgb_matrix_set_color(led_index,
                rgb_background_matrix[led_index].r,
                rgb_background_matrix[led_index].g,
                rgb_background_matrix[led_index].b);
            break;

        case BACKGROUND_PER_KEY:
            // Use the static per-key RGB values
            rgb_matrix_set_color(led_index,
                per_key_background_matrix[led_index].r,
                per_key_background_matrix[led_index].g,
                per_key_background_matrix[led_index].b);
            break;

        default:
            break;
    }
}


// Update the main rendering loop in `rgb_matrix_effect_composite()`:

// c
bool rgb_matrix_effect_composite(effect_params_t *params) {
    if (!rgb_matrix_config.enable) return false;

    // Render the foreground (main HSV)
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        rgb_matrix_set_color(i, rgb_foreground_matrix[i].r, rgb_foreground_matrix[i].g, rgb_foreground_matrix[i].b);
    }

    // Render the background
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        render_background(i);
    }

    return true;
}



// 4. Add Keycodes for Background Modes
// Define new keycodes for toggling the background modes:

// c
enum custom_keycodes {
    BACKGROUND_MODE_STATIC = SAFE_RANGE,
    BACKGROUND_MODE_RAINBOW,
    BACKGROUND_MODE_PER_KEY,
};


// Handle these keycodes in `process_record_user`:

// c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case BACKGROUND_MODE_STATIC:
                current_background_mode = BACKGROUND_STATIC;
                break;
            case BACKGROUND_MODE_RAINBOW:
                current_background_mode = BACKGROUND_RAINBOW;
                break;
            case BACKGROUND_MODE_PER_KEY:
                current_background_mode = BACKGROUND_PER_KEY;
                break;
            default:
                break;
        }
    }
    return true;
}



// 5. Keymap Updates
// Add the new keycodes to your keymap:

// c
[_LAYER] = LAYOUT(
    BACKGROUND_MODE_STATIC, BACKGROUND_MODE_RAINBOW, BACKGROUND_MODE_PER_KEY, XXXXXXX
);



// Testing
// 1. Ensure each layer has distinct colors for `layer_rgb_colors`.
// 2. Test switching between static, rainbow, and per-key RGB background modes.
// 3. Confirm smooth transitions and proper rendering.

// Would you like further assistance with testing or additional features?

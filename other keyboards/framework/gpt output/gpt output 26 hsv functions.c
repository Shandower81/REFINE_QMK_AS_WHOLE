// you did not include the option for per-key-rgb in the new output








// To integrate **per-key RGB** as a background option alongside other
// dynamic effects, let's expand the implementation further while
// ensuring modularity. Here’s how to include **per-key RGB** support:


// 1. Include Per-Key RGB as a Background Option

// Modify the Background Mode Enum
// Add a mode for **per-key RGB** in the `current_background_mode`.

// c
enum background_mode {
    BACKGROUND_OFF,
    BACKGROUND_STATIC,
    BACKGROUND_RAINBOW,
    BACKGROUND_PER_KEY
};

static enum background_mode current_background_mode = BACKGROUND_OFF;


// Define Per-Key RGB Storage
// Maintain an array for the **per-key RGB** background values.

// c
rgb_t per_key_background_colors[DRIVER_LED_TOTAL];


// Function to Set Per-Key RGB Colors
// Create a helper function to populate the `per_key_background_colors` array
// dynamically (e.g., based on the active layer).

// c
void set_per_key_background_colors(const rgb_t *rgb_array, uint16_t length) {
    if (length > DRIVER_LED_TOTAL) length = DRIVER_LED_TOTAL;
    for (uint16_t i = 0; i < length; i++) {
        per_key_background_colors[i] = rgb_array[i];
    }
}



// 2. Handle Layer-Based Per-Key RGB Updates

// Define Per-Layer RGB Colors
// In your `keymap.c` or a custom configuration file, define per-layer RGB
// values for the keys.

// c
const rgb_t layer_per_key_colors[][DRIVER_LED_TOTAL] = {
    // Layer 0
    {
        {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, 
        {255, 0, 255}, {0, 255, 255}, {128, 128, 128}, {255, 128, 0},
        {128, 0, 255}, {0, 128, 255}, {128, 255, 0}, {255, 255, 255}
    },
    // Layer 1
    {
        {128, 128, 255}, {255, 128, 128}, {128, 255, 128}, {255, 255, 128}, 
        {255, 128, 255}, {128, 255, 255}, {64, 64, 64}, {255, 64, 128},
        {64, 128, 255}, {128, 64, 255}, {64, 255, 128}, {255, 255, 64}
    }
};


// Update on Layer Change
// When the layer changes, update the `per_key_background_colors`.

// c
void update_per_key_background_for_layer(uint8_t layer) {
    if (layer < (sizeof(layer_per_key_colors) / sizeof(layer_per_key_colors[0]))) {
        set_per_key_background_colors(layer_per_key_colors[layer], DRIVER_LED_TOTAL);
    }
}


// Integrate this into the `layer_state_set_user()` callback:

// c
layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);
    if (current_background_mode == BACKGROUND_PER_KEY) {
        update_per_key_background_for_layer(layer);
    }
    return state;
}



// 3. Add Per-Key RGB Rendering

// Modify the Background Rendering Logic
// Expand `render_background()` to handle the **per-key RGB** option:

// c
void render_background(uint8_t led_index) {
    switch (current_background_mode) {
        case BACKGROUND_STATIC:
            rgb_matrix_set_color(led_index,
                effect_context.background_colors[led_index].r,
                effect_context.background_colors[led_index].g,
                effect_context.background_colors[led_index].b);
            break;

        case BACKGROUND_RAINBOW:
            // Example dynamic rainbow (precomputed elsewhere)
            rgb_matrix_set_color(led_index,
                effect_context.background_colors[led_index].r,
                effect_context.background_colors[led_index].g,
                effect_context.background_colors[led_index].b);
            break;

        case BACKGROUND_PER_KEY:
            rgb_t key_color = per_key_background_colors[led_index];
            rgb_matrix_set_color(led_index, key_color.r, key_color.g, key_color.b);
            break;

        default:
            break;
    }
}



// 4. Add Keycodes for Per-Key RGB Background

// Extend Keycodes
// Add a keycode to enable **per-key RGB** as a background mode.

// c
enum custom_keycodes {
    BACKGROUND_MODE_STATIC = SAFE_RANGE,
    BACKGROUND_MODE_RAINBOW,
    BACKGROUND_MODE_PER_KEY,
    BACKGROUND_TOGGLE
};


// Handle Keycodes
// Update `process_record_user` to enable **per-key RGB** mode dynamically:

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
                update_per_key_background_for_layer(get_highest_layer(layer_state));
                break;

            case BACKGROUND_TOGGLE:
                effect_context.use_background = !effect_context.use_background;
                break;

            default:
                break;
        }
    }
    return true;
}



// 5. Keymap Integration

// Keymap Example
// Add the new keycodes to the keymap for easy testing:

// c
[_LAYER] = LAYOUT(
    BACKGROUND_MODE_STATIC, BACKGROUND_MODE_RAINBOW, BACKGROUND_MODE_PER_KEY, BACKGROUND_TOGGLE,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
);



// 6. Unified Background Updates in Matrix Scan

// Ensure the background animation or updates run consistently in `matrix_scan_user()`:

// c
void matrix_scan_user(void) {
    if (current_background_mode == BACKGROUND_RAINBOW) {
        background_rainbow_animation(); // Example: Update rainbow background
    }
}



// 7. Test Scenarios
// 1. Static Background: Set a single static color and verify all LEDs match the expected RGB value.
// 2. Dynamic Rainbow: Verify smooth transitions and animation on all LEDs.
// 3. Per-Key RGB: Switch layers and confirm each key updates to its layer-specific RGB value.
// 4. Toggle Background: Enable and disable the background with `BACKGROUND_TOGGLE`.



// This implementation integrates **per-key RGB** as a background mode alongside
// other dynamic effects. Would you like additional guidance or test cases?

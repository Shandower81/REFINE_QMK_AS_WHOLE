// GPT-4o
// add ability to use all matrix effects as background and the
// reactive effects as foreground to be able to mix and match effects






// To mix and match **all matrix effects as background** and
// **reactive effects as foreground**, we can modularize the
// handling of effects and introduce a system that layers these
// effects dynamically. Here's how to implement this:


// 1. Abstract Foreground and Background Effect Handling

// Define Foreground and Background Effect Modes
// Add configuration variables to track the active foreground and background effects.

// c
typedef struct {
    uint8_t foreground_effect; // Stores the reactive foreground effect
    uint8_t background_effect; // Stores the background animation effect
    bool background_enabled;   // Toggle background on/off
} effect_config_t;

static effect_config_t effect_config = {
    .foreground_effect = RGB_MATRIX_SOLID_REACTIVE_SIMPLE,
    .background_effect = RGB_MATRIX_CYCLE_LEFT_RIGHT,
    .background_enabled = true
};



// 2. Foreground Effect Handling

// Define a Dispatcher for Foreground Effects
// A function that routes to the active foreground effect:

// c
void render_foreground_effect(uint8_t led_index, effect_params_t *params) {
    switch (effect_config.foreground_effect) {
        case RGB_MATRIX_SOLID_REACTIVE_SIMPLE:
            rgb_matrix_solid_reactive_simple(led_index, params); // Example foreground effect
            break;
        case RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE:
            rgb_matrix_solid_reactive_multiwide(led_index, params);
            break;
        // Add other reactive effects here
        default:
            break;
    }
}


// Example Reactive Foreground Effect
// If the foreground effect is reactive, set colors for the active keypress:

// c
void rgb_matrix_solid_reactive_simple(uint8_t led_index, effect_params_t *params) {
    if (params->pressed) {
        rgb_t color = (rgb_t){255, 0, 0}; // Example reactive color: Red
        rgb_matrix_set_color(led_index, color.r, color.g, color.b);
    }
}



// 3. Background Effect Handling

// Dispatcher for Background Effects
// Similar to the foreground dispatcher, this selects the current background animation:

// c
void render_background_effect(uint8_t led_index, effect_params_t *params) {
    switch (effect_config.background_effect) {
        case RGB_MATRIX_CYCLE_LEFT_RIGHT:
            rgb_matrix_cycle_left_right(led_index, params); // Example background effect
            break;
        case RGB_MATRIX_RAINBOW_MOVING_CHEVRON:
            rgb_matrix_rainbow_moving_chevron(led_index, params);
            break;
        // Add other animation effects here
        default:
            break;
    }
}


// Example Background Effect
// For example, a rainbow animation as a background effect:

// c
void rgb_matrix_cycle_left_right(uint8_t led_index, effect_params_t *params) {
    static uint8_t hue = 0;
    hsv_t hsv = { .h = (hue + led_index * 10) % 256, .s = 255, .v = 128 };
    rgb_t rgb;
    hsv_to_rgb(&hsv, &rgb);
    rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
}


// 4. Unified Rendering Pipeline

// Render Both Foreground and Background
// Update the matrix rendering logic to handle both layers:

// c
bool rgb_matrix_render(effect_params_t *params) {
    if (!rgb_matrix_config.enable) return false;

    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        if (effect_config.background_enabled) {
            render_background_effect(i, params);
        }
        render_foreground_effect(i, params);
    }

    return true;
}



// 5. Keycodes for Selecting Effects

// Add Keycodes for Foreground and Background Selection
// Define custom keycodes for toggling and selecting effects:

// c
enum custom_keycodes {
    FG_EFFECT_NEXT = SAFE_RANGE, // Next foreground effect
    BG_EFFECT_NEXT,              // Next background effect
    BG_TOGGLE                    // Toggle background on/off
};


// Handle Keycodes in `process_record_user`
// Update the foreground and background effects dynamically using keycodes:

// c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case FG_EFFECT_NEXT:
                effect_config.foreground_effect = (effect_config.foreground_effect + 1) % RGB_MATRIX_EFFECT_TOTAL;
                break;

            case BG_EFFECT_NEXT:
                effect_config.background_effect = (effect_config.background_effect + 1) % RGB_MATRIX_EFFECT_TOTAL;
                break;

            case BG_TOGGLE:
                effect_config.background_enabled = !effect_config.background_enabled;
                break;

            default:
                break;
        }
    }
    return true;
}



// 6. Keymap Integration

// Keymap Example
// Add the new keycodes to the keymap for testing:

// c
[_LAYER] = LAYOUT(
    FG_EFFECT_NEXT, BG_EFFECT_NEXT, BG_TOGGLE, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
);



// 7. Layered Effects

// To further integrate per-layer effects for background animations,
// update the background effect based on the active layer:

// c
layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

    switch (layer) {
        case 0:
            effect_config.background_effect = RGB_MATRIX_CYCLE_LEFT_RIGHT;
            break;
        case 1:
            effect_config.background_effect = RGB_MATRIX_RAINBOW_MOVING_CHEVRON;
            break;
        // Add more layer-specific backgrounds here
        default:
            break;
    }

    return state;
}



// 8. Test Scenarios

//    1. Dynamic Switching: Verify that you can toggle foreground and
//       background effects independently using the keycodes.
//    2. Layer-Specific Backgrounds: Ensure that changing layers updates
//       the background animation.
//    3. Foreground Reactivity: Confirm that reactive effects respond
//       to keypresses without disrupting the background.


// This design modularizes the **foreground** and **background** rendering
// pipelines, enabling all effects to be mixed and matched dynamically.
// Would you like further refinements or additional test cases?

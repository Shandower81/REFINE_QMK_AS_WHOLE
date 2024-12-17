// implement a way to take the output of animation effects for all
// current matrix effects that have a foreground and background
// colors and be able to update these without changing the code
// for each effect.







// To create a framework where you can dynamically manage **foreground**
// and **background** colors for all matrix effects without modifying
// each effect individually, we can abstract the color handling into a
// shared **pipeline**. Here's how to implement this:


// 1. Centralized Foreground and Background Handlers

//  Define a Unified Effect Context
// This context will manage **foreground** and **background** color data and effects dynamically:

// c
typedef struct {
    rgb_t foreground_colors[DRIVER_LED_TOTAL];
    rgb_t background_colors[DRIVER_LED_TOTAL];
    bool use_background;  // Toggle background rendering
} effect_context_t;

static effect_context_t effect_context;



// 2. Abstract Foreground and Background Rendering

// Foreground Handler
// This function will apply the main effect's foreground rendering.

// c
void render_foreground(uint8_t led_index, const rgb_t color) {
    effect_context.foreground_colors[led_index] = color;
    rgb_matrix_set_color(led_index, color.r, color.g, color.b);
}


// Background Handler
// This function updates the background independently of the effect logic.

// c
void render_background(uint8_t led_index) {
    if (effect_context.use_background) {
        rgb_t bg_color = effect_context.background_colors[led_index];
        rgb_matrix_set_color(led_index, bg_color.r, bg_color.g, bg_color.b);
    }
}



// 3. Update Existing Effects to Use the Pipeline

// Each matrix effect will now delegate its **foreground rendering** to
// `render_foreground`. For example, for a solid reactive cross effect:

// c
bool rgb_matrix_solid_reactive_cross(effect_params_t *params) {
    if (!rgb_matrix_config.enable) return false;

    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        rgb_t color = {255, 0, 0};  // Example reactive red
        render_foreground(i, color);
    }
    return true;
}


// Add Background Rendering
// In the effect compositor (`rgb_matrix_effect_composite()` or equivalent),
// invoke `render_background` after applying the foreground rendering:

// c
bool rgb_matrix_effect_composite(effect_params_t *params) {
    if (!rgb_matrix_config.enable) return false;

    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        render_foreground(i, effect_context.foreground_colors[i]);
        render_background(i);
    }
    return true;
}



// 4. Add Dynamic Foreground/Background Updates

// Keycodes for Foreground/Background Modes
// Introduce keycodes to toggle between foreground and background updates:

// c
enum custom_keycodes {
    FOREGROUND_MODE = SAFE_RANGE,
    BACKGROUND_MODE_STATIC,
    BACKGROUND_MODE_RAINBOW,
    BACKGROUND_MODE_PER_KEY,
    BACKGROUND_TOGGLE,
    BACKGROUND_HUE_UP,
    BACKGROUND_HUE_DOWN,
    BACKGROUND_SAT_UP,
    BACKGROUND_SAT_DOWN,
    BACKGROUND_VAL_UP,
    BACKGROUND_VAL_DOWN
};


// Handle Keycodes in `process_record_user`
// Modify `process_record_user` to manage the background or foreground settings dynamically.

// c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case BACKGROUND_MODE_STATIC:
                effect_context.use_background = true;
                for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
                    effect_context.background_colors[i] = (rgb_t){128, 128, 255};  // Static blue
                }
                break;
            case BACKGROUND_MODE_RAINBOW:
                effect_context.use_background = true;
                // Example dynamic rainbow handler
                for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
                    effect_context.background_colors[i] = (rgb_t){rand() % 256, rand() % 256, rand() % 256};
                }
                break;
            case BACKGROUND_MODE_PER_KEY:
                effect_context.use_background = true;
                // Set per-key colors (hook into layer logic if needed)
                break;
            case BACKGROUND_TOGGLE:
                effect_context.use_background = !effect_context.use_background;
                break;
            case BACKGROUND_HUE_UP:
                // Adjust HSV for background
                break;
            default:
                break;
        }
    }
    return true;
}



// 5. Modular Background Rendering Logic

// Abstract Background Effects
// Define reusable functions for common background effects, such as
// static colors, rainbows, or per-key colors.

// c
void background_static_color(rgb_t color) {
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        effect_context.background_colors[i] = color;
    }
}

void background_rainbow_animation(void) {
    static uint8_t hue_offset = 0;
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        hsv_t hsv = {.h = (hue_offset + i * 10) % 256, .s = 255, .v = 128};
        hsv_to_rgb(&hsv, &effect_context.background_colors[i]);
    }
    hue_offset++;
}



// 6. Unified Update and Render Pipeline

// Matrix Effect Update
// Periodically update the background effect (e.g., in `matrix_scan_user`):

// c
void matrix_scan_user(void) {
    if (effect_context.use_background) {
        background_rainbow_animation();  // Example: Dynamic rainbow background
    }
}


// Unified Render Function
// Ensure all effects pass through the unified render pipeline:

// c
bool rgb_matrix_render(effect_params_t *params) {
    if (!rgb_matrix_config.enable) return false;

    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        // Foreground rendering by the current effect
        rgb_t fg_color = effect_context.foreground_colors[i];
        render_foreground(i, fg_color);

        // Background rendering
        render_background(i);
    }
    return true;
}



// 7. Keymap Integration

// Keymap Example
// Add keycodes for foreground/background controls:

// c
[_LAYER] = LAYOUT(
    FOREGROUND_MODE, BACKGROUND_MODE_STATIC, BACKGROUND_MODE_RAINBOW, BACKGROUND_MODE_PER_KEY,
    BACKGROUND_TOGGLE, BACKGROUND_HUE_UP, BACKGROUND_HUE_DOWN, BACKGROUND_SAT_UP, BACKGROUND_SAT_DOWN
);



// Advantages of This Design
// - Dynamic Updates: You can modify foreground and background behaviors
//     independently at runtime without editing each effect.
// - Extensibility: Add new background animations or behaviors without
//     touching the foreground effect logic.
// - Centralized Control: Unified rendering simplifies effect management.

// Would you like further refinement or additional example animations for testing?

// does this include both foreground and background operations in same
// code or do they both need separate code?









// The current code I provided focuses primarily on the **foreground effects**
// (such as managing the colors of the numeric row based on proximity to tens
// and units digits, with the logic for blinking and blending colors). However,
// it doesn't explicitly cover how to handle **both foreground and background
// effects** simultaneously. 

// To address both foreground and background:
// 1. **Foreground Effect**: The main effect for key presses, which in this
//    case is dynamic based on proximity to the tens and units digits, is
//    already handled in the code.
   
// 2. **Background Effect**: We can apply a different set of logic to control
//    the background color of the matrix. For example, if the background
//    effect is a solid color or an animation like static rainbow, it will
//    need to be applied to the matrix without interfering with the
//    dynamic foreground colors.

// Modified Approach for Both Foreground and Background Effects:

// To **combine both foreground and background effects** in the same rendering
// loop, you can layer them by rendering the background first, then overlay
// the foreground on top of it.


// 1. Background Effect Logic:

// The background effect will manage the overall matrix background, and the
// foreground (reactive or step effects) will overlay on top. We will need
// a way to blend or switch between these effects.

// c
void render_background_effect(effect_params_t *params) {
    // Determine the background effect type
    uint8_t background_effect = effect_config.background_effect;

    // Logic for rendering background (static color, rainbow, etc.)
    if (background_effect == BACKGROUND_SOLID_COLOR) {
        // Render solid color background (can use the background HSV color)
        rgb_matrix_set_color(ALL_KEYS, background_color.r, background_color.g, background_color.b);
    } else if (background_effect == BACKGROUND_RAINBOW) {
        // Static rainbow logic or other background effects
        apply_rainbow_effect();
    }
    // More background effects can be added as needed
}


// 2. Foreground Effect Logic:

// The foreground effect will handle reactive key presses and step-through effects.

// c
void render_foreground_effect(effect_params_t *params) {
    uint8_t current_effect = effect_config.foreground_effect;
    
    // Handle specific foreground effects, for example, reactive effects
    if (current_effect == EFFECT_REACTIVE) {
        render_reactive_effect();
    } else if (current_effect == EFFECT_SOLID_COLOR) {
        // Set solid color effect for foreground
        rgb_matrix_set_color(ALL_KEYS, foreground_color.r, foreground_color.g, foreground_color.b);
    }
    // More foreground effects can be added as needed
}


// 3. Updated RGB Matrix Render Function:

// Now, we combine both the background and foreground effects, ensuring that
// the **background effect** is applied first, and then the **foreground
// effect** (such as reactive keypress effects or step-through) is overlaid.

// c
void rgb_matrix_render(effect_params_t *params) {
    // Clear all LEDs (reset the state)
    clear_numeric_row_leds();
    
    // Apply the background effect
    render_background_effect(params);
    
    // Apply the foreground effect (reactive, solid, etc.)
    render_foreground_effect(params);
}



// Handling Foreground and Background Interaction:

// In cases where the background and foreground colors are intended to be
// distinct or work together (e.g., a background that is a solid color with
// a reactive foreground), **foreground effects** should be drawn over the
// **background matrix**.

// For example, with **reactive effects**, you'll want the background (which
// might be a rainbow effect or static color) to be present on all keys, but
// once a key is pressed, the foreground (the reactive effect) should override
// that key’s color without affecting the rest of the matrix.


// Handling Foreground (Reactive) with Background (Static or Animation):

// c
void render_foreground_reactive_effect() {
    // Check the currently active key for a reactive effect
    uint8_t active_key = get_active_key();

    // Adjust the color of the active key (foreground color) based on reactive effect
    if (active_key != INVALID_KEY) {
        rgb_matrix_set_color(active_key, reactive_foreground_color.r, reactive_foreground_color.g, reactive_foreground_color.b);
    }
}


// This `render_foreground_reactive_effect()` function will overlay reactive
// effects (foreground) on top of the static or animated background.


// Example of Complete Foreground + Background Handling for Effect 19:

// Let’s integrate the foreground (reactive effect) with the background for effect `19`.

// c
void rgb_matrix_render(effect_params_t *params) {
    uint8_t current_effect = effect_config.foreground_effect; // Retrieve active effect index
    
    // Apply background effect first (e.g., static rainbow or solid color)
    render_background_effect(params);
    
    // Handle repeated digits (foreground)
    handle_repeated_digit_effect(current_effect);
    
    // Now, apply foreground effect (e.g., reactive keys)
    render_foreground_effect(params);
    
    // Optional: Render any other effects (if necessary)
    apply_other_effects();
}



// Final Thoughts:

// - **Background Effects**: Should be applied first to set the base color
//   (static rainbow, solid color, etc.).
// - **Foreground Effects**: Overlayed on top of the background to react
//   to key presses (reactive, static, etc.).
// - **Blended Effects**: If you want blended foreground/background colors
//   (e.g., mixture of reactive color with a static rainbow background),
//   additional blending logic will be needed for each key where the
//   foreground and background overlap.

// Would you like further details or any other modifications to this behavior?

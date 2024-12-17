// what would happen with dynamic colors as each new effect is chosen?
// this was from previous output that i dont see anymore










// Dynamic colors for the top-row effect counter can be implemented
// by either cycling through pre-defined colors or generating colors
// programmatically based on the currently selected effect index.
// Let’s revisit and expand on how **dynamic colors** can work
// seamlessly when a new effect is chosen.


// What Are Dynamic Colors?
//   Dynamic colors change automatically based on the currently selected
//   effect. These colors could:
//    1. Map to Effect Types: Each effect has a unique, associated color.
//    2. Generate Procedurally: Use a formula to derive a color (e.g.,
//       hue progression or gradient).
//    3. Cycle or Animate: Colors of the top-row keys could cycle through
//       a pattern or gradient to enhance the visual appeal.


// Behavior When New Effects Are Chosen
//   1. Foreground Effects:
//      - When switching to a new foreground effect, the dynamic color
//        updates the corresponding keys on the top row.
//   2. Background Effects:
//      - When switching to a new background effect, a separate part of the
//        top row updates with colors representing the background effect.


// Dynamic Color Implementation Options

// 1. Predefined Colors Per Effect
// You can map each effect to a unique RGB color stored in a lookup table
// (`effect_colors[]`). When a new effect is chosen, the top-row LEDs
// update to reflect this color.

// c
rgb_t effect_colors[] = {
    {255, 0, 0},   // Effect 0: Red
    {0, 255, 0},   // Effect 1: Green
    {0, 0, 255},   // Effect 2: Blue
    {255, 255, 0}, // Effect 3: Yellow
    {255, 0, 255}, // Effect 4: Magenta
    {0, 255, 255}, // Effect 5: Cyan
    {128, 128, 128} // Effect 6: Gray
};


// In this case:
// - When the **foreground effect** changes, the first half of the
//   top-row LEDs update with the new color.
// - When the **background effect** changes, the second half of
//   the top-row LEDs update.


// 2. Procedural Color Generation
// Instead of hardcoding colors, you can compute colors dynamically
// using a formula. A common approach is using **HSV colors** (e.g.,
// a hue rotation) for visual consistency.

// c
void generate_dynamic_color(uint8_t effect_index, rgb_t *color) {
    hsv_t hsv = { .h = effect_index * (256 / RGB_MATRIX_EFFECT_TOTAL), .s = 255, .v = 255 };
    hsv_to_rgb(&hsv, color);
}


// - This generates a color by rotating the hue for each effect index.
// - The saturation (`s`) and value (`v`) remain constant for vibrant colors.

// Usage:
// Update the top row based on this generated color:
// c
void display_effect_counter_dynamic() {
    uint8_t top_row_keys[] = {0, 1, 2, 3};
    rgb_t color;

    // Foreground effect
    generate_dynamic_color(effect_config.foreground_effect, &color);
    for (uint8_t i = 0; i < 2; i++) {
        rgb_matrix_set_color(top_row_keys[i], color.r, color.g, color.b);
    }

    // Background effect
    generate_dynamic_color(effect_config.background_effect, &color);
    for (uint8_t i = 2; i < 4; i++) {
        rgb_matrix_set_color(top_row_keys[i], color.r, color.g, color.b);
    }
}



// 3. Animated Color Updates
// If you'd like the top-row LEDs to animate briefly when switching to a
// new effect, you can implement a **short transition animation**:

// c
void animate_effect_counter_transition(uint8_t key, rgb_t target_color) {
    static uint8_t step = 0;
    rgb_t current_color;

    // Gradually interpolate between the current and target color
    current_color.r = (current_color.r * (10 - step) + target_color.r * step) / 10;
    current_color.g = (current_color.g * (10 - step) + target_color.g * step) / 10;
    current_color.b = (current_color.b * (10 - step) + target_color.b * step) / 10;

    rgb_matrix_set_color(key, current_color.r, current_color.g, current_color.b);

    // Increment animation step
    if (++step < 10) {
        // Schedule next frame of the animation
        timer_set_timeout(10, animate_effect_counter_transition(key, target_color));
    }
}



// Behavior Summary with Dynamic Updates
// Here’s what happens when a **new effect** is chosen:
// 1. Foreground Effect Change:
//    - The keys representing the foreground effect on the top row instantly or
//      smoothly transition to the dynamic color for the selected foreground effect.

// 2. Background Effect Change:
//    - The keys representing the background effect transition to the dynamic
//      color corresponding to the background effect.

// 3. Combination Effects:
//    - Both foreground and background sections of the top row are updated
//      based on their respective dynamic colors.

// 4. Animations:
//    - You can make the top row animate (e.g., flash briefly or fade) to
//      visually highlight the effect change.


// Advantages of This Approach
// 1. Real-Time Feedback:
//    - Visual representation of the current effect, instantly available to the user.
// 2. Minimal Overhead:
//    - Color changes are handled within the existing rendering pipeline.
// 3. Customizable and Modular:
//    - Easily modify the color scheme or add animations for transitions.
// 4. Dynamic Integration:
//    - Works seamlessly regardless of how many effects or key layers are involved.


// Integration into the Main Rendering Pipeline
// To incorporate this logic:
// 1. Call `display_effect_counter_dynamic()` in the `rgb_matrix_render()` function.
// 2. If animations are enabled, call `animate_effect_counter_transition()`
//    after an effect change to trigger smooth transitions.

// Would you like assistance with testing, extending the animations, or
// integrating this into a larger configuration system?

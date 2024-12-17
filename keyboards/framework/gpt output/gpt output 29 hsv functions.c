// GPT-4o mini
// add counter for the effect in the list by showing the top rows keys
// as a specific color to represent the number of effects you are
// currently at.  would this be easier to implement that making a pop
// up telling the current effect?







// Adding a **counter** for the currently active effect in the list
// and displaying it on the top row of the keyboard is indeed a
// good approach to represent the current state visually. It would
// likely be more efficient than displaying a popup notification
// because it integrates directly with the LED matrix and doesn't
// require additional computation for a popup display.

// 1. Display Current Effect as a Counter on the Top Row
//   We can use the top row of the keys to represent the current
//   effect index. Each key in the top row will light up with a
//   color based on the effect number, which visually indicates
//   the current effect.

// Define LED Colors for Effects
// For each effect, assign a color that will be shown on the top
// row. This will help you visually indicate the current position
// in the effect list.

// c
rgb_t effect_colors[] = {
    {255, 0, 0},    // Red for Effect 0
    {0, 255, 0},    // Green for Effect 1
    {0, 0, 255},    // Blue for Effect 2
    {255, 255, 0},  // Yellow for Effect 3
    {255, 0, 255},  // Magenta for Effect 4
    {0, 255, 255},  // Cyan for Effect 5
    {128, 128, 128} // Gray for Effect 6
};


// Update the Top Row LEDs Based on the Effect Index
// Use the `effect_config.foreground_effect` and `effect_config.background_effect`
// values to update the top row keys with the corresponding colors.

// c
void display_effect_counter() {
    // Determine how many effects are currently active (background + foreground)
    uint8_t foreground_effect_index = effect_config.foreground_effect;
    uint8_t background_effect_index = effect_config.background_effect;

    // Top row key positions
    uint8_t top_row_keys[] = {0, 1, 2, 3};  // Adjust based on your keyboard layout

    // Update top row LEDs based on the current effect number
    rgb_t foreground_color = effect_colors[foreground_effect_index % sizeof(effect_colors) / sizeof(effect_colors[0])];
    rgb_t background_color = effect_colors[background_effect_index % sizeof(effect_colors) / sizeof(effect_colors[0])];

    // Set the foreground effect indicator on the first half of the top row
    for (uint8_t i = 0; i < 2; i++) {  // Assuming we want to represent the foreground effects on the first 2 keys
        rgb_matrix_set_color(top_row_keys[i], foreground_color.r, foreground_color.g, foreground_color.b);
    }

    // Set the background effect indicator on the second half of the top row
    for (uint8_t i = 2; i < 4; i++) {  // Assuming we want to represent the background effects on the next 2 keys
        rgb_matrix_set_color(top_row_keys[i], background_color.r, background_color.g, background_color.b);
    }
}


// You can adjust `top_row_keys[]` according to your layout. If you want more
// keys on the top row to show the effect index, just increase the array size
// and modify the number of effect rows accordingly.


// 2. Call the `display_effect_counter()` Function Regularly

// To update the top row based on the current effect, call
// `display_effect_counter()` within your matrix rendering or
// update loop. For example, within `rgb_matrix_render()` or a
// function that updates your key matrix:

// c
void rgb_matrix_render(effect_params_t *params) {
    // Render the background and foreground effects
    render_background_effect(i, params);
    render_foreground_effect(i, params);

    // Update the effect counter on the top row
    display_effect_counter();
}



// 3. Handle Effect Switching
// When you switch between effects using keycodes (e.g., `FG_EFFECT_NEXT`,
// `BG_EFFECT_NEXT`), the effect counter will automatically update the top
// row because it calls `display_effect_counter()` during rendering.


// 4. Optional: Use a Static Color Scheme
// If you don't want to use a different color for each effect but instead
// just want to show the current effect index with a static color gradient
// or bar, you can modify the `effect_colors[]` to use a single color for
// each effect step, such as varying shades of blue or red.

// c
rgb_t effect_colors[] = {
    {255, 0, 0},    // Effect 0
    {255, 50, 0},   // Effect 1
    {255, 100, 0},  // Effect 2
    {255, 150, 0},  // Effect 3
    {255, 200, 0},  // Effect 4
    {255, 255, 0},  // Effect 5
    {255, 255, 0}   // Effect 6
};



// 5. Comparison: Popup vs. Matrix Effect Display

// Advantages of Using the Matrix for Effect Display:
//   - No Pop-Up Interruption: The LED-based effect counter is
//     non-intrusive and doesn't require user interaction to dismiss.
//   - Real-Time Feedback: Users see immediate visual feedback on
//     the keyboard itself, making it easy to see the current effect status.
//   - More Efficient: The matrix is constantly being updated, so using
//     it for effect indication won't require additional computations
//     or interrupts like a popup would.
  
// Popup:
// - Pro: A popup might be more informative and can be used to display
//   detailed messages (e.g., show the full name of the effect).
// - Con: A popup would require additional logic for displaying and
//   dismissing messages. It could slow down the system due to additional processing.


// 6. Conclusion
// This approach allows for a **simple yet effective solution** for
// displaying the current effect index using the **top row of keys**.
// It is likely more efficient and user-friendly than implementing
// a popup notification for the effect change. The top row will show
// different colors based on the current effect index, providing a
// visual cue about which effect is active without disrupting the
// flow of the keyboard. Would you like further details on customizing
// this logic or additional test cases?

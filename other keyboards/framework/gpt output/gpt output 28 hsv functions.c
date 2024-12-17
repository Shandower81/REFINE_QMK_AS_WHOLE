// GPT-4o
// add counter for the effect in the list by showing the top rows keys
// as a specific color to represent the number of effects you are
// currently at.  would this be easier to implement that making a pop
// up telling the current effect?









// Adding a counter using the **top row keys** as a visual indicator of the
// current effect is a simpler and less intrusive solution compared to
// implementing a pop-up. Here's how to do it:


// 1. Define the Number of Keys for the Counter

// Choose how many keys in the top row will represent the counter.
// For a **4x6 matrix**, you can use the 4 keys in the first row
// (or all 6 if you prefer).


// 2. Define the Counter Update Logic

// Helper Function to Display the Counter

// c
void update_effect_counter(uint8_t current_effect, uint8_t total_effects) {
    uint8_t top_row_keys[] = {0, 1, 2, 3}; // Indices of the top row keys (adjust for your layout)
    uint8_t num_keys = sizeof(top_row_keys) / sizeof(top_row_keys[0]);

    // Calculate the number of keys to light up
    uint8_t keys_to_light = (current_effect * num_keys) / total_effects;

    // Loop through top row keys and set their colors
    for (uint8_t i = 0; i < num_keys; i++) {
        if (i < keys_to_light) {
            rgb_matrix_set_color(top_row_keys[i], 0, 255, 0); // Green for active keys
        } else {
            rgb_matrix_set_color(top_row_keys[i], 0, 0, 0);   // Off for inactive keys
        }
    }
}



// 3. Call the Counter Update When Effects Change

// In the keycode processing logic, call `update_effect_counter()`
// whenever the foreground or background effect changes.

// Example Integration in `process_record_user`

// c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case FG_EFFECT_NEXT:
                effect_config.foreground_effect = (effect_config.foreground_effect + 1) % RGB_MATRIX_EFFECT_TOTAL;
                update_effect_counter(effect_config.foreground_effect, RGB_MATRIX_EFFECT_TOTAL);
                break;

            case BG_EFFECT_NEXT:
                effect_config.background_effect = (effect_config.background_effect + 1) % RGB_MATRIX_EFFECT_TOTAL;
                update_effect_counter(effect_config.background_effect, RGB_MATRIX_EFFECT_TOTAL);
                break;

            default:
                break;
        }
    }
    return true;
}



// 4. Clear the Top Row When Effects Are Not Changing

// Add logic to clear the top row after a timeout or when the user switches to a different mode.

// Example: Clear Top Row After Timeout

// c
static uint32_t effect_counter_timer = 0;

void matrix_scan_user(void) {
    if (timer_elapsed32(effect_counter_timer) > 2000) { // Clear after 2 seconds
        uint8_t top_row_keys[] = {0, 1, 2, 3};
        uint8_t num_keys = sizeof(top_row_keys) / sizeof(top_row_keys[0]);

        for (uint8_t i = 0; i < num_keys; i++) {
            rgb_matrix_set_color(top_row_keys[i], 0, 0, 0);
        }
    }
}


// Reset the timer whenever the counter is updated:

// c
void update_effect_counter(uint8_t current_effect, uint8_t total_effects) {
    effect_counter_timer = timer_read32(); // Reset timer
    // ... (rest of the function as above)
}



// 5. Keymap Integration

// Define the top row indices in the `keymap.c` file to match your layout. For a 4x6 matrix:

// c
const uint8_t top_row_indices[] = {0, 1, 2, 3}; // Adjust based on your physical key arrangement



// 6. Testing

//    1. Effect Counter Updates: Confirm that the correct number of keys light
//       up when changing effects.
//    2. Clear Behavior: Ensure the counter clears after 2 seconds of inactivity.
//    3. Layer Handling: Verify that changing layers does not interfere with
//       the counter display.


// This implementation provides a clean and intuitive way to indicate the
// current effect using hardware, avoiding the need for more complex display
// integration. Would you like to refine this further or explore additional
// features like dynamic colors for the counter?

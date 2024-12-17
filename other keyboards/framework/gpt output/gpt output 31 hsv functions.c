// how about the top row slow blinks a number of keys for first
// digit of effect number in list then slow blink the second
// digit once get above single digits 







// Implementing a **slow blinking top row to indicate the effect
// number** (e.g., by blinking keys for the first and second digits
// of the effect index) is a creative and effective solution for
// visually displaying multi-digit effect numbers.

// Concept Overview
// 1. **Single-Digit Effect Numbers (0–9)**:
//    - Blink a number of keys equal to the effect index in the top row.
// 2. **Two-Digit Effect Numbers (10 and above)**:
//    - First blink the number of keys corresponding to the **tens digit**, then the **units digit**.
//    - There will be a brief pause between the two blinking sequences for clarity.


// Implementation Steps

// 1. Define the Key Positions for the Top Row
// The top row's keys should be identified based on your keyboard layout. For example:

// c
uint8_t top_row_keys[] = {0, 1, 2, 3}; // Adjust this for your layout
#define TOP_ROW_KEY_COUNT (sizeof(top_row_keys) / sizeof(top_row_keys[0]))


// 2. Define the Blinking Function
// This function will handle the blinking logic for a given digit.

// c
void blink_effect_digit(uint8_t digit, uint8_t offset) {
    // Clear the top row LEDs first
    for (uint8_t i = 0; i < TOP_ROW_KEY_COUNT; i++) {
        rgb_matrix_set_color(top_row_keys[i], 0, 0, 0);
    }

    // Blink LEDs for the digit, starting from 'offset'
    for (uint8_t i = 0; i < digit && (i + offset) < TOP_ROW_KEY_COUNT; i++) {
        rgb_matrix_set_color(top_row_keys[i + offset], 255, 255, 255); // White for blinking
    }
}


// 3. Add Timing Logic for Slow Blinking
// We can use a timer or a loop to alternate between "on" and "off" states for the blink.

// c
void display_effect_blink(uint8_t effect_index) {
    static uint8_t blink_state = 0; // 0 = off, 1 = on
    static uint16_t timer = 0;     // Timing variable

    // Ensure this function runs periodically
    if (timer_elapsed(timer) < 500) { // 500 ms interval for slow blinking
        return;
    }
    timer = timer_read();

    // Determine digits for the effect number
    uint8_t tens = effect_index / 10;
    uint8_t units = effect_index % 10;

    // Handle blinking states
    if (blink_state == 0) {
        // Blink tens digit first (if two digits)
        if (tens > 0) {
            blink_effect_digit(tens, 0); // Blink starting from key 0
        } else {
            blink_effect_digit(units, 0); // For single digits, blink units directly
        }
        blink_state = 1;
    } else if (blink_state == 1 && tens > 0) {
        // Blink units digit if tens were already blinked
        blink_effect_digit(units, 0); // Adjust offset if needed for large layouts
        blink_state = 0;
    }
}



// 4. Integrate into RGB Matrix Rendering
// Add this logic to the main `rgb_matrix_render()` loop, ensuring it
// executes during matrix updates.

// c
void rgb_matrix_render(effect_params_t *params) {
    // Standard RGB Matrix rendering logic
    render_background_effect(params);
    render_foreground_effect(params);

    // Display effect number blinking on the top row
    display_effect_blink(effect_config.foreground_effect);
}



// 5. Handle Special Cases
// - Effect Index Exceeds Top Row Length:
//   - If you have more effects than the number of keys in the top row,
//     you may need to truncate the representation or add scrolling logic.
// - Dynamic Color for Blinking:
//   - Replace the white color with a hue or RGB value tied to the effect
//     number for more visually distinct feedback.


// Behavior Summary
// - **Single-Digit Effects (0–9)**: The first `n` keys in the top row blink, where `n` is the effect index.
// - **Two-Digit Effects (10–99)**:
//    1. The first blinking sequence represents the **tens digit**.
//    2. After a short pause, the second sequence represents the **units digit**.
// - The blinking behavior alternates every 500ms (or the interval you set), creating a clear and consistent indication of the current effect.


// Advantages
// 1. **Simple and Non-Intrusive**:
//    - Uses the existing LED matrix without requiring overlays or extra UI components.
// 2. **Scalable**:
//    - Can represent both single and multi-digit indices effectively.
// 3. **Dynamic and Customizable**:
//    - The color, blinking speed, and representation logic can be easily adjusted.


// Would you like help with testing or extending this logic further? For
// example, would you like to add animations during the pause between
// tens and units, or would you prefer a static color scheme for clarity?

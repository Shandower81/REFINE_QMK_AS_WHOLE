// what about clear colors from numeric row leds and then a key with
// specific color based on the digits place (single digit number shows
// red) that matches the number key that is lit. when it is a two digit
// number the first digit color should be blue and second digit remains
// red.  if the number is a repeat of same number like 11 will slow
// blink on the number 1 key the first digit color and then slow blink
// the second color on same key that repeats until selection is
// reached or stepping to a new effect.








// This approach makes use of **specific keys on the numeric row** to
// represent the digits of the current effect index, with distinct
// colors for the **tens place** and **ones place** (e.g., blue for
// the first digit and red for the second digit). If the number
// contains the same digit twice (like `11`), it will alternate
// blinking the colors on the same key.

// Here’s how you can implement it:


// Implementation Steps

// 1. Define the Numeric Row Keys
// The numeric row corresponds to the `1-9` and `0` keys. Map these to
// their respective LED indices:

// c
uint8_t numeric_row_keys[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // LED indices for keys 1-0
#define NUMERIC_ROW_KEY_COUNT (sizeof(numeric_row_keys) / sizeof(numeric_row_keys[0]))



// 2. Clear All Numeric Row LEDs
// Before displaying the digits, make sure all LEDs on the numeric row are cleared.

// c
void clear_numeric_row_leds() {
    for (uint8_t i = 0; i < NUMERIC_ROW_KEY_COUNT; i++) {
        rgb_matrix_set_color(numeric_row_keys[i], 0, 0, 0);
    }
}



// 3. Blink the Digits
// Create a function to display the digits using the specific color logic.

// c
void blink_effect_index(uint8_t effect_index) {
    static uint8_t blink_state = 0; // 0 = off, 1 = on
    static uint16_t timer = 0;     // Timing variable

    // Ensure this function runs periodically (slow blink: 500ms interval)
    if (timer_elapsed(timer) < 500) {
        return;
    }
    timer = timer_read();

    // Clear all numeric row LEDs
    clear_numeric_row_leds();

    // Extract digits
    uint8_t tens = effect_index / 10; // Tens digit
    uint8_t units = effect_index % 10; // Units digit

    if (tens > 0) {
        if (blink_state == 0) {
            // Blink the tens place in blue
            rgb_matrix_set_color(numeric_row_keys[tens - 1], 0, 0, 255); // Blue
        } else if (tens == units) {
            // For repeated digits, alternate blue and red on the same key
            rgb_matrix_set_color(numeric_row_keys[tens - 1], 255, 0, 0); // Red
        }
    }

    if (blink_state == 1 || tens == 0) {
        // Blink the units place in red
        rgb_matrix_set_color(numeric_row_keys[units - 1], 255, 0, 0); // Red
    }

    // Toggle the blink state
    blink_state = !blink_state;
}



// 4. Handle Repeated Digits (e.g., `11`, `22`)
// For repeated digits, the same key will alternate between the two
// colors, ensuring clarity.


// 5. Integrate into RGB Matrix Rendering
// Add this blinking logic to your existing rendering pipeline to
// update during matrix effects:

// c
void rgb_matrix_render(effect_params_t *params) {
    // Standard rendering for foreground/background effects
    render_background_effect(params);
    render_foreground_effect(params);

    // Blink the numeric row to show the current effect index
    blink_effect_index(effect_config.foreground_effect);
}



// Behavior Example

// Single-Digit Effect (e.g., `5`)
// 1. Clear all numeric row LEDs.
// 2. Blink the **red LED** on the key for `5` every 500ms.

// Two-Digit Effect (e.g., `23`)
// 1. Clear all numeric row LEDs.
// 2. Blink the **blue LED** on the key for `2` for the first half of the blink cycle.
// 3. Blink the **red LED** on the key for `3` for the second half of the blink cycle.

// Repeated Digit Effect (e.g., `11`)
// 1. Clear all numeric row LEDs.
// 2. Blink the **blue LED** on the key for `1` for the first half of the blink cycle.
// 3. Blink the **red LED** on the key for `1` for the second half of the blink cycle.


// Advantages
// 1. Intuitive Feedback: Directly maps the effect index to numeric row keys,
//     which are easy to interpret.
// 2. Distinct Colors: Blue for the first digit and red for the second digit ensures clarity.
// 3. Simple Integration: Fits into the existing rendering loop without
//    additional UI requirements.

// Would you like to extend this behavior to dynamically update colors based
// on active layers or add animations for transitions?

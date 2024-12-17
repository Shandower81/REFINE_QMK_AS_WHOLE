// is this effect accessible through the mode step or does a new file need to be
// created in the quantum/rgb_matrix/animations folder to run the matrix effect?





// To make this effect accessible via `rgb_matrix_mode()` or the mode stepping
// (`rgb_matrix_step()`), it needs to be integrated into the RGB matrix effect
// system within QMK. By default, effects in QMK are stored in the
// `quantum/rgb_matrix/animations` folder, and each effect is registered in the
// RGB matrix handling logic.

// Here’s how to make the effect accessible:


// Option 1: Register as a Custom RGB Matrix Effect
// You can add your effect directly to the `quantum/rgb_matrix/animations`
// system by creating a new effect file. This is the best option for reusable,
// system-wide effects.

// Steps

// 1. Create a New Effect File
//    Create a file, e.g., `rgb_matrix_effect_composite.c`, in the
//    `quantum/rgb_matrix/animations/` folder.

// 2. Implement the Effect Logic
//    In this file, add your effect's logic using the `rgb_matrix_set_color()`
//    and other helper functions. Use the precomputed `hsv_foreground_matrix`
//    and `rgb_foreground_matrix`.

// c
#include "rgb_matrix.h"
#include "effects.h" // Include your helper file if needed

bool rgb_matrix_effect_composite(effect_params_t *params) {
    if (!rgb_matrix_config.enable) return false;

    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        composite_matrix_render(i);
    }

    return true; // Indicates the effect has been processed
}


// 3. Add the Effect to the Registry
//    Open `quantum/rgb_matrix/rgb_matrix.c` and register your new effect in the effect array:
// c
const rgb_matrix_effect_t RGB_MATRIX_EFFECTS[] = {
    ...
    rgb_matrix_effect_composite, // Add your effect here
    ...
};


// 4. Define the Effect Name
//    Update `quantum/rgb_matrix/rgb_matrix.h` with the effect's name:
// c
RGB_MATRIX_COMPOSITE_EFFECT,


// 5. Update `rgb_matrix_types.h`
//    Ensure the effect's ID is accessible:
// c
#define RGB_MATRIX_COMPOSITE_EFFECT (RGB_MATRIX_CUSTOM_EFFECT_START + X) // Choose an unused ID


//  Replace `X` with the next available effect number.

// 6. Rebuild the Firmware
//    Add the new source file to `rules.mk` in the QMK core directory:
// makefile
QUANTUM_SRC += rgb_matrix/animations/rgb_matrix_effect_composite.c



// Option 2: Use It Only in Your Keyboard's Implementation
// If you don't want to add this effect globally, you can integrate it directly
// into your keyboard’s `keymap.c` and manually trigger it via a key or mode.

// 1. Override `rgb_matrix_indicators_user()`
//    Instead of adding it to `quantum/rgb_matrix`, handle the rendering in your keyboard’s `keymap.c`:
// c
void rgb_matrix_indicators_user(void) {
    for (uint16_t i = 0; i < KEY_COUNT; i++) {
        composite_matrix_render(i);
    }
}


// 2. Assign It to a Mode
//    Use custom logic in `matrix_scan_user` to check the current mode:
// c
void matrix_scan_user(void) {
    if (rgb_matrix_get_mode() == MY_CUSTOM_EFFECT_MODE) {
        update_rainbow_background();
        decay_foreground_effects();
    }
}



// Option 3: Dynamically Switch Between Effects
// You can integrate the effect into the mode stepping logic:

// 1. Add the Effect to a New Mode
//    Use a custom mode ID in `config.h`:
// c
#define RGB_MATRIX_CUSTOM_COMPOSITE_MODE 50 // Use an ID beyond existing modes


// 2. Check the Mode in `matrix_scan_user`
//    Update the effect based on the current mode:
// c
void matrix_scan_user(void) {
    if (rgb_matrix_get_mode() == RGB_MATRIX_CUSTOM_COMPOSITE_MODE) {
        update_rainbow_background();
        decay_foreground_effects();
    }
}


// 3. Integrate Into Keymap
//    Use a keycode to set the mode dynamically:
// c
case RGB_MATRIX_CUSTOM_COMPOSITE_MODE:
    rgb_matrix_mode(RGB_MATRIX_CUSTOM_COMPOSITE_MODE);
    break;



// Summary
// - If you want the effect globally accessible in `rgb_matrix_mode()` and
//   other QMK effects, add it to `quantum/rgb_matrix/animations`.
// - If it’s specific to your keyboard, handle it in your `keymap.c` and invoke the effect logic directly.
// - Custom modes or user-defined keys can dynamically enable/disable the effect if it’s specific to your keymap.

// Would you like help implementing a specific option?

/* port latest qmk firmware to work with vial




 
Porting the latest QMK firmware to work with **VIAL** requires a few modifications to the QMK code and some setup in the VIAL software. VIAL is a configuration software that allows real-time keyboard configuration, including remapping keys, adjusting layers, and setting macros. It works similarly to QMK but requires specific code changes to make it compatible with VIAL's features.

Here’s a step-by-step guide on how to port QMK to VIAL:

### Steps to Port QMK Firmware to Work with VIAL

#### 1. **Set up your QMK Environment**:
Ensure you have the latest QMK environment set up on your computer. If you haven't done so, follow the official guide to install QMK:
- [QMK Setup Guide](https://docs.qmk.fm/#/newbs_getting_started)

#### 2. **Clone the VIAL Firmware Repository**:
VIAL has a custom firmware that is a fork of QMK, which includes the necessary changes for compatibility with VIAL. Start by cloning this repository:
*/
//bash
git clone https://github.com/vial-kb/vial-qmk.git
cd vial-qmk

/*
#### 3. **Set Up Your Keyboard in VIAL**:
Once you’ve cloned the VIAL repo, you can begin porting your keyboard’s QMK firmware. For this, you will need to identify your keyboard's QMK folder, as VIAL will need a version of the QMK firmware with VIAL-specific configurations.

- Find your keyboard folder in the QMK repository. For example, if you’re working with a 60% keyboard, find its folder in `qmk_firmware/keyboards/<your_keyboard>`.

#### 4. **Modify `config.h` for VIAL**:
To enable VIAL support, you need to modify your keyboard's `config.h` file. You'll need to define certain macros to tell QMK that your keyboard is VIAL-compatible.

In the `config.h` file, add:
*/
//c
#define VIAL_KEYBOARD  // This enables VIAL support
#define VIAL_TAP_DANCE // Optional: Enables tap dance support for VIAL

/*
- **VIAL_KEYBOARD**: This defines the keyboard as compatible with the VIAL software.
- **VIAL_TAP_DANCE**: This is optional but enables tap dance functionality, allowing keys to have different actions based on tap or hold behavior.

#### 5. **Modify `rules.mk`**:
In the `rules.mk` file of your keyboard folder, add the following lines to enable VIAL features and build with VIAL compatibility:
*/
//makefile
VIAL_KEYBOARD = yes
VIAL_TAP_DANCE_ENABLE = yes  # Optional, for tap dance support

/*
This tells the build system to include VIAL-specific code when compiling your keyboard's firmware.

#### 6. **Update Keymap to Support VIAL**:
The VIAL software uses an extended protocol that includes support for real-time keymap changes, so you don’t need to manually edit the firmware every time you change your keymap. Ensure that your keymap is compatible with VIAL.

If you're using advanced features like macros or layers, they should work out of the box, but if you have special requirements (like custom keycodes), ensure they are compatible.

In your `keymap.c` file, you can include VIAL-related functionality (such as custom keycodes or macros), but generally, you won't need to make changes for basic keymap functionality. VIAL will manage the keymap on the fly.

#### 7. **Compile the Firmware**:
Now that the necessary changes have been made, you can compile the firmware.
*/
//bash
qmk compile -kb <keyboard> -km <keymap>

/*
Replace `<keyboard>` with your specific keyboard name (e.g., `preonic`), and `<keymap>` with the name of your keymap (e.g., `default`).

#### 8. **Flash the Firmware**:
Once the firmware is compiled successfully, you can flash it onto your keyboard. This process will vary depending on your hardware. Typically, it will involve placing your keyboard into bootloader mode and using a tool like `qmk toolbox` or `dfu-programmer` to flash the firmware.
*/
//bash
qmk flash -kb <keyboard> -km <keymap>

/*
#### 9. **Install and Set Up VIAL**:
Once you’ve flashed the firmware, you can install and use the VIAL software to configure your keyboard in real time.

- Download the VIAL software from [VIAL's official website](https://get.vial.today/).
- Open VIAL and connect your keyboard. VIAL should recognize the firmware you just flashed and let you configure your keymap, layers, macros, and more through a graphical interface.

### Troubleshooting Tips:
- **Firmware Not Detected by VIAL**: If your keyboard doesn’t show up in VIAL, double-check that you’ve correctly defined `VIAL_KEYBOARD` in `config.h` and ensured your keyboard is running VIAL-compatible firmware.
- **Keymap Issues**: If VIAL isn’t registering keymaps or layers properly, ensure your keymap is set up as expected in QMK. Sometimes you may need to clear the EEPROM to reset any conflicting settings.
- **Custom Keycodes**: If you're using custom keycodes, you may need to implement a handler for those in `process_record_user` or other related functions.

### Additional Resources:
- **VIAL Documentation**: [VIAL Documentation](https://vial.project)
- **QMK Documentation**: [QMK Firmware Documentation](https://docs.qmk.fm/)
- **VIAL GitHub Repository**: [VIAL QMK Repo](https://github.com/vial-kb/vial-qmk)

By following these steps, you should be able to port your QMK keyboard firmware to work with VIAL, giving you the ability to configure it in real time through VIAL's graphical interface.
*/








/* what about if there is not a keyboard folder for my keyboard?




 
If there isn’t an existing folder for your keyboard in the QMK firmware repository, you’ll need to create a custom QMK keymap for your keyboard. This process involves defining your keyboard’s hardware and layout in a way that QMK can recognize and compile the firmware. Here’s how you can create a custom QMK folder and port it to work with **VIAL**:

### Steps to Create a Custom QMK Folder and Port to VIAL

#### 1. **Identify Your Keyboard’s Microcontroller**
   - Before starting, identify the microcontroller used by your keyboard (e.g., **ATmega32U4**, **STM32**, etc.). This will help you set up the correct hardware configuration.

   - You can usually find the microcontroller information on the manufacturer’s website or in the documentation that came with your keyboard.

#### 2. **Create a Custom Keyboard Folder in QMK**
   - **Clone QMK Firmware Repository**:
     First, if you haven't already, clone the QMK repository to your local machine:
*/
//   bash
     git clone https://github.com/qmk/qmk_firmware.git
     cd qmk_firmware
     

// - **Create a New Keyboard Folder**:
//   Now, navigate to the `keyboards` directory and create a folder for your keyboard. The folder should be named after your keyboard (e.g., `mykeyboard`).
//   bash
     mkdir keyboards/mykeyboard
     cd keyboards/mykeyboard
     
/*
   - **Create Essential Files**:
     Inside your keyboard’s folder, you’ll need to create several files to define your keyboard in QMK:
     - `config.h`: This file contains configuration options for your keyboard (e.g., defining the microcontroller, enabling features).
     - `keymap.c`: This file defines the keymap and how the keys should behave.
     - `rules.mk`: This file contains build rules and options specific to your keyboard.

     You can start with a basic configuration. Here’s an example of the minimal structure:
*/
//   bash
     touch config.h keymap.c rules.mk
     
/*
#### 3. **Define the Keyboard in `config.h`**
   In the `config.h` file, define key settings specific to your keyboard. Here’s an example for a keyboard using the **ATmega32U4** microcontroller:
*/
// c
   #pragma once

   #define VIAL_KEYBOARD  // Enable VIAL compatibility
   #define VIAL_TAP_DANCE_ENABLE  // Optional: Enable tap dance support for VIAL

   // Set the microcontroller and hardware type (for ATmega32U4)
   #define MCU_NAME atmega32u4

   // Optionally, set up additional features
   #define RGBLIGHT_ENABLE  // Enable RGB lighting (if your keyboard has LEDs)
   
/*
   Replace `atmega32u4` with the appropriate microcontroller for your board. If you don’t know the microcontroller, check the board’s documentation or look at other similar boards.

#### 4. **Set Up the Keymap in `keymap.c`**
   In the `keymap.c` file, define your keymap layout. Here’s a simple example for a basic 60% layout:
*/
// c
   #include QMK_KEYBOARD_H

   // Define layers (default layer and function layer)
   enum layers {
       _QWERTY,
       _FUNCTION,
   };

   // Define keycodes (you can use any standard QMK keycode)
   const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
       [_QWERTY] = LAYOUT_60_ansi(  // Basic 60% ANSI layout
           KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
           KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
           KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
           KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,  KC_BSPC, KC_RALT, KC_RGUI, KC_RCTL
       ),
       [_FUNCTION] = LAYOUT_60_ansi(  // Function layer for additional keys
           KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
           KC_F11,  KC_F12,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
           KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
           KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
       ),
   };
   
/*
   This layout defines two layers: `_QWERTY` (the default layer) and `_FUNCTION` (the function layer with F1-F12 keys). Modify this to fit your keyboard's actual layout.

#### 5. **Modify `rules.mk` for VIAL Support**
   In the `rules.mk` file, enable the required options for VIAL compatibility:
*/
// makefile
   VIAL_KEYBOARD = yes  # Enable VIAL support
   VIAL_TAP_DANCE_ENABLE = yes  # Optional: Enable tap dance
   
/*
#### 6. **Compile the Firmware**
   Now, compile your firmware for your keyboard using the following command:
*/
// bash
   qmk compile -kb mykeyboard -km default
   
/*
   Replace `mykeyboard` with the name of your keyboard folder, and `default` with the name of your keymap (e.g., `default`).

#### 7. **Flash the Firmware**
   After compilation, flash the firmware onto your keyboard using QMK’s flash command:
*/
// bash
   qmk flash -kb mykeyboard -km default
   
/*
   Make sure to follow the specific steps for your keyboard to enter bootloader mode before flashing.

#### 8. **Set Up VIAL**
   Once your keyboard is flashed with the QMK firmware that includes VIAL support, you can use the VIAL software to configure your keyboard in real-time.

   - **Download VIAL**: Get the VIAL software from [here](https://get.vial.today/).
   - Open VIAL and connect your keyboard. VIAL should detect your keyboard and allow you to configure keymaps, layers, macros, and more through the GUI.

### Final Notes:
- If your keyboard is a non-standard layout or uses special hardware features (like rotary encoders or custom switches), you’ll need to implement support for those features in your keymap or by modifying the QMK source code accordingly.
- Make sure to test your keyboard thoroughly to ensure that all keys, layers, and features work as expected.

By following these steps, you should be able to create a custom keyboard folder, set up VIAL compatibility, and successfully port your keyboard’s firmware to work with VIAL! Let me know if you need further assistance.
*/

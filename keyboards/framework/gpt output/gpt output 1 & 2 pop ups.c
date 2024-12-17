/* is it possible to have keyboard inputs pop up on display similar to sound volume changes and display brightness or caps lock and num lock



 
Yes, it is possible to have keyboard inputs pop up on a display in a way similar to how system notifications appear for things like sound volume changes, display brightness adjustments, or Caps Lock/Num Lock status changes. This functionality can be achieved through a combination of QMK firmware and the system's software (e.g., via a graphical display or notification system).

Approach:
    You would need two main components:
    1. **QMK Firmware**: This handles the detection of keypresses and sends the information to the operating system.
    2. **Operating System / Software**: This listens for the keypress events and displays notifications or pop-ups.

Here are two ways to achieve this:

    1. **Using QMK with OS-Level Notifications (e.g., on Windows, macOS, or Linux)**:
    The basic idea here is to send specific keypresses from the keyboard (via QMK) and use software on the computer to display notifications when certain keys or actions are triggered.

    - **QMK Firmware (Keypress Detection)**: You would modify your QMK firmware to send special key codes or combinations to trigger specific events or actions, like toggling Caps Lock, changing volume, or showing a notification.
    - **Software (Notification Handler)**: You would need an application running on your computer that listens for these specific keypress events (e.g., using HID, USB, or even custom macros). This software would trigger a notification on the screen when such keys are pressed.

For example:
    - **Keymaps in QMK**: In your keymap, you can map certain keys to trigger a key combination that the software will interpret as a command to show a notification.

Example in QMK Firmware:
*/
//c
enum custom_keycodes {
    VOLUME_UP = SAFE_RANGE,
    VOLUME_DOWN,
    SHOW_CAPS_LOCK,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case VOLUME_UP:
            if (record->event.pressed) {
                tap_code(KC_VOLU);  // Simulate volume up key press
            }
            break;
        case VOLUME_DOWN:
            if (record->event.pressed) {
                tap_code(KC_VOLD);  // Simulate volume down key press
            }
            break;
        case SHOW_CAPS_LOCK:
            if (record->event.pressed) {
                // Trigger custom behavior like showing Caps Lock status
                // You can send a specific keycode or a signal to your software here
            }
            break;
    }
    return true;
}

/*
Then, on the computer side, you would write a program or use an existing tool to listen for these keypresses. On Windows, for example, you could use a tool like **AutoHotkey** to display a notification when certain keycodes are pressed.

2. **Display On Keyboard (OLED or LCD)**:
    If you're looking to display information directly on a keyboard (such as on an OLED or LCD screen), QMK supports a variety of display integrations. The keyboard firmware itself can manage a small screen and update the display directly. For example, when Caps Lock is toggled or certain keys are pressed, the keyboard firmware can display a notification on the OLED.

Example with OLED Display in QMK:
    QMK supports OLED displays, and you can update the display to show key states or other information when specific keys are pressed.
*/
//c
#include QMK_KEYBOARD_H

// Initialize OLED display
void oled_task_user(void) {
    if (is_caps_word_on()) {
        oled_write_P(PSTR("Caps Lock ON"), false);
    } else {
        oled_write_P(PSTR("Caps Lock OFF"), false);
    }
}

// Define behavior for keypress
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_CAPS) {
        // Trigger visual feedback (change text on OLED display)
        oled_task_user();
    }
    return true;
}

/*
In this example, the OLED would show "Caps Lock ON" when Caps Lock is pressed, and "Caps Lock OFF" when it’s disabled. You can extend this to display other notifications like key presses, system states, etc.

### 3. **System Software Notifications**:
For more advanced usage (system-wide pop-up notifications), you would need to integrate with the system's notification system.

- **Windows**: You can use tools like AutoHotkey or a custom program that listens for specific key events and triggers Windows Toast notifications or popup messages.
- **macOS**: You can write a small program in Python, AppleScript, or Swift that listens for specific key events (via HID) and shows notifications using the `osascript` or `UserNotifications` framework.
- **Linux**: On Linux, you can use `xev` to detect key events and trigger notifications using `notify-send` for pop-up alerts.

For example, using **AutoHotkey** on Windows to show a notification when a specific key is pressed:
*/
//ahk
#Persistent
SetTimer, CheckCapsLock, 100

CheckCapsLock:
If (GetCapsLockState()) {
    Tooltip, Caps Lock is ON
} else {
    Tooltip, Caps Lock is OFF
}
return

/*
This script will display a tooltip when Caps Lock is toggled.

Conclusion:
    It is certainly possible to display keyboard inputs or system states like Caps Lock on-screen, either through a small display on the keyboard (like an OLED) or system-wide notifications. The QMK firmware can send the necessary keypress data, while the operating system or a custom program can handle the visual notifications. 

Let me know if you need help implementing one of these methods in more detail!
*/






/* which programs allows new toasts to be created




 
Several programs and libraries allow you to create new toasts (pop-up notifications) on different operating systems. Here’s a breakdown of options based on the platform you are using:

### 1. **Windows**:
   - **AutoHotkey**: A powerful scripting language for automating tasks on Windows. You can create toast notifications using the `Windows 10 Toast` library or `Toast` function in AutoHotkey.
     - **Example**:
*/
//     ahk
       Toast := ComObjCreate("WScript.Shell")
       Toast.Popup("This is a Toast notification!", 5, "Title", 64)
       
/* - **PowerShell**: You can use PowerShell scripts to trigger native toast notifications in Windows 10/11.
     - **Example**:
*/
//     powershell
       New-BurntToastNotification -Text "Hello, world!" -AppLogo "C:\Path\To\Icon.png"
       
/*   - This requires the **BurntToast** module to be installed: `Install-Module -Name BurntToast`.

   - **C# (Windows Desktop App)**: If you’re developing a Windows app, you can use the `Windows.UI.Notifications` API to create toasts.
     - **Example**:
*/
//     csharp
       var toastXml = ToastNotificationManager.GetTemplateContent(ToastTemplateType.ToastGeneric);
       var toastNotification = new ToastNotification(toastXml);
       ToastNotificationManager.CreateToastNotifier().Show(toastNotification);
       
/*
   - **Notifu**: A lightweight command-line tool for creating toast notifications in Windows.
     - **Example**:
*/
//       bash
       notifu /m "Hello World" /t "My Title"
       
/*
   - **Growl for Windows**: Growl is a notification system that supports toast notifications and can be used with various scripts and programs.

### 2. **macOS**:
   - **AppleScript**: You can use AppleScript to create native macOS notifications.
     - **Example**:
*/
//     applescript
       display notification "Hello, world!" with title "My Title"
       
/*   - This can be invoked from the terminal or from within macOS applications.

   - **Python (pync)**: You can use the `pync` library to send macOS notifications from a Python script.
     - **Installation**:
*/
//     bash
       pip install pync
       
//   - **Example**:
//     python
       from pync import Notifier
       Notifier.notify('Hello World', title='My Title')
       
/*
   - **Terminal (osascript)**: You can use `osascript` to send notifications directly from the terminal.
     - **Example**:
*/
//     bash
       osascript -e 'display notification "Hello, world!" with title "My Title"'
       
/*
   - **Swift**: If you’re developing an application in Swift, you can use the `NSUserNotificationCenter` to display notifications.

### 3. **Linux**:
   - **notify-send**: A command-line tool for creating notifications on Linux using the `libnotify` library.
     - **Example**:
*/
//     bash
       notify-send "Title" "This is a toast notification!"
       
/*
   - **Python (pynotifier)**: You can use `pynotifier` for simple notifications.
     - **Installation**:
*/
//     bash
       pip install pynotifier

//   - **Example**:
//     python
       from pynotifier import Notification
       Notification(
           title='My Title',
           description='Hello, world!',
           duration=5
       ).send()
       

// - **Zenity**: A tool for creating GTK+ dialogs in Linux, including notifications.
//   - **Example**:
//     bash
       zenity --info --text="Hello World"
       
/*
### 4. **Cross-Platform Solutions**:
   - **Node.js (node-notifier)**: If you're building a cross-platform application using Node.js, you can use `node-notifier` to send notifications.
     - **Installation**:
*/
//     bash
       npm install node-notifier
       
//   - **Example**:
//     javascript
       const notifier = require('node-notifier');
       notifier.notify({
           title: 'My Title',
           message: 'Hello, world!'
       });
       
/*
   - **Electron**: If you're building a desktop app with Electron, you can use its built-in notification API to create toasts on all major platforms (Windows, macOS, and Linux).
     - **Example**:
*/
//     javascript
       const { Notification } = require('electron');
       new Notification({ title: 'My Title', body: 'Hello, world!' }).show();
       
/*
### Summary:
- **Windows**: AutoHotkey, PowerShell (`BurntToast`), C# (Windows.UI.Notifications), Notifu
- **macOS**: AppleScript, Python (`pync`), `osascript`, Swift
- **Linux**: `notify-send`, Python (`pynotifier`), Zenity
- **Cross-Platform**: Node.js (`node-notifier`), Electron

Each of these tools offers a different way to trigger toast notifications based on your platform or development environment. Let me know if you need more specific guidance on one of these tools!
*/

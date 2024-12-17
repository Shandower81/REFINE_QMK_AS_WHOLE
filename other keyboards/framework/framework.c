// Copyright 2022 Framework Computer
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "framework.h"
#include "os_detection.h"
#include "factory.h"

void keyboard_post_init_kb(void) {
  keyboard_post_init_user();

  // Enable debug output
  debug_enable = true;
  debug_matrix = true;
  debug_keyboard = true;
}

/**
 * Hook into early keyboard initialization
*/
void keyboard_pre_init_kb(void) {
    keyboard_pre_init_user();

    // Mark boot as done.
    // Before this, when holding down both alt keys QSPI_SS is pulled low to put
    // the RP2040 in bootloader mode during reset.
    setPinOutput(BOOT_DONE_GPIO);
    writePinLow(BOOT_DONE_GPIO);

    //// TODO: Do we ever need to disable it to save power?
    setPinOutput(MUX_ENABLE_GPIO);
    writePinHigh(MUX_ENABLE_GPIO);

    setPinOutput(IS31FL3743A_ENABLE_GPIO);
#if defined(RGB_MATRIX_ENABLE)
    writePinHigh(IS31FL3743A_ENABLE_GPIO);
#else
    writePinLow(IS31FL3743A_ENABLE_GPIO);
#endif
    setPinInput(SLEEP_GPIO);
}

/**
 * Called by QMK when the keyboard suspends
 */
void suspend_power_down_kb(void) {
  suspend_power_down_user();

#ifdef RGB_MATRIX_ENABLE
#  ifndef NO_SUSPEND_POWER_DOWN
  writePinLow(IS31FL3743A_ENABLE_GPIO);
#  endif
#endif
}

/**
 * Called by QMK when the keyboard wakes up from suspend
 */
void suspend_wakeup_init_kb(void) {
  suspend_wakeup_init_user();

#ifdef RGB_MATRIX_ENABLE
#  ifndef NO_SUSPEND_POWER_DOWN
    writePinHigh(IS31FL3743A_ENABLE_GPIO);
#  endif
#endif
}

// If in BIOS mode, no matter what the keys have been remapped to, always send them as the F keys
bool bios_mode = true;
void set_bios_mode(bool val) {
  bios_mode = val;

#ifdef BIOS_HOTKEY_DEBUG
  if (bios_mode) {
    // Red
#if defined(RGB_MATRIX_ENABLE)
    rgb_matrix_sethsv_noeeprom(0, 0xFF, 0xFF);
    rgb_matrix_mode_noeeprom(1);
#endif
    writePin(GP24, 1);
  } else {
    // White
#if defined(RGB_MATRIX_ENABLE)
    rgb_matrix_sethsv_noeeprom(0, 0, 0xFF);
    rgb_matrix_mode_noeeprom(1);
#endif
    writePin(GP24, 0);
  }
#endif
}
bool handle_bios_hotkeys(uint16_t keycode, keyrecord_t *record) {
  // Not in bios mode, no special handling, handle as normal
  if (!bios_mode)
    return true;

  if (record->event.key.col == 5 && record->event.key.row == 3) {
      if (record->event.pressed) {
        register_code(KC_F1);
      } else {
        unregister_code(KC_F1);
      }
      return false;
  }
  if (record->event.key.col == 5 && record->event.key.row == 2) {
      if (record->event.pressed) {
        register_code(KC_F2);
      } else {
        unregister_code(KC_F2);
      }
      return false;
  }
  if (record->event.key.col == 4 && record->event.key.row == 6) {
      if (record->event.pressed) {
        register_code(KC_F3);
      } else {
        unregister_code(KC_F3);
      }
      return false;
  }
  if (record->event.key.col == 4 && record->event.key.row == 3) {
      if (record->event.pressed) {
        register_code(KC_F4);
      } else {
        unregister_code(KC_F4);
      }
      return false;
  }
  if (record->event.key.col == 10 && record->event.key.row == 4) {
      if (record->event.pressed) {
        register_code(KC_F5);
      } else {
        unregister_code(KC_F5);
      }
      return false;
  }
  if (record->event.key.col == 10 && record->event.key.row == 3) {
      if (record->event.pressed) {
        register_code(KC_F6);
      } else {
        unregister_code(KC_F6);
      }
      return false;
  }
  if (record->event.key.col == 10 && record->event.key.row == 2) {
      if (record->event.pressed) {
        register_code(KC_F7);
      } else {
        unregister_code(KC_F7);
      }
      return false;
  }
  if (record->event.key.col == 15 && record->event.key.row == 1) {
      if (record->event.pressed) {
        register_code(KC_F8);
      } else {
        unregister_code(KC_F8);
      }
      return false;
  }
  if (record->event.key.col == 11 && record->event.key.row == 3) {
      if (record->event.pressed) {
        register_code(KC_F9);
      } else {
        unregister_code(KC_F9);
      }
      return false;
  }
  if (record->event.key.col == 8 && record->event.key.row == 4) {
      if (record->event.pressed) {
        register_code(KC_F10);
      } else {
        unregister_code(KC_F10);
      }
      return false;
  }
  if (record->event.key.col == 8 && record->event.key.row == 6) {
      if (record->event.pressed) {
        register_code(KC_F11);
      } else {
        unregister_code(KC_F11);
      }
      return false;
  }
  if (record->event.key.col == 13 && record->event.key.row == 3) {
      if (record->event.pressed) {
        register_code(KC_F12);
      } else {
        unregister_code(KC_F12);
      }
      return false;
  }

  return true;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  process_record_user(keycode, record);

  os_variant_t os = detected_host_os();
  set_bios_mode(true);
#if defined(OS_DETECTION_RGB_DEBUG) && defined(RGB_MATRIX_ENABLE)
  rgb_matrix_mode_noeeprom(1);
  switch (os) {
      case OS_UNSURE:
        rgb_matrix_sethsv(0, 0, 255); // White
        break;
      case OS_LINUX:
        // Works on laptop and android
        rgb_matrix_sethsv(213, 255, 255); // purple
        break;
      case OS_WINDOWS:
        rgb_matrix_sethsv(170, 255, 255); // blue
        break;
      case OS_MACOS:
        rgb_matrix_sethsv(85, 255, 255); // green
        break;
      case OS_UEFI:
      case OS_IOS:
        // works on M1 mac
        rgb_matrix_sethsv(43, 255, 255); // yellow
        break;
      default:
        rgb_matrix_sethsv(125, 255, 255); // cyan
        break;
  }
#endif
  switch (os) {
      case OS_UNSURE:
        break;
      case OS_LINUX:
        set_bios_mode(false);
        break;
      case OS_WINDOWS:
        set_bios_mode(false);
        break;
      case OS_MACOS:
        break;
      case OS_UEFI:
      case OS_IOS:
        set_bios_mode(true);
        // works on M1 mac
        break;
  }
//#endif

  // Enable NKRO only in OS. Can cause issues with some pre-boot environments
  // Framework UEFI is ok, but I think grub can't handle it properly
  if (!bios_mode) {
    keymap_config.nkro = 1;
  }

  if (!handle_bios_hotkeys(keycode, record)) {
    return false;
  }

#ifdef RGB_MATRIX_ENABLE
  uint8_t h;
  uint8_t s;
  uint8_t v;
  uint8_t new_v;
  uint8_t x = 0;
  static uint8_t a = hsv_config.hsv_inc;

#endif

  switch (keycode) {
    // Implement step brightness for RGB backlight
#ifdef RGB_MATRIX_ENABLE
    case RGB_MODE_FORWARD:
    case RGB_MODE_REVERSE:
      // Turn on if it was off
      if (!rgb_matrix_is_enabled()) {
        rgb_matrix_enable();
      }
      return true;
    case BL_STEP:
      if (record->event.pressed) {
        h = rgb_matrix_get_hue();
        s = rgb_matrix_get_sat();
        v = rgb_matrix_get_val();
        switch (v) {
          default: // Default when user set to a different level
          case 0:
            new_v = 85;
            break;
          case 85:
            new_v = 170;
            break;
          case 170:
            new_v = 255;
            break;
          case 255:
            new_v = 0;
            break;
        }
        rgb_matrix_sethsv(h, s, new_v);
      }
      return true;
    case RGB_STEP:
        if (record->event.pressed) {
            x = hsv_config.brite_adj * 100;
            hsv_config.brite_adj = (get_mods() == (MOD_BIT(KC_LSFT) || MOD_BIT(KC_RSFT)) ? x - a > 0 ? x - a : x == 0 ? 101 : x == 101 ? 100 : 0 : x + a < 100 ? x + a : x == 101 ? 0 : x == 100 ? 101 : 100) / 100;
            hsv_config.rgb_switch = hsv_config.brite_adj == 101 ? 0 : 1;
        }
        return true;
    case HSV_INC:
        if (record->event.pressed) {
            a = hsv_config.hsv_inc * 100;
            hsv_config.hsv_inc = (get_mods() == (MOD_BIT(KC_LSFT) || MOD_BIT(KC_RSFT)) ? a / 2 >= 100 ? a / 2 : 10000 : a * 2 >= 10000 ? 100 : a * 2) / 100;}
        return true;
    case HSV_ABH:
    case HSV_ABS:
    case HSV_ABV:
    case HSV_STEP:
        if (record->event.pressed) {
            switch (keycode) {
              case HSV_ABH: x = hsv_config.anim_bkgnd_hsv.h; break;
              case HSV_ABS: x = hsv_config.anim_bkgnd_hsv.s; break;
              case HSV_ABV: x = hsv_config.anim_bkgnd_hsv.v; break;
              case HSV_STEP: x = rgb_matrix_config.hsv.v; break;}
            x = get_mods() == (MOD_BIT(KC_LSFT) || MOD_BIT(KC_RSFT)) ? x - a > 0 ? x - a : x == 0 ? 255 : 0 : x + a < 255 ? x + a : x == 255 ? 0 : 255;
            switch (keycode) {
              case HSV_ABH: hsv_config.anim_bkgnd_hsv.h = x; break;
              case HSV_ABS: hsv_config.anim_bkgnd_hsv.s = x; break;
              case HSV_ABV: hsv_config.anim_bkgnd_hsv.v = x; break;
              case HSV_STEP: rgb_matrix_config.hsv.v = x; break;}
        }
        return true;
#endif
    case KC_SCRN:
      // Simulate press WIN+P
      // Works (at least) on Windows and GNOME
      if (record->event.pressed) {
        register_code(KC_LGUI);
        register_code(KC_P);
      } else {
        unregister_code(KC_P);
        unregister_code(KC_LGUI);
      }
      return false; // Skip all further processing of this key
    default:
      return true; // Process all other keycodes normally
  }

  return true;
}

#ifdef BIOS_HOTKEY_DEBUG
// Ignore capslock updates in order to manually control it
bool led_update_kb(led_t led_state) {
    return false;
}
#endif


HSV rgb_to_hsv(RGB rgb) {
    HSV     hsv;
    uint8_t min, max, delta, r, g, b, h, s, v;
    r   = rgb.r;
    g   = rgb.g;
    b   = rgb.b;
    min = r < g ? (r < b ? r : b) : (g < b ? g : b);
    max = r > g ? (r > b ? r : b) : (g > b ? g : b);

    v     = max;
    delta = max - min;
    s     = delta < 0.00001 ? 0 : max > 0.0 ? delta / max : 0;
    h     = delta < 0.00001 ? 0 : max > 0.0 ? (r == max ? (g - b) / delta : g == max ? 2.0 + (b - r) / delta : 4.0 + (r - g) / delta) * 60 : 500;
    if (h == 500) {
#undef h
    }

    h = h < 0.0 ? h + 360 : h;

    hsv.h = h;
    hsv.s = s;
    hsv.v = v;

    return hsv;
}



__attribute__((weak)) HSV rgb_matrix_rgb_to_hsv(RGB rgb) {
    return rgb_to_hsv(rgb);
}

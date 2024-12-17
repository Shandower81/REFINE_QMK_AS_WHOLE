// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

/*******************************************************************************
  88888888888 888      d8b                .d888 d8b 888               d8b
      888     888      Y8P               d88P"  Y8P 888               Y8P
      888     888                        888        888
      888     88888b.  888 .d8888b       888888 888 888  .d88b.       888 .d8888b
      888     888 "88b 888 88K           888    888 888 d8P  Y8b      888 88K
      888     888  888 888 "Y8888b.      888    888 888 88888888      888 "Y8888b.
      888     888  888 888      X88      888    888 888 Y8b.          888      X88
      888     888  888 888  88888P'      888    888 888  "Y8888       888  88888P'
                                                        888                 888
                                                        888                 888
                                                        888                 888
     .d88b.   .d88b.  88888b.   .d88b.  888d888 8888b.  888888 .d88b.   .d88888
    d88P"88b d8P  Y8b 888 "88b d8P  Y8b 888P"      "88b 888   d8P  Y8b d88" 888
    888  888 88888888 888  888 88888888 888    .d888888 888   88888888 888  888
    Y88b 888 Y8b.     888  888 Y8b.     888    888  888 Y88b. Y8b.     Y88b 888
     "Y88888  "Y8888  888  888  "Y8888  888    "Y888888  "Y888 "Y8888   "Y88888
         888
    Y8b d88P
     "Y88P"
*******************************************************************************/

#pragma once

#ifndef BACKLIGHT_PIN
#    define BACKLIGHT_PIN GP25
#endif // BACKLIGHT_PIN

#ifndef DYNAMIC_KEYMAP_LAYER_COUNT
#    define DYNAMIC_KEYMAP_LAYER_COUNT 10
#endif // DYNAMIC_KEYMAP_LAYER_COUNT

#ifndef LED_CAPS_LOCK_PIN
#    define LED_CAPS_LOCK_PIN GP24
#endif // LED_CAPS_LOCK_PIN

#ifndef LED_NUM_LOCK_PIN
#    define LED_NUM_LOCK_PIN GP24
#endif // LED_NUM_LOCK_PIN

#ifndef DEBOUNCE
#    define DEBOUNCE 5
#endif // DEBOUNCE

#ifndef LAYOUT_default
#    define LAYOUT_default LAYOUT
#endif // LAYOUT_default

#ifndef DEVICE_VER
#    define DEVICE_VER 0x0029
#endif // DEVICE_VER

#ifndef MANUFACTURER
#    define MANUFACTURER "Framework"
#endif // MANUFACTURER

#ifndef PRODUCT
#    define PRODUCT "Laptop 16 Keyboard Module - ANSI"
#endif // PRODUCT

#ifndef PRODUCT_ID
#    define PRODUCT_ID 0x0012
#endif // PRODUCT_ID

#ifndef VENDOR_ID
#    define VENDOR_ID 0x32AC
#endif // VENDOR_ID

#ifndef MATRIX_COLS
#    define MATRIX_COLS 16
#endif // MATRIX_COLS

#ifndef MATRIX_ROWS
#    define MATRIX_ROWS 8
#endif // MATRIX_ROWS

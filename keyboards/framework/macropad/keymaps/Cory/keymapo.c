// Copyright 2022 Framework Computer
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "factory.h"
#include "print.h"      

enum _layers {
    _NUM_LOCK,
    _MOUSE_KEYS,
    _RGB_LYR,
    _RGB_SEL_LYR,
    _BOOT_LYR,
};

enum Custom_Keycodes {
    RGB_STEP,
    RGB_SFT,
    RGB_SEL,
    L0,
    L1,
    L2,
    L3,
    L4,
};

#define L0 TO(_NUM_LOCK)
#define L1 TO(_MOUSE_KEYS)
#define L2 TO(_RGB_LYR)
#define L3 TO(_RGB_SEL_LYR)
#define L4 TO(_BOOT_LYR)

// I could not find an RGB equivalent to the Backlight step brightness code
// so i made my own.
/*
enum _Custom_Keycodes {
    RGB_STEP,
    RGB_SFT,
    RGB_SEL,
};
*/
// these are variables for RGB_STEP. brite_adj is set initially
// to 255 so the rgb is max brightness when keyboard initializes.
static int rgb_brite_val = 0;
static int brite_adj     = 255;

// variable to switch between per key rgb and animations
static int rgb_switch = 1;

static int num_val = 0;
static int rgb_shift_val = 37;
static int x_val         = 0;

//              LED[][x] = 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
static int LED[15][24] = {{0, 0, 5, 0, 3, 3, 3, 8, 8, 0, 7, 0, 0, 8, 0, 0, 8, 0, 7, 8, 8, 0, 8, 0},
                          {0, 0, 3, 0, 0, 9, 9, 3, 2, 0, 7, 2, 0, 0, 0, 2, 2, 0, 7, 3, 3, 4, 3, 8},
                          {8, 3, 3, 8, 3, 0, 0, 3, 0, 8, 0, 0, 8, 0, 8, 0, 0, 8, 0, 3, 3, 6, 3, 0},

                          {0, 0, 8, 0, 3, 3, 3, 8, 5, 5, 5, 0, 5, 8, 5, 0, 5, 5, 5, 4, 4, 5, 4, 0},
                          {0, 0, 3, 0, 0, 9, 9, 3, 1, 1, 1, 2, 1, 0, 1, 2, 1, 1, 1, 0, 0, 1, 0, 8},
                          {8, 3, 3, 8, 3, 0, 0, 3, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0},

                          {0, 0, 0, 0, 8, 7, 3, 8, 8, 8, 8, 2, 8, 0, 8, 8, 0, 8, 0, 2, 0, 0, 0, 8},
                          {0, 0, 3, 3, 0, 7, 3, 0, 2, 1, 2, 2, 1, 8, 1, 0, 8, 1, 8, 2, 3, 8, 3, 0},
                          {8, 8, 3, 3, 0, 0, 0, 3, 8, 0, 8, 5, 0, 0, 0, 3, 0, 0, 0, 5, 3, 0, 3, 0},

                          {0, 0, 0, 9, 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 0, 0, 0, 0, 0, 0, 3, 8, 3, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 3, 0, 3, 8},

                          {0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 9, 0, 0, 0} };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*   _NUMLOCK_OFF :
        ┌───────-┬───────-┬───────-┬───────-┐
 4 keys │ ESCAPE │ OPEN   │FUNCTION│ BACK-  │
        │        │CALC APP│  TWO   │  SPACE │
        ├───────-┼───────-┼───────-┼───────-┤
 4 keys │NUMLOCK │ DIVIDE │MULTIPLY│ COMMA  │
        │ ON/OFF │        │        │        │
        ├──────-─┼───────-┼───────-┼───────-┤
 4 keys │ Home   │   ↑    │ Page   │ MINUS  │
        │        │        │ Up     │        │
        ├──────-─┼───────-┼───────-┼───────-┤
 4 keys │  ←     │ CLEAR  │   →    │  PLUS  │
        │        │        │        │        │
        ├──────-─┼───────-┼───────-┼───────-┤
 4 keys │ End    │    ↓   │ Page   │ EQUAL  │
        │        │        │ Down   │        │
        ├──────-─┼───────-┼───────-┼───────-┤
 4 keys │ OPEN   │ Insert │ Delete │ NUMPAD │
        │MOUSEKEY│        │        │  ENTER │
        └──────-─┴──────-─┴───────-┴───────-┘
 24 total

    _NUMLOCK_ON :
        ┌───────-┬───────-┬───────-┬───────-┐
        │ ESCAPE │ OPEN   │FUNCTION│ BACK-  │
        │        │CALC APP│   TWO  │  SPACE │
        ├───────-┼───────-┼───────-┼───────-┤
        │NUMLOCK │ DIVIDE │MULTIPLY│ COMMA  │
        │        │        │        │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │ NUMBER │ NUMBER │ NUMBER │ MINUS  │
        │ SEVEN  │ EIGHT  │  NINE  │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │ NUMBER │ NUMBER │ NUMBER │  PLUS  │
        │  FOUR  │  FIVE  │   SIX  │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │ NUMBER │ NUMBER │ NUMBER │ EQUAL  │
        │  ONE   │   TWO  │  THREE │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │ OPEN   │ NUMBER │ PERIOD │ NUMPAD │
        │MOUSEKEY│  ZERO  │        │ ENTER  │
        └───────-┴───────-┴───────-┴───────-┘
*/

    [_NUM_LOCK] = LAYOUT(
        KC_ESC,  KC_CALC, KC_F2,   KC_BSPC,
        KC_NUM,  KC_PSLS, KC_PAST, KC_PCMM,
        KC_P7,   KC_P8,   KC_P9,   KC_PMNS,
        KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_P1,   KC_P2,   KC_P3,   KC_EQL,
        L1,      KC_P0,   KC_PDOT, KC_PENT
    ),

   
/*   _MOUSE_KEYS :
        ┌───────-┬───────-┬───────-┬───────-┐
        │ ESCAPE | SWITCH │SWITCH  │ BACK-  │
        |        │APP WNDW│ WINDOW │  SPACE |
        ├───────-┼───────-┼───────-┼───────-┤
        │NUMLOCK │  UNDO  │  REDO  │  SAVE  │
        │  LAYER │        │        │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │ MOUSE  │MSWHEEL │ MOUSE  │  CUT   │
        │ BTTN 1 │   UP   │ BTTN 2 │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │MSWHEEL │MSCURSER│MSWHEEL │ PASTE  │
        │   LEFT │   UP   │  RIGHT │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │MSCURSER│MSWHEEL │MSCURSER│  COPY  │
        │   LEFT │   DOWN │  RIGHT │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │BOOT KEY│MSCURSER│  TAB   │ NUMPAD │
        │  LAYER │  DOWN  │        │  ENTER │
        └───────-┴───────-┴───────-┴──────-─┘
*/

    [_MOUSE_KEYS] = LAYOUT(
        KC_ESC,    A(KC_TAB), A(KC_F6),     KC_BSPC,
        L0,        C(KC_Z),   C(KC_Y),      C(KC_S),
        KC_BTN1,   KC_WH_U,   KC_BTN2,      C(KC_X),
        KC_WH_L,   KC_MS_U,   KC_WH_R,      C(KC_V),
        KC_MS_L,   KC_WH_D,   KC_MS_R,      C(KC_C),
        L2,        KC_MS_D,   KC_TAB,       KC_PENT
    ),


/*   _RGB_LYR :
        ┌───────-┬───────-┬───────-┬───────-┐
        │        │        │ RGB ++ │INCREASE│
        │        │        │  MODES │ SPEED  │
        ├───────-┼───────-┼───────-┼───────-┤
        │NUMLOCK │        │ RGB -- │DECREASE│
        │  LAYER │        │  MODES │ SPEED  │
        ├───────-┼───────-┼───────-┼───────-┤
        │        |INCREASE│INCREASE│INCREASE│
        │        |  HUE   │  SAT.  │ BRIGHT │
        ├───────-┼───────-┼───────-┼───────-┤
        │        |DECREASE│DECREASE│DECREASE│
        │        |  HUE   │  SAT.  │ BRIGHT │
        ├───────-┼───────-┼───────-┼───────-┤
        │        │        │        │        │
        │        │        │        │        │
        ├───────-┼───────-┼───────-┼───────-┤
        |RGB SEL │RGB STEP│ RGB    │BCKLGHT │
        |  LAYER │ BRIGHT │  TOGGLE│  STEP  │
        └───────-┴───────-┴───────-┴───────-┘
*/


    [_RGB_LYR] = LAYOUT(
        XXXXXXX, XXXXXXX,  RGB_MOD,  RGB_SPI,
        L0,      XXXXXXX,  RGB_RMOD, RGB_SPD,
        XXXXXXX, RGB_HUI,  RGB_SAI,  RGB_VAI,
        XXXXXXX, RGB_HUD,  RGB_SAD,  RGB_VAD,
        XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,
        L3,      RGB_STEP, RGB_TOG,  BL_STEP
    ),


/*   _RGB_SEL_LYR :
        ┌───────-┬───────-┬───────-┬───────-┐
        │KEYBOARD│KEYBOARD│KEYBOARD│KEYBOARD│
        │   "A"  │   "B"  │   "C"  │   "D"  │
        ├───────-┼───────-┼───────-┼───────-┤
        │KEYBOARD│KEYBOARD│KEYBOARD│KEYBOARD│
        │   "E"  │   "F"  │   "G"  │   "H"  │
        ├───────-┼───────-┼───────-┼───────-┤
        │KEYBOARD│KEYBOARD│KEYBOARD│KEYBOARD│
        │   "I"  │   "J"  │   "K"  │   "L"  │
        ├───────-┼───────-┼───────-┼───────-┤
        │KEYBOARD│KEYBOARD│KEYBOARD│KEYBOARD│
        │   "M"  │   "N"  │   "O"  │   "P"  │
        ├───────-┼───────-┼───────-┼───────-┤
        │KEYBOARD│KEYBOARD│KEYBOARD│KEYBOARD│
        │   "Q"  │   "R"  │   "S"  │   "T"  │
        ├───────-┼───────-┼───────-┼───────-┤
        | BOOT   │RGB STEP│RGB  ++ │ SELECT │
        |  LAYER │ BRIGHT │  STEP  │RGB KEY │
        └───────-┴───────-┴───────-┴───────-┘
*/


    [_RGB_SEL_LYR] = LAYOUT(
        KC_A, KC_B,     KC_C,    KC_D,
        KC_E, KC_F,     KC_G,    KC_H,
        KC_I, KC_J,     KC_K,    KC_L,
        KC_M, KC_N,     KC_O,    KC_P,
        KC_Q, KC_R,     KC_S,    KC_T,
        L4,   RGB_STEP, RGB_SFT, RGB_SEL
    ),


/*   _BOOT_LYR :
        ┌───────-┬───────-┬───────-┬───────-┐
        │        │        │        │  QK-   │
        │        │        │        │   BOOT │
        ├───────-┼───────-┼───────-┼───────-┤
        │        │        │        │        │
        │        │        │        │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │        │        │        │        │
        │        │        │        │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │        │        │        │        │
        │        │        │        │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │        │        │        │        │
        │        │        │        │        │
        ├───────-┼───────-┼───────-┼───────-┤
        │NUMLOCK │        │        │        │
        │  LAYER │        │        │        │
        └───────-┴───────-┴───────-┴───────-┘
*/


    [_BOOT_LYR] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        L0,      XXXXXXX, XXXXXXX, XXXXXXX
    ),
};


bool led_update_user(led_t led_state) {
    // Change layer if numlock state changes, either triggered by OS or
    // by numlock key on this keyboard
    if (led_state.num_lock) {
        uprintf("num_lock:0: rgb_shift_val: %u, rgb_switch: %u, num_val: %u\n", rgb_shift_val, rgb_switch, num_val);
        num_val = 1;
    } else {
        uprintf("num_lock:1: rgb_shift_val: %u, rgb_switch: %u, num_val: %u\n", rgb_shift_val, rgb_switch, num_val);
        num_val = 0;
    }
    return true;
};


bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case RGB_STEP:
            if (record->event.pressed) {
                x_val = 1;
                if (rgb_brite_val <= 0) {
                    rgb_brite_val = 1;
                } else if (rgb_brite_val <= 10) {
                    ++rgb_brite_val;
                    if (rgb_brite_val == 11) {
                        rgb_switch = 0;
                    }
                } else if (rgb_brite_val > 10) {
                    rgb_brite_val = 0;
                    rgb_switch    = 1;
                };
                //rgbv[x]: x =  0    1    2    3    4    5    6    7    8    9    10   11
                int rgbv[]   = {255, 230, 204, 178, 153, 128, 102, 77,  51,  26,  0,   255};
                brite_adj    = rgbv[rgb_brite_val];
            }
            uprintf("RGB_STEP:0: col: %u, row: %u, pressed: %u, rgb_shift_val: %u, rgb_switch: %u, num_val: %u\n", record->event.key.col, record->event.key.row, record->event.pressed, rgb_shift_val, rgb_switch, num_val);
            return brite_adj && rgb_brite_val;

        case RGB_SFT:
            if (record->event.pressed) {
                x_val = 1;
                uprintf("RGB_SFT:0: col: %u, row: %u, pressed: %u, rgb_shift_val: %u, rgb_switch: %u, num_val: %u\n", record->event.key.col, record->event.key.row, record->event.pressed, rgb_shift_val, rgb_switch, num_val);
                rgb_shift_val = rgb_shift_val > 36 ? 0 : (rgb_shift_val + 1);
                uprintf("RGB_SFT:1: col: %u, row: %u, pressed: %u, rgb_shift_val: %u, rgb_switch: %u, num_val: %u\n", record->event.key.col, record->event.key.row, record->event.pressed, rgb_shift_val, rgb_switch, num_val);
            }
            return rgb_shift_val;

    }
    return false;
};



/*  Zeroed Key Numbers for Assigning LED Colors
          ┌───-┬───-┬───-┬───-┐
          │ 0  │ 1  │ 2  │ 3  │
          ├───-┼──-─┼───-┼───-┤
          │ 4  │ 5  │ 6  │ 7  │
          ├────┼-──-┼───-┼───-┤
          │ 8  │ 9  │ 10 │ 11 │
          ├─-──┼────┼───-┼─-──┤
          │ 12 │ 13 │ 14 │ 15 │
          ├──-─┼──-─┼──-─┼──-─┤
          │ 16 │ 17 │ 18 │ 19 │
          ├───-┼──-─┼──-─┼───-┤
          │ 20 │ 21 │ 22 │ 23 │
          └──-─┴───-┴───-┴──-─┘

 RGB_MATRIX_INDICATOR_SET_COLOR( [LED INDEX from above diagram] , RED (Range 0 to 243) , GREEN (Range 0 to 243) , BLUE (Range 0 to 243) )

      KS0[] = { Zeroed Key Numbers used here }

      KS1[] = { UnZeroed Key Numbers used here }
*/


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (rgb_switch == 1) {
        if (x_val == 1) {
            uprintf("rgb_switch:0: rgb_shift_val: %u, rgb_switch: %u, num_val: %u\n", rgb_shift_val, rgb_switch, num_val);
            uprintf("rgb_switch:1: _NUM_LOCK: %u, _MOUSE_KEYS: %u, _RGB_LYR: %u, _RGB_SEL_LYR: %u, _BOOT_LYR: %u\n", IS_LAYER_ON(_NUM_LOCK), IS_LAYER_ON(_MOUSE_KEYS), IS_LAYER_ON(_RGB_LYR), IS_LAYER_ON(_RGB_SEL_LYR), IS_LAYER_ON(_BOOT_LYR));
        }
        int a = 0;
        if (IS_LAYER_ON(_NUM_LOCK)) {
            a = num_val < 1 ? 0 : 3;
        } else if (IS_LAYER_ON(_MOUSE_KEYS)) {
            a = 6;
        } else if (IS_LAYER_ON(_RGB_LYR)) {
            a = 9;
        } else if (IS_LAYER_ON(_BOOT_LYR)) {
            a = 12;
        }
        if (x_val == 1) {
            uprintf("IS_LAYER_ON:0: a: %u, _NUM_LOCK: %u, _MOUSE_KEYS: %u, _RGB_LYR: %u, _RGB_SEL_LYR: %u, _BOOT_LYR: %u\n", a, IS_LAYER_ON(_NUM_LOCK), IS_LAYER_ON(_MOUSE_KEYS), IS_LAYER_ON(_RGB_LYR), IS_LAYER_ON(_RGB_SEL_LYR), IS_LAYER_ON(_BOOT_LYR));
        }
        for (int b = 0; b <= 23; b++) {
            int LEDr = ((LED[0 + a][b] * 10) / 100) * 255;
            if (x_val == 1) {
                uprintf("LEDr:0: a: %3u, b: %3u, LEDr: %4u, _NUM_LOCK: %u, _MOUSE_KEYS: %u, _RGB_LYR: %u, _RGB_SEL_LYR: %u, _BOOT_LYR: %u\n", a, b, LEDr, IS_LAYER_ON(_NUM_LOCK), IS_LAYER_ON(_MOUSE_KEYS), IS_LAYER_ON(_RGB_LYR), IS_LAYER_ON(_RGB_SEL_LYR), IS_LAYER_ON(_BOOT_LYR));
            }
            int LEDg = ((LED[1 + a][b] * 10) / 100) * 255;
            if (x_val == 1) {
                uprintf("LEDg:0: a: %3u, b: %3u, LEDr: %4u, LEDg: %4u, _NUM_LOCK: %u, _MOUSE_KEYS: %u, _RGB_LYR: %u, _RGB_SEL_LYR: %u, _BOOT_LYR: %u\n", a, b, LEDr, LEDg, IS_LAYER_ON(_NUM_LOCK), IS_LAYER_ON(_MOUSE_KEYS), IS_LAYER_ON(_RGB_LYR), IS_LAYER_ON(_RGB_SEL_LYR), IS_LAYER_ON(_BOOT_LYR));
            }
            int LEDb = ((LED[2 + a][b] * 10) / 100) * 255;
            if (x_val == 1) {
                uprintf("LEDb:0: a: %3u, b: %3u, LEDr: %4u, LEDg: %4u, LEDb: %4u, _NUM_LOCK: %u, _MOUSE_KEYS: %u, _RGB_LYR: %u, _RGB_SEL_LYR: %u, _BOOT_LYR: %u\n", a, b, LEDr, LEDg, LEDb, IS_LAYER_ON(_NUM_LOCK), IS_LAYER_ON(_MOUSE_KEYS), IS_LAYER_ON(_RGB_LYR), IS_LAYER_ON(_RGB_SEL_LYR), IS_LAYER_ON(_BOOT_LYR));
            }
                RGB_MATRIX_INDICATOR_SET_COLOR(b, LEDr * brite_adj / 255, LEDg * brite_adj / 255, LEDb * brite_adj / 255);
        }
    }

    if (IS_LAYER_ON(_RGB_SEL_LYR) && rgb_switch == 1) {
        if (x_val == 1) {
            uprintf("SEL_LYR:0: rgb_shift_val: %u, rgb_switch: %u, num_val: %u\n", rgb_shift_val, rgb_switch, num_val);
            uprintf("SEL_LYR:1: _NUM_LOCK: %u, _MOUSE_KEYS: %u, _RGB_LYR: %u, _RGB_SEL_LYR: %u, _BOOT_LYR: %u\n", IS_LAYER_ON(_NUM_LOCK), IS_LAYER_ON(_MOUSE_KEYS), IS_LAYER_ON(_RGB_LYR), IS_LAYER_ON(_RGB_SEL_LYR), IS_LAYER_ON(_BOOT_LYR));
        }
        //           a     =  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36
        int LEDr[] = {36,  128, 191, 255, 255, 255, 255, 255, 191, 128, 36,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 191, 128, 36,  0,   255, 255, 255 };
        int LEDg[] = {0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 255, 191, 128, 36,  0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 191, 128, 36,  0,   0,   0,   0,   0,   0,   0,   0   };
        int LEDb[] = {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 191, 128, 36  };
        int a;
        int b = rgb_shift_val;
        if (x_val == 1) {
            uprintf("SEL_LYR:2: rgb_shift_val: %u, rgb_switch: %u, num_val: %u\n", rgb_shift_val, rgb_switch, num_val);
        }
        for (a = 0; a <= 23; a++) {
            if (x_val == 1) {
                uprintf("SEL_LYR:3: rgb_shift_val: %u, rgb_switch: %u, num_val: %u\n", rgb_shift_val, rgb_switch, num_val);
                uprintf("SEL_LYR:4: a: %3u, b: %3u, LEDr[b]: %4u, LEDg[b]: %4u, LEDb[b]: %4u\n", a, b, LEDr[b], LEDg[b], LEDb[b]);
            }
            b = b > 36 ? 0 : (b + 1);
            if (x_val == 1) {
                uprintf("SEL_LYR:5: a: %3u, b: %3u, LEDr[b]: %4u, LEDg[b]: %4u, LEDb[b]: %4u\n", a, b, LEDr[b], LEDg[b], LEDb[b]);
                uprintf("SEL_LYR:6: a: %3u, b: %3u, LEDr: %5u, LEDg: %5u, LEDb: %5u\n", a, b, LEDr[b] * brite_adj / 255, LEDg[b] * brite_adj / 255, LEDb[b] * brite_adj / 255);
            }
            RGB_MATRIX_INDICATOR_SET_COLOR(a, LEDr[b] * brite_adj / 255, LEDg[b] * brite_adj / 255, LEDb[b] * brite_adj / 255);
        }
    }
    x_val = 0;

    return false;
};

/*
┌───────-┬───────-┬───────-┬───────-┐ ┌───────-┬───────-┬───────-┬───────-┐┌───────-┬───────-┬───────-┬───────-┐
│ ESCAPE │ OPEN   │FUNCTION│ BACK-  │ │ ESCAPE │ OPEN   │FUNCTION│ BACK-  ││ ESCAPE | SWITCH │SWITCH  │ BACK-  |
│        │CALC APP│  TWO   │  SPACE │ │        │CALC APP│   TWO  │  SPACE ││        |APP WNDW│ WINDOW │  SPACE |
├───────-┼───────-┼───────-┼───────-┤ ├───────-┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤
│NUMLOCK │                          │ │NUMLOCK │                          ││  BOOT  │                          │
│        │                          │ │        │                          ││ LAYER  │                          │
├──────-─┼                          ┤ ├───────-┼                          ┤├───────-┼                          ┤
│                                   │ │                                   ││                                   │
│                                   │ │                                   ││                                   │
├                                   ┤ ├                                   ┤├                                   ┤
│                                   │ │                                   ││                                   │
│                                   │ │                                   ││                                   │
├                                   ┤ ├                                   ┤├                                   ┤
│                                   │ │                                   ││                                   │
│                                   │ │                                   ││                                   │
├──────-─┼                          ┤ ├───────-┼                          ┤├───────-┼                 ┼───────-┤
│ OPEN   │                          │ │ OPEN   │                          ││ LAYER  │                 │ NUMPAD │
│MOUSEKEY│                          │ │MOUSEKEY│                          ││    0   │                 │  ENTER │
└──────-─┴──────-─┴───────-┴───────-┘ └───────-┴───────-┴───────-┴───────-┘└───────-┴───────-┴───────-┴──────-─┘

┌───────-┬───────-┬───────-┬───────-┐┌───────-┬───────-┬───────-┬───────-┐┌───────-┬───────-┬───────-┬───────-┐
│Numlock │ OPEN   │FUNCTION│ BACK - ││Numlock │ OPEN   │ ESCAPE │ BACK - ││ SWITCH │SWITCH  │FUNCTION| ESCAPE │
│        │CALC APP│  TWO   │  SPACE ││        │CALC APP│        │  SPACE ││APP WNDW│ WINDOW │  TWO   |        │
├───────-┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤
│        │ DIVIDE │MULTIPLY│ COMMA  ││        │ DIVIDE │MULTIPLY│ COMMA  ││ LAYER  │ UNDO   │  REDO  │  SAVE  │
│        │        │        │        ││        │        │        │        ││    0   │        │        │        │
├──────-─┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤
│ Home   │   ↑    │ Page   │ MINUS  ││ NUMBER │ NUMBER │ NUMBER │ MINUS  ││ MOUSE  │MSWHEEL │ MOUSE  │  CUT   │
│        │        │ Up     │        ││ SEVEN  │ EIGHT  │  NINE  │        ││ BTTN 1 │   UP   │ BTTN 2 │        │
├──────-─┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤
│  ←     │ CLEAR  │   →    │  PLUS  ││ NUMBER │ NUMBER │ NUMBER │  PLUS  ││MSWHEEL │MSCURSER│MSWHEEL │ PASTE  │
│        │        │        │        ││  FOUR  │  FIVE  │   SIX  │        ││   LEFT │   UP   │  RIGHT │        │
├──────-─┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤
│ End    │    ↓   │ Page   │ EQUAL  ││ NUMBER │ NUMBER │ NUMBER │ EQUAL  ││MSCURSER│MSWHEEL │MSCURSER│  COPY  │
│        │        │ Down   │        ││  ONE   │   TWO  │  THREE │        ││   LEFT │   DOWN │  RIGHT │        │
├──────-─┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤├───────-┼───────-┼───────-┼───────-┤
│ OPEN   │ Insert │ Delete │ NUMPAD ││ OPEN   │ NUMBER │ PERIOD │ NUMPAD ││ BACK - │MSCURSER│  TAB   │ QK -   │
│MOUSEKEY│        │        │  ENTER ││MOUSEKEY│  ZERO  │        │ ENTER  ││  SPACE │  DOWN  │        │   BOOT │
└──────-─┴──────-─┴───────-┴───────-┘└───────-┴───────-┴───────-┴───────-┘└───────-┴───────-┴───────-┴──────-─┘
*/







/*


    if (IS_LAYER_ON(_NUM_LOCK) && num_val == 1 && rgb_switch == 1) {
        //  a      =  0
        int lmin[] = {0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23 };
        int lmax[] = {1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24 };
        int a;
        int LEDr[] = {36,  128, 191, 255, 255, 255, 255, 255, 191, 128, 36,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 191, 128, 36,  0,   255, 255, 255,   };
        int LEDg[] = {0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 255, 191, 128, 36,  0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 191, 128, 36,  0,   0,   0,   0,   0,   0,   0,   0,     };
        int LEDb[] = {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 191, 128, 36,    };
        for (a = 0; a <= 23; a++) {
            led_min = lmin[a]
            led_max = lmax[a];
            for (uint8_t i = led_min; i < led_max; i++) {
                RGB_MATRIX_INDICATOR_SET_COLOR(i, LEDr[a] * brite_adj / 255, LEDg[a] * brite_adj / 255, LEDb[a] * brite_adj / 255);
            }
        }
    }



        if (IS_LAYER_ON(_MOUSE_KEYS) && rgb_switch == 1) {
        //  a      =  0
        int lmin[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
        int lmax[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
        int a;
        int LEDr[] = {0,  0,  0,  0,  8,  7,  3,  8,  8,  8,  8,  2,  8,  0,  8,  8,  0,  8,  0,  2,  0,  0,  0,  8 };
        int LEDg[] = {0,  0,  3,  3,  0,  7,  3,  0,  2,  1,  2,  2,  1,  8,  1,  0,  8,  1,  8,  2,  3,  8,  3,  0 };
        int LEDb[] = {8,  8,  3,  3,  0,  0,  0,  3,  8,  0,  8,  5,  0,  0,  0,  3,  0,  0,  0,  5,  3,  0,  3,  0 };
        for (a = 0; a <= 23; a++) {
            led_min = lmin[a];
            led_max = lmax[a];
            for (uint8_t i = led_min; i < led_max; i++) {
                RGB_MATRIX_INDICATOR_SET_COLOR(i, LEDr[a] * brite_adj / 255, LEDg[a] * brite_adj / 255, LEDb[a] * brite_adj / 255);
            }
        }
    }



    if (IS_LAYER_ON(_BOOT_LYR) && rgb_switch == 1) {
        //  a      =  0
        int lmin[] = {0,  1,  2,  3,   4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
        int lmax[] = {1,  2,  3,  4,   5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
        int a;
        int LEDr[] = {0,  0,  0,  255, 8,  0,  0,  0,  0,  0,  0,  0,  8,  0,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0 };
        int LEDg[] = {0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  1,  8,  0,  0,  0,  0,  0,  0,  3,  8,  3,  0 };
        int LEDb[] = {0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  0,  0,  0,  0,  3,  0,  3,  8 };
        for (a = 0; a <= 23; a++) {
            led_min = lmin[a];
            led_max = lmax[a];
            for (uint8_t i = led_min; i < led_max; i++) {
                RGB_MATRIX_INDICATOR_SET_COLOR(i, LEDr[a] * brite_adj / 255, LEDg[a] * brite_adj / 255, LEDb[a] * brite_adj / 255);
            }
        }
    }




*/

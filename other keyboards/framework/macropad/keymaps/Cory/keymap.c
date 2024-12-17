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
    RGB_SFT  = SAFE_RANGE,
    RGB_SEL,
    LYR_SFT
};

#define L0 TO(_NUM_LOCK)
#define L1 TO(_MOUSE_KEYS)
#define L2 TO(_RGB_LYR)
#define L3 OSL(_RGB_SEL_LYR)
#define L4 TO(_BOOT_LYR)

// these are variables for RGB_STEP. brite_adj is set initially
// to 255 so the rgb is max brightness when keyboard initializes.
static int rgb_brite_val = 0;
static int brite_adj     = hsv_config.brite_adj;

// variable to switch between per key rgb and animations
static int rgb_switch = hsv_config.rgb_switch;

static int num_val = 0;
static int rgb_shift_val = 0;

static int max_lyrs = 4; // zeroed value
static int pck_lyr = 0;
static bool select_active = 0;

//              LED[][x] =   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24
static int LED[15][24] = {{  0,   0,   5,   0,   3,   3,   3,   8,   8,   0,   7,   0,   0,   8,   0,   0,   8,   0,   7,   8,   8,   0,   8,   0},
                          {  0,   0,   3,   0,   0,   9,   9,   3,   2,   0,   7,   2,   0,   0,   0,   2,   2,   0,   7,   3,   3,   4,   3,   8},
                          {  8,   3,   3,   8,   3,   0,   0,   3,   0,   8,   0,   0,   8,   0,   8,   0,   0,   8,   0,   3,   3,   6,   3,   0},
//              LED[][x] =  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24
                          {170, 170,   0, 170, 213,  71,  71,   0,  11, 170,  43,  85, 170,   0, 170,  85,  11, 170,  43,   0,   0, 142,   0,  85},
                          {255, 255, 255, 255, 255, 255, 255, 158, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 158, 158, 255, 158, 255},
                          {  8,   3,   5,   8,   3,  10,  10,   8,   8,   8,   8,   3,   8,   8,   8,   3,   8,   8,   8,   8,   8,   5,   8,   8},

//              LED[][x] =   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23 24
                          {  0,   0,   8,   0,   3,   3,   3,   8,   5,   5,   5,   0,   5,   8,   5,   0,   5,   5,   5,   4,   4,   5,   4,   0},
                          {  0,   0,   3,   0,   0,   9,   9,   3,   1,   1,   1,   2,   1,   0,   1,   2,   1,   1,   1,   0,   0,   1,   0,   8},
                          {  8,   3,   3,   8,   3,   0,   0,   3,   1,   1,   1,   0,   1,   0,   1,   0,   1,   1,   1,   0,   0,   1,   0,   0},
//              LED[][x] =  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24
                          {170, 170,   0, 170, 213,  71,  71,   0,  85,  85,  85,  85,  85,   0,  85,  85,  85,  84,  84,   0,   0,  84,   0,  85},
                          {255, 255, 158, 255, 255, 255, 255, 158, 204, 204, 204, 255, 204, 255, 204, 255, 204, 204, 204, 255, 255, 204, 255, 255},
                          {  8,   3,   8,   8,   3,  10,  10,   8,   5,   5,   5,   3,   5,   8,   5,   3,   5,   5,   5,   5,   5,   5,   5,   8},

//              LED[][x] =   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23 24
                          {  0,   0,   0,   0,   8,   7,   3,   8,   8,   8,   8,   2,   8,   0,   8,   8,   0,   8,   0,   2,   0,   0,   0,   8},
                          {  0,   0,   3,   3,   0,   7,   3,   0,   2,   1,   2,   2,   1,   8,   1,   0,   8,   1,   8,   2,   3,   8,   3,   0},
                          {  8,   8,   3,   3,   0,   0,   0,   3,   8,   0,   8,   5,   0,   0,   0,   3,   0,   0,   0,   5,   3,   0,   3,   0},
//              LED[][x] =  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24
                          {170, 170, 128, 128,   0,  43,  43, 239, 213,   6, 213, 170,   6,  85,   6, 239,  85,   6,  85, 170, 128,  85, 128,   0},
                          {255, 255, 255, 255, 255, 255, 255, 255, 191, 255, 191, 153, 255, 255, 255, 255, 255, 255, 255, 153, 255, 255, 255, 255},
                          {  8,   8,   3,   3,   8,   8,   3,   8,   8,   8,   8,   5,   8,   8,   8,   8,   8,   8,   8,   5,   3,   8,   3,   8},
    
//              LED[][x] =   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23 24
                          {  0,   0,   0,   9,   8,   0,   0,   0,   0,   0,   0,   0,   8,   0,   8,   8,   0,   0,   0,   0,   0,   0,   0,   0},
                          {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   8,   0,   0,   0,   0,   0,   0,   3,   8,   3,   0},
                          {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,   0,   0,   0,   0,   3,   0,   3,   8},
//              LED[][x] =  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24
                          {213, 213, 213,   0,   0, 213, 213, 213, 213, 213, 213, 213,   6,  85, 239, 239, 213, 213, 213, 213, 128,  85, 128, 170},
                          {  0,   0,   0, 255, 255,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255},
                          {  0,   0,   0,  10,   8,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   0,   0,   0,   0,   3,   8,   3,   8},

//              LED[][x] =   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23 24
                          {  0,   0,   0,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   0,   0,   0,   0,   0,   0,   0,   0},
                          {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
                          {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,   0,   0,   0,   0,   9,   0,   0,   0}
//              LED[][x] =  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24
                          {213, 213, 213,   0, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 239, 239, 213, 213, 213, 213, 170, 213, 213, 213},
                          {  0,   0,   0, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255,   0,   0,   0,   0, 255,   0,   0,   0},
                          {  0,   0,   0,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   0,   0,   0,   0,  10,   0,   0,   0}
};

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
        │ RGB ++ │ RGB    │RGB STEP│ SELECT │
        │  MODES │ TOGGLE │ BRIGHT │RGB LYRS│
        ├───────-┼───────-┼───────-┼───────-┤
        │NUMLOCK │RGB SEL │RGB SEL │RGB SEL │
        │  LAYER │  LYR 1 │  LYR 2 │  LYR 3 │
        ├───────-┼───────-┼───────-┼───────-┤
        │        |RGB SEL │RGB SEL │RGB SEL │
        │        |  LYR 4 │  LYR 5 │  LYR 6 │
        ├───────-┼───────-┼───────-┼───────-┤
        │        │ANIM BCK│ANIM BCK│ANIM BCK│
        │        │STEP HUE│STEP SAT│STEP VAL│
        ├───────-┼───────-┼───────-┼───────-┤
        │INCREASE|INCREASE│INCREASE│INCREASE│
        │  SPEED |  HUE   │  SAT.  │ BRIGHT │
        ├───────-┼───────-┼───────-┼───────-┤
        |RGB SEL │        │HSV INC-│  HSV   │
        |  LAYER │        │ REMENT │   STEP │
        └───────-┴───────-┴───────-┴───────-┘
*/


    [_RGB_LYR] = LAYOUT(
        XXXXXXX, RGB_MOD, RGB_TOG, RGB_STEP,
        L0,      XXXXXXX, RGB_SEL, LYR_SFT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, HSV_ABH, HSV_ABS, HSV_ABV,
        XXXXXXX, RGB_HUI, RGB_SAI, RGB_VAI,
        L4,      RGB_SPI, HSV_INC, HSV_STEP
    ),


/*   _RGB_SEL_LYR :
        ┌───────-┬───────-┬───────-┬───────-┐
        │ NUMPAD │ NUMPAD │ NUMPAD │ NUMPAD │
        │  KEY 1 │  KEY 2 │  KEY 3 │  KEY 4 │
        ├───────-┼───────-┼───────-┼───────-┤
        │ NUMPAD │ NUMPAD │ NUMPAD │ NUMPAD │
        │  KEY 5 │  KEY 6 │  KEY 7 │  KEY 8 │
        ├───────-┼───────-┼───────-┼───────-┤
        │ NUMPAD │ NUMPAD │ NUMPAD │ NUMPAD │
        │  KEY 9 │  KEY 10│  KEY 11│  KEY 12│
        ├───────-┼───────-┼───────-┼───────-┤
        │ NUMPAD │ NUMPAD │ NUMPAD │ NUMPAD │
        │  KEY 13│  KEY 14│  KEY 15│  KEY 16│
        ├───────-┼───────-┼───────-┼───────-┤
        │ NUMPAD │ NUMPAD │ NUMPAD │ NUMPAD │
        │  KEY 17│  KEY 18│  KEY 19│  KEY 20│
        ├───────-┼───────-┼───────-┼───────-┤
        | NUMPAD │ NUMPAD │ NUMPAD │ NUMPAD │
        |  KEY 21│  KEY 22│  KEY 23│  KEY 24│
        └───────-┴───────-┴───────-┴───────-┘
*/


    [_RGB_SEL_LYR] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
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
    num_val = led_state.num_lock ? 1 : 0;
    return true;
};


const uint16_t PROGMEM rgb_combo[]  = {KC_RGUI, KC_A, COMBO_END};
combo_t                key_combos[] = {COMBO(rgb_combo, RGB_SEL)};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case RGUI_T(KC_NO):
            if (record->tap.count && record->event.pressed) {
                switch (state->count) {
                    case 1:
                        rgb_shift_val = rgb_shift_val < 24 ? rgb_shift_val + 4 : 0;
                        break;
                    case 2:
                        rgb_lyr_val = rgb_lyr_val < 4 ? rgb_lyr_val + 1 : 0;
                        break;
                return false;        // Return false to ignore further processing of key
            } else if (!record->tap.count && record->event.pressed) {

                return false;
            }
            if (record->event.pressed) {
                rgb_shift_val = rgb_shift_val < 24 ? rgb_shift_val + 4 : 0;
            }
            break;

            
        case LT(0, KC_X):
            if (!record->tap.count && record->event.pressed) {
                tap_code16(C(KC_X)); // Intercept hold function to send Ctrl-X
                return false;
            }

            
        case LCTL_T(KC_DQUO):
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_DQUO); // Send KC_DQUO on tap
                return false;        // Return false to ignore further processing of key
            }

            
        case LT(0, KC_NO):
            if (record->tap.count && record->event.pressed) {
                tap_code16(C(KC_C)); // Intercept tap function to send Ctrl-C
            } else if (record->event.pressed) {
                tap_code16(C(KC_V)); // Intercept hold function to send Ctrl-V
            }
            return false;


        case RGB_SEL:
            if (record->event.pressed) {
                if (select_active == 0) {
                    pck_lyr       = 0;
                    select_active = 1;
                } else {
                    select_active = 0;
                    return L3;
                }
            }
            return false;

            
        case LYR_SFT:
            if (record->event.pressed && select_active == 1) {
                if (pck_lyr == max_lyrs) {
                    pck_lyr = 0;
                } else {
                    pck_lyr = pck_lyr + 1;
                }
            }
            return false;


        case pick key:
            if (record->event.pressed) {
                if (record->event.key.col == 1 && record->event.key.row == 2) {
                    
                } else {
                    
                } else if (record->event.key.col == 2 && record->event.key.row == 2) {

                } else {

                } else if (record->event.key.col == 4 && record->event.key.row == 3) {
                    
                } else {
                    
                } else if (record->event.key.col == 4 && record->event.key.row == 2) {
                    
                } else {
                    
                } else if (record->event.key.col == 0 && record->event.key.row == 0) {
                    
                } else {
                    
                } else if (record->event.key.col == 4 && record->event.key.row == 0) {
                    
                } else {
                    
                } else if (record->event.key.col == 1 && record->event.key.row == 1) {
                    
                } else {
                    
                } else if (record->event.key.col == 6 && record->event.key.row == 1) {
                    
                } else {
                    
                } else if (record->event.key.col == 1 && record->event.key.row == 0) {
                    
                } else {
                    
                } else if (record->event.key.col == 5 && record->event.key.row == 0) {
                    
                } else {
                    
                } else if (record->event.key.col == 2 && record->event.key.row == 1) {
                    
                } else {
                    
                } else if (record->event.key.col == 5 && record->event.key.row == 2) {

                } else {

                } else if (record->event.key.col == 2 && record->event.key.row == 0) {
                    
                } else {
                    
                } else if (record->event.key.col == 6 && record->event.key.row == 0) {
                    
                } else {
                    
                } else if (record->event.key.col == 3 && record->event.key.row == 1) {
                    
                } else {
                    
                } else if (record->event.key.col == 7 && record->event.key.row == 1) {
                    
                } else {
                    
                } else if (record->event.key.col == 3 && record->event.key.row == 0) {
                    
                } else {
                    
                } else if (record->event.key.col == 7 && record->event.key.row == 0) {
                    
                } else {
                    
                } else if (record->event.key.col == 4 && record->event.key.row == 1) {
                    
                } else {
                    
                } else if (record->event.key.col == 6 && record->event.key.row == 2) {
                    
                } else {
                    
                } else if (record->event.key.col == 0 && record->event.key.row == 1) {
                    
                } else {
                    
                } else if (record->event.key.col == 7 && record->event.key.row == 2) {
                    
                } else {
                    
                } else if (record->event.key.col == 5 && record->event.key.row == 1) {
                    
                } else {
                    
                } else if (record->event.key.col == 0 && record->event.key.row == 2) {
                    
                } else {
                    
                }
                return false;
            }

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
    if (IS_LAYER_ON(_RGB_SEL_LYR) && rgb_switch == 1) {
        int a = pck_lyr * 3;
        for (int b = 0; b <= 23; b++) {
            int LEDh = LED[(0 + a)][b];
            int LEDs = LED[(1 + a)][b];
            int LEDv = LED[(2 + a)][b];

            RGB_MATRIX_INDICATOR_SET_COLOR(b, LEDr * brite_adj / 255, LEDg * brite_adj / 255, LEDb * brite_adj / 255);
        }
    }

    if (rgb_switch == 1) {
        int a = IS_LAYER_ON(_NUM_LOCK) ? (num_val < 1 ? 0 : 3) : (IS_LAYER_ON(_MOUSE_KEYS) ? 6 : (IS_LAYER_ON(_RGB_LYR) ? 9 : (IS_LAYER_ON(_BOOT_LYR) ? 12 : false)));
        for (int b = 0; b <= 23; b++) {
            int LEDr = LED[(0 + a)][b];
            int LEDg = LED[(1 + a)][b];
            int LEDb = LED[(2 + a)][b];
            RGB_MATRIX_INDICATOR_SET_COLOR(b, LEDr * brite_adj / 255, LEDg * brite_adj / 255, LEDb * brite_adj / 255);
        }
    }

    if (rgb_switch == 1) {
        int a = IS_LAYER_ON(_NUM_LOCK) ? (num_val < 1 ? 0 : 3) : (IS_LAYER_ON(_MOUSE_KEYS) ? 6 : (IS_LAYER_ON(_RGB_LYR) ? 9 : (IS_LAYER_ON(_BOOT_LYR) ? 12 : false)));
        for (int b = 0; b <= 23; b++) {
            int LEDr = LED[(0 + a)][b];
            int LEDg = LED[(1 + a)][b];
            int LEDb = LED[(2 + a)][b];
            RGB_MATRIX_INDICATOR_SET_COLOR(b, LEDr * brite_adj / 255, LEDg * brite_adj / 255, LEDb * brite_adj / 255);
        }
    }

    if (IS_LAYER_ON(_RGB_SEL_LYR) && rgb_switch == 1) {
        //      a  =  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34
        int LEDr[] = {36,  128, 191, 255, 255, 255, 255, 255, 191, 128, 36,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   36,  255, 255, 255, 191, 128, 36,  0,   255, 255, 255, 255 };
        int LEDg[] = {0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 255, 191, 128, 36,  0,   0,   0,   0,   36,  128, 191, 255, 255, 128, 36,  0,   0,   0,   0,   0,   0,   0,   0,   255 };
        int LEDb[] = {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   36,  128, 191, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 191, 128, 36,  255 };
        int a;
        int b = 0;
        for (a = 0; a <= 23; a++) {
            b = a > 19 ? 34 : (a + rgb_shift_val < 34 ? a + rgb_shift_val : (a + rgb_shift_val > 34 ? a + rgb_shift_val - 33 : 0));
            RGB_MATRIX_INDICATOR_SET_COLOR(a, LEDr[b] * brite_adj / 255, LEDg[b] * brite_adj / 255, LEDb[b] * brite_adj / 255);
        }
    }

    return false;
};

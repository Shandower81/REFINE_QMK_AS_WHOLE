/* Copyright 2017 Jason Williams
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

// clang-format off

/*
 * RGB Colors
 *                                HEX
 *                          RED, GREEN, BLUE
 */
#define RGB_BLACK          0x00, 0x00, 0x00
#define RGB_WHITE          0xFF, 0xFF, 0xFF
#define RGB_RED            0xFF, 0x00, 0x00
#define RGB_BLACK_BEAN     0x3C, 0x00, 0x00
#define RGB_LIGHT_RED      0xFF, 0x80, 0x80
#define RGB_SCARLET        0xFF, 0x24, 0x00
#define RGB_CORAL          0xFF, 0x7C, 0x4D
#define RGB_INFERNO_ORANGE 0xFF, 0x48, 0x00
#define RGB_ORANGE         0xFF, 0x80, 0x00
#define RGB_GOLDENROD      0xD9, 0xA5, 0x21
#define RGB_GOLD           0xFF, 0xD9, 0x00
#define RGB_YELLOW         0xFF, 0xFF, 0x00
#define RGB_DARK_OLIVE     0x40, 0x40, 0x00
#define RGB_CHARTREUSE     0x80, 0xFF, 0x00
#define RGB_NUCLEAR_GREEN  0x48, 0xFF, 0x00
#define RGB_INDIA_GREEN    0x24, 0x80, 0x00
#define RGB_GREEN          0x00, 0xFF, 0x00
#define RGB_PCB_GREEN      0x00, 0x40, 0x00
#define RGB_SPRINGGREEN    0x00, 0xFF, 0x80
#define RGB_TURQUOISE      0x47, 0x6E, 0x6A
#define RGB_CYAN           0x00, 0xFF, 0xFF
#define RGB_TEAL           0x00, 0x80, 0x80
#define RGB_FOREST         0x00, 0x40, 0x40
#define RGB_AZURE          0x99, 0xF5, 0xFF
#define RGB_BLUE           0x00, 0x00, 0xFF
#define RGB_OVERDUE_BLUE   0x3C, 0x00, 0xFF
#define RGB_INDIGO         0x1E, 0x00, 0x80
#define RGB_PURPLE         0x7A, 0x00, 0xFF
#define RGB_VIOLET         0x80, 0x00, 0xFF
#define RGB_MAGENTA        0xFF, 0x00, 0xFF
#define RGB_EGGPLANT       0x40, 0x00, 0x40
#define RGB_SOOTY          0x14, 0x14, 0x14
#define RGB_PINK           0xFF, 0x80, 0xBF
#define RGB_RASPBERRY      0xFF, 0x00, 0x48
#define RGB_VIENNA_ROAST   0x40, 0x00, 0x12
#define RGB_OFF         RGB_BLACK

/*
 * HSV Colors
 *
 * All values (including hue) are scaled to 0-255
 *                            DECIMAL
 *                         HUE, SAT, VAL
 */  
#define HSV_BLACK            0,   0,   0
#define HSV_WHITE            0,   0, 255
#define HSV_RED              0, 255, 255
#define HSV_BLACK_BEAN       0, 255,  61
#define HSV_LIGHT_RED        0, 128, 255
#define HSV_SCARLET          6, 255, 255
#define HSV_CORAL           11, 176, 255
#define HSV_INFERNO_ORANGE  12, 255, 255
#define HSV_ORANGE          21, 255, 255
#define HSV_GOLDENROD       30, 218, 218
#define HSV_GOLD            36, 255, 255
#define HSV_YELLOW          43, 255, 255
#define HSV_DARK_OLIVE      43, 255,  64
#define HSV_CHARTREUSE      64, 255, 255
#define HSV_NUCLEAR_GREEN   73, 255, 255
#define HSV_INDIA_GREEN     73, 255, 128
#define HSV_GREEN           85, 255, 255
#define HSV_PCB_GREEN       85, 255,  64
#define HSV_SPRINGGREEN    106, 255, 255
#define HSV_TURQUOISE      123,  90, 112
#define HSV_CYAN           128, 255, 255
#define HSV_TEAL           128, 255, 128
#define HSV_FOREST         128, 255,  64
#define HSV_AZURE          132, 102, 255
#define HSV_BLUE           170, 255, 255
#define HSV_OVERDUE_BLUE   180, 255, 255
#define HSV_INDIGO         180, 255, 128
#define HSV_PURPLE         191, 255, 255
#define HSV_VIOLET         191, 255, 255
#define HSV_MAGENTA        213, 255, 255
#define HSV_EGGPLANT       213, 255,  64
#define HSV_SOOTY          213,   0,  20
#define HSV_PINK           234, 128, 255
#define HSV_RASPBERRY      243, 255, 255
#define HSV_VIENNA_ROAST   243, 255,  64
#define HSV_OFF            HSV_BLACK


// RGB Colors  -  assigned to short name
//
#define RGB_01 RGB_BLACK          
#define RGB_02 RGB_WHITE          
#define RGB_03 RGB_RED            
#define RGB_04 RGB_BLACK_BEAN     
#define RGB_05 RGB_LIGHT_RED      
#define RGB_06 RGB_SCARLET        
#define RGB_07 RGB_CORAL          
#define RGB_08 RGB_INFERNO_ORANGE 
#define RGB_09 RGB_ORANGE         
#define RGB_10 RGB_GOLDENROD      
#define RGB_11 RGB_GOLD           
#define RGB_12 RGB_YELLOW         
#define RGB_13 RGB_DARK_OLIVE     
#define RGB_14 RGB_CHARTREUSE     
#define RGB_15 RGB_NUCLEAR_GREEN  
#define RGB_16 RGB_INDIA_GREEN    
#define RGB_17 RGB_GREEN          
#define RGB_18 RGB_PCB_GREEN      
#define RGB_19 RGB_SPRINGGREEN    
#define RGB_20 RGB_TURQUOISE      
#define RGB_21 RGB_CYAN           
#define RGB_22 RGB_TEAL           
#define RGB_23 RGB_FOREST         
#define RGB_24 RGB_AZURE          
#define RGB_25 RGB_BLUE           
#define RGB_26 RGB_OVERDUE_BLUE   
#define RGB_27 RGB_INDIGO         
#define RGB_28 RGB_PURPLE         
#define RGB_29 RGB_VIOLET         
#define RGB_30 RGB_MAGENTA        
#define RGB_31 RGB_EGGPLANT       
#define RGB_32 RGB_SOOTY          
#define RGB_33 RGB_PINK           
#define RGB_34 RGB_RASPBERRY      
#define RGB_35 RGB_VIENNA_ROAST   


// HSV Colors  -  assigned to short name
//
#define HSV_01 HSV_BLACK          
#define HSV_02 HSV_WHITE          
#define HSV_03 HSV_RED            
#define HSV_04 HSV_BLACK_BEAN     
#define HSV_05 HSV_LIGHT_RED      
#define HSV_06 HSV_SCARLET        
#define HSV_07 HSV_CORAL          
#define HSV_08 HSV_INFERNO_ORANGE 
#define HSV_09 HSV_ORANGE         
#define HSV_10 HSV_GOLDENROD      
#define HSV_11 HSV_GOLD           
#define HSV_12 HSV_YELLOW         
#define HSV_13 HSV_DARK_OLIVE     
#define HSV_14 HSV_CHARTREUSE     
#define HSV_15 HSV_NUCLEAR_GREEN  
#define HSV_16 HSV_INDIA_GREEN    
#define HSV_17 HSV_GREEN          
#define HSV_18 HSV_PCB_GREEN      
#define HSV_19 HSV_SPRINGGREEN    
#define HSV_20 HSV_TURQUOISE      
#define HSV_21 HSV_CYAN           
#define HSV_22 HSV_TEAL           
#define HSV_23 HSV_FOREST         
#define HSV_24 HSV_AZURE          
#define HSV_25 HSV_BLUE           
#define HSV_26 HSV_OVERDUE_BLUE   
#define HSV_27 HSV_INDIGO         
#define HSV_28 HSV_PURPLE         
#define HSV_29 HSV_VIOLET         
#define HSV_30 HSV_MAGENTA        
#define HSV_31 HSV_EGGPLANT       
#define HSV_32 HSV_SOOTY          
#define HSV_33 HSV_PINK           
#define HSV_34 HSV_RASPBERRY      
#define HSV_35 HSV_VIENNA_ROAST   



/* RGB Colors  -  assigned to short name
Short       Color                  HEX                DECIMAL
Name        Name             RED, GREEN, BLUE     RED, GREEN, BLUE
 *
RGB_01   RGB_BLACK          0x00, 0x00, 0x00     {  0,   0,   0}
RGB_02   RGB_WHITE          0xFF, 0xFF, 0xFF     {255, 255, 255}
RGB_03   RGB_RED            0xFF, 0x00, 0x00     {255,   0,   0}
RGB_04   RGB_BLACK_BEAN     0x3C, 0x00, 0x00     { 60,   0,   0}
RGB_05   RGB_LIGHT_RED      0xFF, 0x80, 0x80     {255, 128, 128}
RGB_06   RGB_SCARLET        0xFF, 0x24, 0x00     {255,  36,   0}
RGB_07   RGB_CORAL          0xFF, 0x7C, 0x4D     {255, 124,  77}
RGB_08   RGB_INFERNO_ORANGE 0xFF, 0x48, 0x00     {255,  72,   0}
RGB_09   RGB_ORANGE         0xFF, 0x80, 0x00     {255, 128,   0}
RGB_10   RGB_GOLDENROD      0xD9, 0xA5, 0x21     {217, 165,  33}
RGB_11   RGB_GOLD           0xFF, 0xD9, 0x00     {255, 217,   0}
RGB_12   RGB_YELLOW         0xFF, 0xFF, 0x00     {255, 255,   0}
RGB_13   RGB_DARK_OLIVE     0x40, 0x40, 0x00     { 64,  64,   0}
RGB_14   RGB_CHARTREUSE     0x80, 0xFF, 0x00     {128, 255,   0}
RGB_15   RGB_NUCLEAR_GREEN  0x48, 0xFF, 0x00     { 72, 255,   0}
RGB_16   RGB_INDIA_GREEN    0x24, 0x80, 0x00     { 36, 128,   0}
RGB_17   RGB_GREEN          0x00, 0xFF, 0x00     {  0, 255,   0}
RGB_18   RGB_PCB_GREEN      0x00, 0x40, 0x00     {  0,  64,   0}
RGB_19   RGB_SPRINGGREEN    0x00, 0xFF, 0x80     {  0, 255, 128}
RGB_20   RGB_TURQUOISE      0x47, 0x6E, 0x6A     { 71, 110, 106}
RGB_21   RGB_CYAN           0x00, 0xFF, 0xFF     {  0, 255, 255}
RGB_22   RGB_TEAL           0x00, 0x80, 0x80     {  0, 128, 128}
RGB_23   RGB_FOREST         0x00, 0x40, 0x40     {  0,  64,  64}
RGB_24   RGB_AZURE          0x99, 0xF5, 0xFF     {153, 245, 255}
RGB_25   RGB_BLUE           0x00, 0x00, 0xFF     {  0,   0, 255}
RGB_26   RGB_OVERDUE_BLUE   0x3C, 0x00, 0xFF     { 60,   0, 255}
RGB_27   RGB_INDIGO         0x1E, 0x00, 0x80     { 30,   0, 128}
RGB_28   RGB_PURPLE         0x7A, 0x00, 0xFF     {122,   0, 255}
RGB_29   RGB_VIOLET         0x80, 0x00, 0xFF     {128,   0, 255}
RGB_30   RGB_MAGENTA        0xFF, 0x00, 0xFF     {255,   0, 255}
RGB_31   RGB_EGGPLANT       0x40, 0x00, 0x40     { 64,   0,  64}
RGB_32   RGB_SOOTY          0x14, 0x14, 0x14     { 20,  20,  20}
RGB_33   RGB_PINK           0xFF, 0x80, 0xBF     {255, 128, 191}
RGB_34   RGB_RASPBERRY      0xFF, 0x00, 0x48     {255,   0,  72}
RGB_35   RGB_VIENNA_ROAST   0x40, 0x00, 0x12     { 64,   0,  18}

*/


/* HSV Colors  -  assigned to short name
Short      Color              HSV ADJUSTED    HSV UNADJUSTED         RGB
Name       Name              HUE, SAT, VAL     HUE, SAT, VAL   RED, GREEN, BLUE

HSV_01   HSV_BLACK             0,   0,   0    {  0,   0,   0} {  0,   0,   0}
HSV_02   HSV_WHITE             0,   0, 255    {  0,   0, 100} {255, 255, 255}
HSV_03   HSV_RED               0, 255, 255    {  0, 100, 100} {255,   0,   0}
HSV_04   HSV_BLACK_BEAN        0, 255,  61    {  0, 100,  24} { 60,   0,   0}
HSV_05   HSV_LIGHT_RED         0, 128, 255    {  0,  50, 100} {255, 128, 128}
HSV_06   HSV_SCARLET           6, 255, 255    {  8, 100, 100} {255,  36,   0}
HSV_07   HSV_CORAL            11, 176, 255    { 16,  75, 100} {255, 124,  77}
HSV_08   HSV_INFERNO_ORANGE   12, 255, 255    { 17, 100, 100} {255,  72,   0}
HSV_09   HSV_ORANGE           21, 255, 255    { 30, 100, 100} {255, 128,   0}
HSV_10   HSV_GOLDENROD        30, 218, 218    { 43,  85,  85} {217, 165,  33}
HSV_11   HSV_GOLD             36, 255, 255    { 51, 100, 100} {255, 217,   0}
HSV_12   HSV_YELLOW           43, 255, 255    { 60, 100, 100} {255, 255,   0}
HSV_13   HSV_DARK_OLIVE       43, 255,  64    { 60, 100,  25} { 64,  64,   0}
HSV_14   HSV_CHARTREUSE       64, 255, 255    { 90, 100, 100} {128, 255,   0}
HSV_15   HSV_NUCLEAR_GREEN    73, 255, 255    {103, 100, 100} { 72, 255,   0}
HSV_16   HSV_INDIA_GREEN      73, 255, 128    {103, 100,  50} { 36, 128,   0}
HSV_17   HSV_GREEN            85, 255, 255    {120, 100, 100} {  0, 255,   0}
HSV_18   HSV_PCB_GREEN        85, 255,  64    {120, 100,  25} {  0,  64,   0}
HSV_19   HSV_SPRINGGREEN     106, 255, 255    {150, 100, 100} {  0, 255, 128}
HSV_20   HSV_TURQUOISE       123,  90, 112    {174,  35,  43} { 71, 110, 106}
HSV_21   HSV_CYAN            128, 255, 255    {180, 100, 100} {  0, 255, 255}
HSV_22   HSV_TEAL            128, 255, 128    {180, 100,  50} {  0, 128, 128}
HSV_23   HSV_FOREST          128, 255,  64    {180, 100,  25} {  0,  64,  64}
HSV_24   HSV_AZURE           132, 102, 255    {186,  40, 100} {153, 245, 255}
HSV_25   HSV_BLUE            170, 255, 255    {240, 100, 100} {  0,   0, 255}
HSV_26   HSV_OVERDUE_BLUE    180, 255, 255    {254, 100, 100} { 60,   0, 255}
HSV_27   HSV_INDIGO          180, 255, 128    {254, 100,  50} { 30,   0, 128}
HSV_28   HSV_PURPLE          190, 255, 255    {269, 100, 100} {122,   0, 255}
HSV_29   HSV_VIOLET          191, 255, 255    {270, 100, 100} {128,   0, 255}
HSV_30   HSV_MAGENTA         213, 255, 255    {300, 100, 100} {255,   0, 255}
HSV_31   HSV_EGGPLANT        213, 255,  64    {300, 100,  25} { 64,   0,  64}
HSV_32   HSV_SOOTY           213,   0,  20    {300,   0,   8} { 20,  20,  20}
HSV_33   HSV_PINK            234, 128, 255    {330,  50, 100} {255, 128, 191}
HSV_34   HSV_RASPBERRY       243, 255, 255    {343, 100, 100} {255,   0,  72}
HSV_35   HSV_VIENNA_ROAST    243, 255,  64    {343, 100,  25} { 64,   0,  18}
*/

// clang-format on

#if defined(__GNUC__)
#    define PACKED __attribute__((__packed__))
#else
#    define PACKED
#endif

#if defined(_MSC_VER)
#    pragma pack(push, 1)
#endif

#ifdef RGBW
#    define LED_TYPE cRGBW
#else
#    define LED_TYPE RGB
#endif

#define WS2812_BYTE_ORDER_RGB 0
#define WS2812_BYTE_ORDER_GRB 1
#define WS2812_BYTE_ORDER_BGR 2

#ifndef WS2812_BYTE_ORDER
#    define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_GRB
#endif

typedef struct PACKED {
#if (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_GRB)
    uint8_t g;
    uint8_t r;
    uint8_t b;
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_RGB)
    uint8_t r;
    uint8_t g;
    uint8_t b;
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_BGR)
    uint8_t b;
    uint8_t g;
    uint8_t r;
#endif
} cRGB;

typedef cRGB RGB;

// WS2812 specific layout
typedef struct PACKED {
#if (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_GRB)
    uint8_t g;
    uint8_t r;
    uint8_t b;
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_RGB)
    uint8_t r;
    uint8_t g;
    uint8_t b;
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_BGR)
    uint8_t b;
    uint8_t g;
    uint8_t r;
#endif
    uint8_t w;
} cRGBW;

typedef struct PACKED {
    uint8_t h;
    uint8_t s;
    uint8_t v;
} HSV;

#if defined(_MSC_VER)
#    pragma pack(pop)
#endif

RGB hsv_to_rgb(HSV hsv);
RGB hsv_to_rgb_nocie(HSV hsv);
#ifdef RGBW
void convert_rgb_to_rgbw(LED_TYPE *led);
#endif

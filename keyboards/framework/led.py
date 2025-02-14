#!/usr/bin/env python3
"""
Generate C code for the common configuration of the RBG matrix feature of QMK

Reference: https://docs.qmk.fm/#/feature_rgb_matrix?id=common-configuration

To use adjust `ansi` variable, run it and use the printed output.

All LEDs are set as type LED_FLAG_KEYLIGHT. That's good enough for us.
All our LEDs are behind keys. However, LED_FLAG_MODIFIER might be useful for
some keys. Not sure what that means, though.
"""

from collections import OrderedDict

# Rows and columns in the electrical keyboard matrix.
# Equivalent to QMK's macros: MATRIX_ROWS and MATRIX_COLS
# (Un)

# Keyboard
#MATRIX_ROWS = 8
#MATRIX_COLS = 16
# Macropad
MATRIX_ROWS = 4
MATRIX_COLS = 8

LED_FLAG_NONE      = 0x00
LED_FLAG_MODIFIER  = 0x01
LED_FLAG_UNDERGLOW = 0x02
LED_FLAG_KEYLIGHT  = 0x04
LED_FLAG_INDICATOR = 0x08

# This structure is (manually) taken from a picture of the LED positions.
# The coordinates don't need to be translated or scaled. This script takes care of that.
# I loaded the PDF into GIMP and just took the coordinates.
# id:     LED id, of where it's connected to the LED controller
# x:      x coordinate of the LED
# y:      y coordinate of the LED
# matrix: The keyboard matrix coordinate of the key this LED belongs to.
#         Only one LED per key needs this. Other's can keep it unset.
ansi = [
    { "id":  "1", "x":0,   "y": 64, "matrix": (0,0)  },  # ESC FN_Lock
    { "id":  "2", "x":134,  "y": 160                   },  # ESC FN_Lock
    { "id":  "3", "x":200,  "y": 154, "matrix": (2,0)  },  # F1  Toggle Volume

    { "id":  "4", "x":275,  "y": 154, "matrix": (3,0)  },  # F2  Decrease Volume
    { "id":  "5", "x":350,  "y": 154, "matrix": (4,0)  },  # F3  Increase Volume
    { "id":  "6", "x":425,  "y": 154, "matrix": (5,0)  },  # F4  Media Rewind/Previous
    { "id":  "7", "x":500,  "y": 154, "matrix": (6,0)  },  # F5  Media Play/Pause
    { "id":  "8", "x":575,  "y": 154, "matrix": (7,0)  },  # F6  Media Fast Forward/Next
    { "id": "9",  "x":650,  "y": 154, "matrix": (8,0)  },  # F7  Decrease Screen Brightness
    { "id": "10", "x":725,  "y": 154, "matrix": (9,0)  },  # F8  Increase Screen Brightness
    { "id": "11", "x":800,  "y": 154, "matrix": (10,0) },  # F9  Change Screen
    { "id": "12", "x":875,  "y": 154, "matrix": (11,0) },  # F10 Airplane Mode

    { "id": "13", "x":935,  "y": 158, "matrix": (12,0) },  # F11 PrtScr
    { "id": "14", "x":960,  "y": 158                   },  # F11 PrtScr
    { "id": "15", "x":1022, "y": 154, "matrix": (14,0) },  # F12
    { "id": "16", "x":1088, "y": 160                   },  # Del
    { "id": "17", "x":1135, "y": 160, "matrix": (16,0) },  # Del

    # Second row
    { "id": "18", "x":97,   "y": 216, "matrix": (0,1)  },  # ` ~
    { "id": "19", "x":172,  "y": 216, "matrix": (2,1)  },  # 1 !
    { "id": "20", "x":247,  "y": 216, "matrix": (3,1)  },  # 2 @
    { "id": "21", "x":322,  "y": 216, "matrix": (4,1)  },  # 3 #
    { "id": "22", "x":397,  "y": 216, "matrix": (5,1)  },  # 4 $
    { "id": "23", "x":472,  "y": 216, "matrix": (6,1)  },  # 5 %
    { "id": "24", "x":547,  "y": 216, "matrix": (7,1)  },  # 6 ^
    { "id": "25", "x":622,  "y": 216, "matrix": (8,1)  },  # 7 &
    { "id": "26", "x":697,  "y": 216, "matrix": (9,1)  },  # 8 *
    { "id": "27", "x":772,  "y": 216, "matrix": (10,1) },  # 9 (
    { "id": "28", "x":847,  "y": 216, "matrix": (11,1) },  # 0 )
    { "id": "29", "x":922,  "y": 216, "matrix": (12,1) },  # - _
    { "id": "30", "x":997,  "y": 216, "matrix": (13,1) },  # = +

    { "id": "31", "x":1067, "y": 224                   },  # Backspace
    { "id": "32", "x":1128, "y": 224, "matrix": (16,1) },  # Backspace

    # Third row
    { "id": "33", "x":90,   "y": 298, "matrix": (0,2)  },  # Tab
    { "id": "34", "x":140,  "y": 298                   },  # Tab

    { "id": "35", "x":207,  "y": 290, "matrix": (2,2)  },  # q Q
    { "id": "36", "x":282,  "y": 290, "matrix": (3,2)  },  # w W
    { "id": "37", "x":357,  "y": 290, "matrix": (4,2)  },  # e E
    { "id": "38", "x":432,  "y": 290, "matrix": (5,2)  },  # r R
    { "id": "39", "x":507,  "y": 290, "matrix": (6,2)  },  # t T
    { "id": "40", "x":582,  "y": 290, "matrix": (7,2)  },  # y Y
    { "id": "41", "x":657,  "y": 290, "matrix": (8,2)  },  # u U
    { "id": "42", "x":732,  "y": 290, "matrix": (9,2)  },  # i I
    { "id": "43", "x":807,  "y": 290, "matrix": (10,2) },  # o O
    { "id": "44", "x":882,  "y": 290, "matrix": (11,2) },  # p P
    { "id": "45", "x":957,  "y": 290, "matrix": (12,2) },  # [ {
    { "id": "46", "x":1032, "y": 290, "matrix": (13,2) },  # ] }

    { "id": "47", "x":1115, "y": 292, "matrix": (16,2) },  # \ |

    # Fourth row
    { "id": "48", "x":94,   "y": 374, "matrix": (0,3)  },  # Caps Lock
    { "id": "49", "x":123,  "y": 367                   },  # Caps Lock
    { "id": "50", "x":150,  "y": 374                   },  # Caps Lock

    { "id": "51", "x":227,  "y": 365, "matrix": (3,3)  },  # a A
    { "id": "52", "x":302,  "y": 365, "matrix": (4,3)  },  # s S
    { "id": "53", "x":377,  "y": 365, "matrix": (5,3)  },  # d D
    { "id": "54", "x":452,  "y": 365, "matrix": (6,3)  },  # f F
    { "id": "55", "x":527,  "y": 365, "matrix": (7,3)  },  # g G
    { "id": "56", "x":602,  "y": 365, "matrix": (8,3)  },  # h H
    { "id": "57", "x":677,  "y": 365, "matrix": (9,3)  },  # j J
    { "id": "58", "x":752,  "y": 365, "matrix": (10,3) },  # k K
    { "id": "59", "x":827,  "y": 365, "matrix": (11,3) },  # l L
    { "id": "60", "x":902,  "y": 365, "matrix": (12,3) },  # ; :
    { "id": "61", "x":977,  "y": 365, "matrix": (13,3) },  # ' "

    { "id": "62", "x":1050, "y": 365                   },  # Enter
    { "id": "63", "x":1131, "y": 373, "matrix": (16,3) },  # Enter

    # Fifth row
    { "id": "64", "x":91,   "y": 448, "matrix": (0,4)  },  # LShift
    { "id": "65", "x":113,  "y": 448                   },  # LShift
    { "id": "66", "x":191,  "y": 440                   },  # LShift

    { "id": "67", "x":266,  "y": 440, "matrix": (3,4)  },  # z Z
    { "id": "68", "x":341,  "y": 440, "matrix": (4,4)  },  # x X
    { "id": "69", "x":416,  "y": 440, "matrix": (5,4)  },  # c C
    { "id": "70", "x":491,  "y": 440, "matrix": (6,4)  },  # v V
    { "id": "71", "x":566,  "y": 440, "matrix": (7,4)  },  # b B
    { "id": "72", "x":641,  "y": 440, "matrix": (8,4)  },  # n N
    { "id": "73", "x":716,  "y": 440, "matrix": (9,4)  },  # m M
    { "id": "74", "x":791,  "y": 440, "matrix": (10,4) },  # , <
    { "id": "75", "x":866,  "y": 440, "matrix": (11,4) },  # . >
    { "id": "76", "x":941,  "y": 440, "matrix": (12,4) },  # / ?

    { "id": "77", "x":1013, "y": 445                   },  # RShift
    { "id": "78", "x":1041, "y": 445                   },  # RShift
    { "id": "79", "x":1106, "y": 445                   },  # RShift
    { "id": "80", "x":1127, "y": 445, "matrix": (16,4) },  # RShift

    # Sixth row
    { "id": "81", "x":89,   "y": 522, "matrix": (0,5)  },  # CTRL
    { "id": "82", "x":124,  "y": 522 },                    # CTRL
    { "id": "83", "x":191,  "y": 515, "matrix": (2,5)  },  # FN
    { "id": "84", "x":266,  "y": 515, "matrix": (3,5)  },  # LGUI
    { "id": "85", "x":341,  "y": 515, "matrix": (4,5)  },  # LALT
    { "id": "86", "x":415,  "y": 522, "matrix": (5,5)  },  # Space
    { "id": "87", "x":475,  "y": 522                   },  # Space
    { "id": "88", "x":535,  "y": 522                   },  # Space
    { "id": "89", "x":595,  "y": 522                   },  # Space
    { "id": "90", "x":655,  "y": 522                   },  # Space
    { "id": "91", "x":715,  "y": 522                   },  # Space
    { "id": "92", "x":791,  "y": 515, "matrix": (11,5) },  # RALT
    { "id": "93", "x":865,  "y": 515, "matrix": (12,5) },  # RCTL
    { "id": "94", "x":945,  "y": 542, "matrix": (13,5) },  # LEFT
    { "id": "95", "x":1033, "y": 534, "matrix": (14,5) },  # DOWN
    { "id": "96", "x":1033, "y": 510, "matrix": (15,5) },  # Up    
    { "id": "97", "x":1120, "y": 542, "matrix": (16,5) },  # RIGHT
]

macropad = [
    { "id": "1",  "x":1332, "y": 158, "matrix": (0,0) },
    { "id": "2",  "x":1395, "y": 158, "matrix": (1,0) },
    { "id": "3",  "x":1460, "y": 158, "matrix": (2,0) },
    { "id": "4",  "x":1523, "y": 158, "matrix": (3,0) },

    { "id": "5",  "x":1332, "y": 219, "matrix": (0,1) },
    { "id": "6",  "x":1395, "y": 219, "matrix": (1,1) },
    { "id": "7",  "x":1460, "y": 219, "matrix": (2,1) },
    { "id": "8",  "x":1523, "y": 219, "matrix": (3,1) },

    { "id": "9",  "x":1332, "y": 294, "matrix": (0,2) },
    { "id": "10", "x":1395, "y": 294, "matrix": (1,2) },
    { "id": "11", "x":1460, "y": 294, "matrix": (2,2) },
    { "id": "12", "x":1523, "y": 294, "matrix": (3,2) },

    { "id": "13", "x":1332, "y": 368, "matrix": (0,3) },
    { "id": "14", "x":1395, "y": 368, "matrix": (1,3) },
    { "id": "15", "x":1460, "y": 368, "matrix": (2,3) },
    { "id": "16", "x":1523, "y": 368, "matrix": (3,3) },

    { "id": "17", "x":1332, "y": 443, "matrix": (0,4) },
    { "id": "18", "x":1395, "y": 443, "matrix": (1,4) },
    { "id": "19", "x":1460, "y": 443, "matrix": (2,4) },
    { "id": "20", "x":1523, "y": 443, "matrix": (3,4) },

    { "id": "21", "x":1332, "y": 519, "matrix": (0,5) },
    { "id": "22", "x":1395, "y": 519, "matrix": (1,5) },
    { "id": "23", "x":1460, "y": 519, "matrix": (2,5) },
    { "id": "24", "x":1523, "y": 519, "matrix": (3,5) },
]

# Recommended by QMK to be the (x,y) range of position values
LED_MAX = (224.0, 64.0)

NO_LED = 255

# Map LEDs to keyboard matrix and normalize LED coordinates
def normalize(layout):
    led_to_el = [[NO_LED for _ in range(MATRIX_COLS)] for _ in range(MATRIX_ROWS)]
    # Find smallest (offset) and largest (max) values
    offset_x = layout[0]['x']
    offset_y = layout[0]['y']
    max_x = layout[-1]['x']
    max_y = layout[-1]['y']
    for v in layout:
        if v['x'] < offset_x:
            offset_x = v['x']
        if v['y'] < offset_y:
            offset_y = v['y']
        if v['x'] > max_x:
            max_x = v['x']
        if v['y'] > max_y:
            max_y = v['y']
    # Ratio of coordinates in the input data vs QMK's reference rectangle
    x_ratio = LED_MAX[0] / (max_x - offset_x)
    y_ratio = LED_MAX[1] / (max_y - offset_y)

    # Normalize LED coordinates by scaling and translating
    normalized = OrderedDict()
    for v in sorted(layout, key=lambda item: int(item['id'])):
        x = x_ratio * (v['x'] - offset_x)
        y = y_ratio * (v['y'] - offset_y)

        normalized[int(v['id'])] = (x, y)

        if 'matrix' in v:
            if not v['matrix'] or v['matrix'] == ():
                continue
            (matrix_x,matrix_y) =  v['matrix']
            # Map LED IDs to keyboard matrix
            # Turn LED index into 0-indexed
            led_to_el[matrix_y][matrix_x] = int(v['id']) - 1
    return led_to_el, normalized

# Turn the data to C code that can be used in QMK's keymap.c
def print_matrix(layout, led_to_el, normalized):
    print("led_config_t g_led_config = { {")
    print("  // Key Matrix to LED Index")
    for row in led_to_el:
        print("  {", end='')
        for col in row:
            if col == 255:
                col = 'NO_LED'
            print(f"{col: >6}, ", end='')
        print("},")

    print("}, {")
    print("  // LED Index to Physical Position")
    for (i, (led_id, (x, y))) in enumerate(normalized.items()):
        coords = f"  {{ {int(x): >3}, {int(y): >3} }},"
        print(f"{coords: <15} // LED {led_id}")

    print("}, {")
    print("  // LED Index to Flag")
    print("  ", end='')
    for i in range(len(layout)):
        # TODO: Support other LED flags
        print(LED_FLAG_KEYLIGHT, end='')
        if i + 1 == len(layout):
            print()
            break
        else:
            print(", ", end='')
        if (i + 1) % 16 == 0:
            print("\n  ", end='')
    print("} };")

def main(data):
    # Normalize data and convert to C code
    led_to_el, normalized = normalize(data)
    print_matrix(data, led_to_el, normalized)

    # Draw led positions to visually check them with the reference design
    import matplotlib.pyplot as plt
    a = [[x, y] for _i, (x, y) in normalized.items()]
    scatter = plt.scatter(*zip(*a))
    ax = scatter.axes
    ax.invert_yaxis()
    plt.show()

if __name__ == "__main__":
    # Can choose which dataset to process
    main(ansi)

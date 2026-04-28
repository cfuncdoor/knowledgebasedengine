#include "core/types/types.h"

i32 color_as_hex(color_t color) {
    return color.hex;
}

color_t hex_as_color(i32 hex) {
    color_t myColor;
    myColor.hex = hex;
    return myColor;
}
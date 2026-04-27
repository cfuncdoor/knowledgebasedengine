#include "../types.h"
#include <stdbool.h>
#include <string.h>

b8 sv_equals(string_view a, string_view b) {
    if (a.len != b.len) return false;
    return memcmp(a.data, b.data, a.len) == 0;
}
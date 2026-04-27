#pragma once
#include "../types/types.h"

typedef enum {
    LOG_LEVEL_INFO  = 0,
    LOG_LEVEL_WARN  = 1,
    LOG_LEVEL_ERROR = 2,
    LOG_LEVEL_FATAL = 3
} log_level;

typedef struct {
    string_view timestamp;
} log_data_t;

typedef struct {
    b8 write_to_file;
    b8 has_timestamp;
} log_flags_t;

typedef struct {
    log_level level;
    log_flags_t flags;
} log_details_t;

typedef struct {
    log_details_t details;
    string_view msg;
} log_t;

void log_msg(log_level level, const char* fmt, ...);
void get_timestamp_string(char* buffer, u32 len);
void set_console_color(log_level level);

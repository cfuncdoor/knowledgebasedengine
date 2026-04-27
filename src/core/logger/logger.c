#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <windows.h>

void set_console_color(log_level level) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (level) {
        case LOG_LEVEL_INFO:  SetConsoleTextAttribute(hConsole, 10); break; 
        case LOG_LEVEL_WARN:  SetConsoleTextAttribute(hConsole, 14); break; 
        case LOG_LEVEL_ERROR: SetConsoleTextAttribute(hConsole, 12); break; 
        case LOG_LEVEL_FATAL: SetConsoleTextAttribute(hConsole, 79); break;
    }
}

void get_timestamp_string(char* buffer, u32 len) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, len, "%H:%M:%S", t);
}

void log_msg(log_level level, const char* fmt, ...) {
    static b8 is_logging = false;
    if (is_logging) {
        printf("[INTERNAL LOGGER ERROR]: prevented overflow! \n");
        return;
    }
    char message_buffer[1024];
    char time_buffer[32];

    va_list args;
    va_start(args, fmt);
    i32 result = vsnprintf(message_buffer, sizeof(message_buffer), fmt, args);
    va_end(args);

    if (result < 0) {
        is_logging = false;
        printf("internal logger error: vsnprintf failed.");
        return;
    } else if (result >= (i32)sizeof(message_buffer)) {
        is_logging = false;
        printf("message truncated! (required %d bytes, only have 1024)", result);
        is_logging = true;
    }

    log_t log_entry = {0};
    
    log_entry.details.level = level;
    log_entry.details.flags.has_timestamp = true;
    log_entry.details.flags.write_to_file = false;

    log_entry.msg.data = message_buffer;
    log_entry.msg.size = strlen(message_buffer);

    log_data_t data = {0};
    if (log_entry.details.flags.has_timestamp) {
        get_timestamp_string(time_buffer, sizeof(time_buffer));
        data.timestamp.data = time_buffer;
        data.timestamp.size = strlen(time_buffer);
    }

    set_console_color(log_entry.details.level);

    const char* level_strings[] = { "[info]", "[warn]", "[error]", "[fatal]" };
    
    printf("%s %s: %s\n", 
           (char*)data.timestamp.data, 
           level_strings[log_entry.details.level], 
           (char*)log_entry.msg.data);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    if (log_entry.details.level == LOG_LEVEL_FATAL) {
        OutputDebugStringA("fatal error, cannot recover. \n");
        __debugbreak();
        exit(1);
    }
}
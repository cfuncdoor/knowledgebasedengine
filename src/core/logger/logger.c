#include "core/logger/logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <windows.h>

void set_console_color(log_level level) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch ((i32)level) {
        case LOG_LEVEL_INFO:  SetConsoleTextAttribute(hConsole, 10); break; 
        case LOG_LEVEL_WARN:  SetConsoleTextAttribute(hConsole, 14); break; 
        case LOG_LEVEL_ERROR: SetConsoleTextAttribute(hConsole, 12); break; 
        case LOG_LEVEL_FATAL: SetConsoleTextAttribute(hConsole, 79); break;
        default:
            SetConsoleTextAttribute(hConsole, 7); 
            break;
    }
}

void get_timestamp_string(char* buffer, u32 len) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, len, "%H:%M:%S", t);
}

void log_msg(log_level level, const char* fmt, ...) {
    char message_buffer[1024];
    char time_buffer[32];
    i32 result;
    log_t log_entry;
    log_data_t data = {0};
    const char* level_str;
    va_list args;

    if (!fmt) return;

    {
        va_start(args, fmt);
        result = vsnprintf(message_buffer, sizeof(message_buffer), fmt, args);
        va_end(args);
    }

    {
        if (result < 0) {
            printf("internal logger error: vsnprintf failed.");
            return;
        } else if (result >= (i32)sizeof(message_buffer)) {
            printf("message truncated! (required %d bytes, only have 1024)", result);
        }
    }


    log_entry.details.level = level;
    log_entry.details.flags.has_timestamp = true;
    log_entry.details.flags.write_to_file = false;

    switch ((i32)level) {
        case LOG_LEVEL_INFO:  level_str = "[info]";  break;
        case LOG_LEVEL_WARN:  level_str = "[warn]";  break;
        case LOG_LEVEL_ERROR: level_str = "[error]"; break;
        case LOG_LEVEL_FATAL: level_str = "[fatal]"; break;
        default:              level_str = "[unknown]"; break;
    }

    log_entry.msg.data = message_buffer;
    log_entry.msg.len = strlen(message_buffer);

    if (log_entry.details.flags.has_timestamp) {
        get_timestamp_string(time_buffer, sizeof(time_buffer));
        data.timestamp.data = time_buffer;
        data.timestamp.len = strlen(time_buffer);
    }

    set_console_color(log_entry.details.level);
    
    printf("%s %s: %s\n", 
       data.timestamp.data, 
       level_str, 
       message_buffer);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    if (log_entry.details.level == LOG_LEVEL_FATAL) {
        OutputDebugStringA("fatal error, cannot recover. \n");
        __debugbreak();
        exit(1);
    }
}

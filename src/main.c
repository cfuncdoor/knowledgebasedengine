#include "core/application.h"
#include "core/logger/logger.h"

int main() {
    log_msg(LOG_LEVEL_INFO, "booting up...");
    application_t app;
    application_create(&app);

    while (!application_should_close(&app)) {
        application_run(&app);
    }

    log_msg(LOG_LEVEL_INFO, "goodbye!");
    return 0;
}
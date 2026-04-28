#include "core/application.h"
#include "core/logger/logger.h"

i32 main(void) {
    log_msg(LOG_LEVEL_INFO, "booting up...");
    application_t app = {0};
    application_create(&app);

    while (!application_should_close(&app)) {
        application_run(&app);
    }

    application_destroy(&app);
    log_msg(LOG_LEVEL_INFO, "leaving main()...");
    return 0;
}

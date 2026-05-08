/**
 * @file datapipe.c
 * @brief DataPipe 主模块实现
 */

#include "datapipe.h"
#include "platform.h"
#include <stdio.h>

const char* datapipe_version(void) {
    static char version[32];
    snprintf(version, sizeof(version), "%d.%d.%d",
             DATAPIPE_VERSION_MAJOR,
             DATAPIPE_VERSION_MINOR,
             DATAPIPE_VERSION_PATCH);
    return version;
}

const char* datapipe_platform(void) {
    return datapipe_get_platform_name();
}

const char* datapipe_build_info(void) {
    static char info[128];
    snprintf(info, sizeof(info),
             "DataPipe v%d.%d.%d | %s | Built: %s %s",
             DATAPIPE_VERSION_MAJOR,
             DATAPIPE_VERSION_MINOR,
             DATAPIPE_VERSION_PATCH,
             datapipe_get_platform_name(),
             __DATE__, __TIME__);
    return info;
}

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stddef.h>

#include "http.h"


typedef struct LOGGER LOGGER;
typedef struct LOGGER *LoggerPtr;
LoggerPtr logger__init(const char *log_file);
void logger__destroy(LoggerPtr this);
void logger__request(LoggerPtr this, const HttpRequestPtr request, size_t response_code);

static const struct Logger {
    LoggerPtr (*init)(const char* log_file);
    void (*destroy)(LoggerPtr this);
    void (*request)(LoggerPtr this, const HttpRequestPtr request, size_t response_code);
} Logger = {
    .init = &logger__init,
    .destroy = &logger__destroy,
    .request = &logger__request,
};


#endif

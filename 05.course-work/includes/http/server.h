#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <stddef.h>

#include "../options.h"


typedef struct HTTP_SERVER_SETTINGS *HttpServerSettingsPtr;
typedef struct HTTP_SERVER_SETTINGS {
    size_t max_connections;
    size_t queue_size;
    size_t buf_size;
    const char *port;
    const char *public_dir;
} HTTP_SERVER_SETTINGS;


typedef struct HTTP_SERVER HTTP_SERVER;
typedef struct HTTP_SERVER *HttpServerPtr;
HttpServerPtr server__init(const HttpServerSettingsPtr settings);
void server__destroy(HttpServerPtr this);
void server__start(HttpServerPtr this);
void server__accept_connections(HttpServerPtr this);

static const struct HttpServer {
    HttpServerPtr (*init)(const HttpServerSettingsPtr settings);
    void (*destroy)(HttpServerPtr this);
    void (*start)(HttpServerPtr this);
    void (*accept_connections)(HttpServerPtr this);
} HttpServer = {
    .init = &server__init,
    .destroy = &server__destroy,
    .start = &server__start,
    .accept_connections = &server__accept_connections,
};

#endif

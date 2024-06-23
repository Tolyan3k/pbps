#ifndef __HTTP__REQUEST_H__
#define __HTTP__REQUEST_H__

#include <stddef.h>

#include "utils.h"


typedef struct HTTP_REQUEST *HttpRequestPtr;
HttpRequestPtr request__init(const char *request_raw);
void request__destroy(HttpRequestPtr this);
const char* request__get_method(const HttpRequestPtr this);
const char* request__get_uri(const HttpRequestPtr this);
const char* request__get_protocol(const HttpRequestPtr this);
const char* request__get_header_value(const HttpRequestPtr this, const char *header);
const char* request__get_payload(const HttpRequestPtr this);
size_t request__get_payload_size(const HttpRequestPtr this);
const DictPtr request__get_headers(const HttpRequestPtr this);


static const struct {
    HttpRequestPtr (*init)(const char *request_raw);
    void (*destroy)(HttpRequestPtr this);
    const char *(*get_method)(const HttpRequestPtr this);
    const char *(*get_uri)(const HttpRequestPtr this);
    const char *(*get_protocol)(const HttpRequestPtr this);
    const char *(*get_header)(const HttpRequestPtr this, const char *header);
    DictPtr (*get_headers)(const HttpRequestPtr this);
    const char *(*get_payload)(const HttpRequestPtr this);
    size_t (*get_payload_size)(const HttpRequestPtr this);
} HttpRequest = {
    .init = &request__init,
    .destroy = &request__destroy,
    .get_method = &request__get_method,
    .get_uri = &request__get_uri,
    .get_protocol = &request__get_protocol,
    .get_header = &request__get_header_value,
    .get_headers = &request__get_headers,
    .get_payload = &request__get_payload,
    .get_payload_size = &request__get_payload_size,
};


#endif

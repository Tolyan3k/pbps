#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <stddef.h>


typedef struct HTTP_REQUEST HTTP_REQUEST;
typedef struct HTTP_REQUEST *HttpRequestPtr;
HTTP_REQUEST* request__init(const char *request_raw);
void request__destroy(HttpRequestPtr this);
const char* request__get_method(const HttpRequestPtr this);
const char* request__get_uri(const HttpRequestPtr this);
const char* request__get_protocol(const HttpRequestPtr this);
const char* request__get_header(const HttpRequestPtr this, const char *header);
const char** request__get_headers(const HttpRequestPtr this);
const char* request__get_payload(const HttpRequestPtr this);
size_t request__get_payload_size(const HttpRequestPtr this);


static const struct {
    HttpRequestPtr (*init)(const char *request_raw);
    void (*destroy)(HttpRequestPtr this);
    const char *(*get_method)(const HttpRequestPtr this);
    const char *(*get_uri)(const HttpRequestPtr this);
    const char *(*get_protocol)(const HttpRequestPtr this);
    const char *(*get_header)(const HttpRequestPtr this, const char *header);
    const char **(*get_headers)(const HttpRequestPtr this);
    const char *(*get_payload)(const HttpRequestPtr this);
    size_t (*get_payload_size)(const HttpRequestPtr this);
} HttpRequest = {
    .init = &request__init,
    .destroy = &request__destroy,
    .get_method = &request__get_method,
    .get_uri = &request__get_uri,
    .get_protocol = &request__get_protocol,
    .get_header = &request__get_header,
    .get_headers = &request__get_headers,
    .get_payload = &request__get_payload,
    .get_payload_size = &request__get_payload_size,
};


#endif

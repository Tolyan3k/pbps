#ifndef __HTTP_ROUTER_H__
#define __HTTP_ROUTER_H__


#include "request.h"
#include "server.h"
#include "common.h"


#define HTTP_200 printf("%s 200 OK\n\n", HTTP_RESPONSE_PROTOCOL)
#define HTTP_201 printf("%s 201 Created\n\n", HTTP_RESPONSE_PROTOCOL)
#define HTTP_404 printf("%s 404 Not found\n\n", HTTP_RESPONSE_PROTOCOL)
#define HTTP_500 printf("%s 500 Internal Server Error\n\n", HTTP_RESPONSE_PROTOCOL)

#define ROUTER_START() if (0) {
#define ROUTE(METHOD, URI, HTTP_REQ_PTR)                                       \
    }                                                                          \
    else if (!Router.request(METHOD, URI, HTTP_REQ_PTR)) {
#define GET(URI, HTTP_REQ_PTR) ROUTE(HTTP_METHOD_GET, URI, HTTP_REQ_PTR)
#define POST(URI, HTTP_REQ_PTR) ROUTE(HTTP_METHOD_POST, URI, HTTP_REQ_PTR)
#define ROUTER_END()                                                           \
    }                                                                          \
    else HTTP_500;


void route(
    const HttpRequestPtr request,
    const HttpServerSettingsPtr server_settings
);

int router__request(
    const char *method, const char *uri, const HttpRequestPtr request
);
int router__get(const char *uri, const HttpRequestPtr request);
int router__post(const char *uri, const HttpRequestPtr request);

static const struct {
    int (*request)(
        const char *method, const char *uri, const HttpRequestPtr request);
    int (*get)(const char *uri, const HttpRequestPtr request);
    int (*post)(const char *uri, const HttpRequestPtr request);
} Router = {
    .request = &router__request,
    .get = &router__get,
    .post = &router__post,
};


#endif

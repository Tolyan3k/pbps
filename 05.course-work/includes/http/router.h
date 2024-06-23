#ifndef __HTTP_ROUTER_H__
#define __HTTP_ROUTER_H__

#include <stdio.h>

#include "request.h"
#include "server.h"
#include "common.h"


#define HTTP_200(FILE) fprintf(FILE, "%s 200 OK\n\n", HTTP_RESPONSE_PROTOCOL)
#define HTTP_201(FILE) fprintf(FILE, "%s 201 Created\n\n", HTTP_RESPONSE_PROTOCOL)
#define HTTP_404(FILE) fprintf(FILE, "%s 404 Not found\n\n", HTTP_RESPONSE_PROTOCOL)
#define HTTP_500(FILE) fprintf(FILE, "%s 500 Internal Server Error\n\n", HTTP_RESPONSE_PROTOCOL)

#define ROUTER_START() if (0) {
#define ROUTE(METHOD, URI, HTTP_REQ_PTR)                                       \
    }                                                                          \
    else if (!Router.request(METHOD, URI, HTTP_REQ_PTR)) {
#define GET(URI, HTTP_REQ_PTR) ROUTE(HTTP_METHOD_GET, URI, HTTP_REQ_PTR)
#define POST(URI, HTTP_REQ_PTR) ROUTE(HTTP_METHOD_POST, URI, HTTP_REQ_PTR)
#define ROUTER_END(FILE)                                                           \
    }                                                                          \
    else HTTP_500(FILE);


void route(
    const HttpRequestPtr request,
    const HttpServerSettingsPtr server_settings,
    FILE *file_ptr
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

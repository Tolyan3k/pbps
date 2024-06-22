#include "../include/http.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "../include/utils.h"


char* _uri_unescape(char* uri);


typedef struct HTTP_REQUEST {
    DICT *headers;
    char *_raw;
    char *method;
    char *uri;
    char *queries;
    char *proto;
    char *payload;
    size_t payload_size;
} HTTP_REQUEST;

HttpRequestPtr request__init(const char *request_raw) {
    HttpRequestPtr this = (HttpRequestPtr)malloc(sizeof(HTTP_REQUEST));
    this->headers = Dict.init();
    this->_raw = (char*)malloc(sizeof(char) * (strlen(request_raw) + 1));
    this->_raw[strlen(request_raw)] = '\0';
    strncpy(this->_raw, request_raw, strlen(request_raw));
    this->method = strtok(this->_raw, " \t\r\n");
    this->uri = _uri_unescape(strtok(NULL, " \t"));
    this->proto = strtok(NULL, " \t\r\n");
    this->queries = strchr(this->uri, '?');
    if (this->queries)
        *this->queries++ = '\0'; // split URI
    else
        this->queries = this->uri - 1; // use an empty string
    char *t;
    const char *t2;
    while (1) {
        char *key, *value;
        key = strtok(NULL, "\r\n: \t");
        if (key == NULL) {
            break;
        }
        value = strtok(NULL, "\r\n");
        while (*value && *value == ' ') {
            value++;
        }
        Dict.set_value(this->headers, key, value);
        t = value + 1 + strlen(value);
        if (t[1] == '\r' && t[2] == '\n')
            break;
    }
    t = strtok(NULL, "\r\n");
    t2 = Dict.get_value(this->headers, HTTP_HEADER_CONTENT_LENGTH); // and the related header if there is
    this->payload = t;
    this->payload_size = t2 ? atoi(t2) : 0;
    return this;
}

void request__destroy(HttpRequestPtr this) {
    Dict.destroy(this->headers);
    free(this->_raw);
    free(this);
}

const char* request__get_method(const HttpRequestPtr this) {
    return this->method;
}

const char* request__get_uri(const HttpRequestPtr this) {
    return this->uri;
}

const char* request__get_protocol(const HttpRequestPtr this) {
    return this->proto;
}

const char* request__get_payload(const HttpRequestPtr this) {
    return this->payload;
}

size_t request__get_payload_size(const HttpRequestPtr this) {
    return this->payload_size;
}

const char* request__get_header(const HttpRequestPtr this, const char *header) {
    return Dict.get_value(this->headers, header);
}

const char **request__get_headers(const HttpRequestPtr this) {
    return NULL;
}


char* _uri_unescape(char *uri) {
    char chr = 0;
    char *src = uri;
    char *dst = uri;

    // Skip inital non encoded character
    while (*src && !isspace((int)(*src)) && (*src != '%'))
    src++;

    // Replace encoded characters with corresponding code.
    dst = src;
    while (*src && !isspace((int)(*src))) {
        if (*src == '+') {
            chr = ' ';
        }
        else if ((*src == '%') && src[1] && src[2]) {
            src++;
            chr = ((*src & 0x0F) + 9 * (*src > '9')) * 16;
            src++;
            chr += ((*src & 0x0F) + 9 * (*src > '9'));
        } else {
            chr = *src;
        }
        *dst++ = chr;
        src++;
    }
    *dst = '\0';
    return uri;
}

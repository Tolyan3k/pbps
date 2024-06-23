#include "http/router.h"

#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <stdio.h>
#include <fcntl.h>

#include "global.h"
#include "html.h"


int file_exists(const char *file_name) {
    struct stat buffer;
    int exists;

    exists = (open(file_name, O_RDONLY))!=-1;
    return exists;
}

int read_file(const char *file_name, FILE *out) {
    char buf[1024];
    FILE *file;
    size_t nread;
    int err = 1;

    file = fopen(file_name, "r");

    if (file) {
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
        fwrite(buf, 1, nread, out);

        err = ferror(file);
        fclose(file);
    }
    return err;
}


void route(
    const HttpRequestPtr request,
    const HttpServerSettingsPtr server_settings,
    FILE *file_ptr
) {
    const char *public_dir = server_settings->public_dir;

    ROUTER_START()

    GET("/", request) {
        char *index_html_path;
        index_html_path = malloc(strlen(public_dir) + strlen(HTML_INDEX));
        sprintf(index_html_path, "%s%s", public_dir, HTML_INDEX);

        HTTP_200(file_ptr);
        if (file_exists(index_html_path)) {
            read_file(index_html_path, file_ptr);
        } else {
            fprintf(file_ptr,
                "Hello! You are using %s\n\n",
                HttpRequest.get_header(request, "User-Agent"));
        }
        free(index_html_path);
        Logger.request(Global.logger, request, 200);
    }

    GET("/test", request) {
        HTTP_200(file_ptr);
        fprintf(file_ptr,"List of request headers:\n\n");
        const VectorPtr headers = Dict.items(HttpRequest.get_headers(request));
        for (int i = 0; i < Vector.size(headers); ++i) {
            const PairPtr pair = (PairPtr)Vector.get(headers, i);
            fprintf(file_ptr,"%s: %s\n", Pair.get_first(pair), Pair.get_second(pair));
        }
        Logger.request(Global.logger, request, 200);
    }

    POST("/", request) {
        HTTP_201(file_ptr);
        size_t payload_size = HttpRequest.get_payload_size(request);
        const char *payload = HttpRequest.get_payload(request);
        fprintf(file_ptr,"Wow, seems that you POSTed %ld bytes.\n", payload_size);
        fprintf(file_ptr,"Fetch the data using `payload` variable.\n");
        if (payload_size > 0) {
            fprintf(file_ptr,"Request body: %s", payload);
        }
        Logger.request(Global.logger, request, 201);
    }

    GET(HttpRequest.get_uri(request), request) {
        char *file_name;
        file_name = malloc(strlen(public_dir) + strlen(HttpRequest.get_uri(request)));
        sprintf(file_name, "%s%s", public_dir, HttpRequest.get_uri(request));

        if (file_exists(file_name)) {
            HTTP_200(file_ptr);
            read_file(file_name, file_ptr);
            Logger.request(Global.logger, request, 200);
        } else {
            HTTP_404(file_ptr);
            sprintf(file_name, "%s%s", public_dir, HTML_NOT_FOUND);
            if (file_exists(file_name))
                read_file(file_name, file_ptr);
            Logger.request(Global.logger, request, 201);
        }
        free(file_name);
    }

    ROUTER_END(file_ptr)
}

int router__request(
    const char *method, const char *uri, const HttpRequestPtr request
) {
    return (
        strcmp(method, HttpRequest.get_method(request))
        || strcmp(uri, HttpRequest.get_uri(request))
    );
}

int router__get(const char *uri, const HttpRequestPtr request) {
    return 0;
}

int router__post(const char *uri, const HttpRequestPtr request) {
    return 0;
}

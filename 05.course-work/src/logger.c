#include "logger.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct LOGGER {
    FILE *file_ptr;
    pthread_mutex_t mutex;
} LOGGER;


LoggerPtr logger__init(const char *log_file) {
    LoggerPtr this = (LoggerPtr)malloc(sizeof(LOGGER));
    this->file_ptr = NULL;
    if (pthread_mutex_init(&this->mutex, NULL)) {
        fprintf(
                stderr,
                "Failed to create mutex%s. Exiting\n",
                log_file);
        exit(EXIT_FAILURE);
    }
    if (log_file != NULL) {
        this->file_ptr = fopen(log_file, "a");
        if (this->file_ptr == NULL) {
            fprintf(
                stderr,
                "Failed to open/create log file %s. Exiting\n",
                log_file);
            exit(EXIT_FAILURE);
        }
    }
    return this;
};

void logger__destroy(LoggerPtr this) {
    if (this->file_ptr) {
        fclose(this->file_ptr);
    }
    free(this);
}

void logger__request(
    LoggerPtr this, const HttpRequestPtr request, size_t response_code
) {
    time_t rawtime;
    struct tm *timeinfo;
    char date[50];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date, 50, "%d/%b/%Y:%H:%M:%S %z", timeinfo);

    pthread_mutex_lock(&this->mutex);
    FILE *stream = this->file_ptr;
    if (stream == NULL) {
        stream = stderr;
    }
    fprintf(
        stream,
        "%s %s %s [%s] \"%s %s %s\" %ld %ld \"%s\" \"%s\"\r\n",
        HttpRequest.get_header(request, "Host"),
        "-",
        "-",
        date,
        HttpRequest.get_method(request),
        HttpRequest.get_uri(request),
        HttpRequest.get_protocol(request),
        response_code,
        HttpRequest.get_payload_size(request),
        HttpRequest.get_header(request, "Referer"),
        HttpRequest.get_header(request, "User-Agent")
    );
    fflush(stream);
    pthread_mutex_unlock(&this->mutex);
}

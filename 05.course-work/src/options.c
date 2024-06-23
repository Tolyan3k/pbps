#include "options.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "utils.h"


#define OPTS_PUBLIC_DIR_KEY "public_dir"
#define OPTS_PORT_KEY "port"
#define OPTS_LOG_FILE_KEY "log"


typedef struct OPTIONS {
    DictPtr options;
} OPTIONS;

OptionsPtr options__init(int argc, char **argv) {
    OptionsPtr this = (OptionsPtr)malloc(sizeof(OPTIONS));
    this->options = Dict.init();
    Dict.set_value(this->options, OPTS_PORT_KEY, OPTS_PORT_DEFAULT);
    Dict.set_value(this->options, OPTS_PUBLIC_DIR_KEY, OPTS_PUBLIC_DIR_DEFAULT);
    Dict.set_value(this->options, OPTS_LOG_FILE_KEY, OPTS_LOG_FILE_DEFAULT);
    char opt;
    while ((opt = getopt(argc, argv, "p:r:l:")) != -1) {
        switch (opt)
        {
        case 'r':
            Dict.set_value(this->options, OPTS_PUBLIC_DIR_KEY, optarg);
            break;
        case 'p':
            Dict.set_value(this->options, OPTS_PORT_KEY, optarg);
            break;
        case 'l':
            Dict.set_value(this->options, OPTS_LOG_FILE_KEY, optarg);
            break;
        case '?':
            printf("Wrong arguments\n");
            exit(EXIT_FAILURE);
        default:
            exit(EXIT_FAILURE);
            break;
        }
    }
    return this;
}

void options__destroy(OptionsPtr this) {
    Dict.destroy(this->options);
    free(this);
}

const char* options__get_port(const OptionsPtr this) {
    return Dict.get_value(this->options, OPTS_PORT_KEY);
}

const char* options__get_public_dir(const OptionsPtr this) {
    return Dict.get_value(this->options, OPTS_PUBLIC_DIR_KEY);
}

const char *options__get_log_file(const OptionsPtr this) {
    return Dict.get_value(this->options, OPTS_LOG_FILE_KEY);
}

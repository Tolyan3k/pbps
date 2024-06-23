#ifndef __OPTS_H__
#define __OPTS_H__

#define OPTS_PUBLIC_DIR_DEFAULT "./webroot"
#define OPTS_LOG_FILE_DEFAULT NULL
#define OPTS_PORT_DEFAULT "8000"


typedef struct OPTIONS OPTIONS;
typedef struct OPTIONS *OptionsPtr;


OptionsPtr options__init(int argc, char **argv);
void options__destroy(OptionsPtr this);
const char* options__get_port(const OptionsPtr this);
const char* options__get_public_dir(const OptionsPtr this);
const char *options__get_log_file(const OptionsPtr this);


static const struct Options {
    OptionsPtr (*init)(int argc, char **argv);
    void (*destroy)(OptionsPtr this);
    const char *(*get_port)(const OptionsPtr this);
    const char *(*get_public_dir)(const OptionsPtr this);
    const char *(*get_log_file)(const OptionsPtr this);
} Options = {
    .init = &options__init,
    .destroy = &options__destroy,
    .get_port = &options__get_port,
    .get_public_dir = &options__get_public_dir,
    .get_log_file = &options__get_log_file,
};


#endif

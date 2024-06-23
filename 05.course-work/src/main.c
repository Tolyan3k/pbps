#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "options.h"
#include "global.h"
#include "logger.h"
#include "http.h"


GLOBAL Global;


int main(int argc, char **argv)
{
    OptionsPtr opts = Options.init(argc, argv);
    Global.logger = Logger.init(Options.get_log_file(opts));
    HTTP_SERVER_SETTINGS server_settings = (HTTP_SERVER_SETTINGS){
        .max_connections = 1000,
        .buf_size = 65535,
        .queue_size = 1000000,
        .port = Options.get_port(opts),
        .public_dir = Options.get_public_dir(opts),
    };
    HttpServerPtr http_server = HttpServer.init(&server_settings);
    HttpServer.start(http_server);
    HttpServer.accept_connections(http_server);

    HttpServer.destroy(http_server);
    Options.destroy(opts);
}

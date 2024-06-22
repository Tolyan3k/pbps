#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "../include/options.h"
#include "../include/utils.h"
#include "../include/http.h"


int main(int argc, char **argv)
{
    OptionsPtr opts = Options.init(argc, argv);
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

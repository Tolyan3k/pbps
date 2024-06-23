#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "options.h"
#include "global.h"
#include "logger.h"
#include "http.h"


GLOBAL Global;


void sig_hdlr(int signum);
void register_signals(void);

static struct {
    OptionsPtr options;
    LoggerPtr logger;
    HttpServerPtr http_server;
} DELETE_AT_EXIT = {
    .options = NULL,
    .logger = NULL,
    .http_server = NULL,
};


int main(int argc, char **argv)
{
    register_signals();

    // Обработка опций и подготовка логера
    OptionsPtr opts = Options.init(argc, argv);
    Global.logger = Logger.init(Options.get_log_file(opts));
    DELETE_AT_EXIT.options = opts;
    DELETE_AT_EXIT.logger = Global.logger;

    // Настройка и запуск сервера
    HTTP_SERVER_SETTINGS server_settings = (HTTP_SERVER_SETTINGS){
        .max_connections = 1000,
        .buf_size = 65535,
        .queue_size = 1000000,
        .port = Options.get_port(opts),
        .public_dir = Options.get_public_dir(opts),
    };

    HttpServerPtr http_server = HttpServer.init(&server_settings);
    DELETE_AT_EXIT.http_server = http_server;
    HttpServer.start(http_server);
    HttpServer.accept_connections(http_server);
}


void sig_hdlr(int signum) {
    // Деструкторы для используемых в main переменных
    HttpServer.destroy(DELETE_AT_EXIT.http_server);
    Logger.destroy(DELETE_AT_EXIT.logger);
    Options.destroy(DELETE_AT_EXIT.options);
    exit(EXIT_SUCCESS);
}

void register_signals(void) {
    signal(SIGINT, sig_hdlr);
    signal(SIGTERM, sig_hdlr);
    signal(SIGQUIT, sig_hdlr);
}

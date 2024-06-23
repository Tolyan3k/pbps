#include "http/server.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>

#include "http.h"

void respond(int client_fd, const HttpServerSettingsPtr server_settings);
void process_connection(HttpServerPtr this);

typedef struct HTTP_SERVER {
    HttpServerSettingsPtr settings;
    int listen_fd;
    int *clients;
    int slot;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
} HTTP_SERVER;


HttpServerPtr server__init(const HttpServerSettingsPtr settings) {
    HttpServerPtr this = (HttpServerPtr)malloc(sizeof(HTTP_SERVER));
    this->settings = settings;
    this->listen_fd = 0;
    this->clients = NULL;
    this->slot = 0;

    // create shared memory for client slot array
    this->clients = mmap(
        NULL, sizeof(*this->clients) * this->settings->max_connections,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    // Setting all elements to -1: signifies there is no client connected
    int i;
    for (i = 0; i < this->settings->max_connections; i++) {
        this->clients[i] = -1;
    }

    return this;
}

void server__destroy(HttpServerPtr this) {
    free(this);
}

void server__start(HttpServerPtr this) {
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, this->settings->port, &hints, &res) != 0) {
        fprintf(stderr, "getaddrinfo() error");
        exit(1);
    }
    // socket and bind
    for (p = res; p != NULL; p = p->ai_next) {
        int option = 1;
        this->listen_fd = socket(p->ai_family, p->ai_socktype, 0);
        setsockopt(this->listen_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (this->listen_fd == -1)
        continue;
        if (bind(this->listen_fd, p->ai_addr, p->ai_addrlen) == 0)
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if (listen(this->listen_fd, this->settings->queue_size) != 0) {
        fprintf(stderr, "listen() error");
        exit(1);
    }

    openlog(NULL, LOG_PERROR || LOG_PID, LOG_DAEMON);
    syslog(
        LOG_INFO,
        "Server started %shttp://127.0.0.1:%s%s\n",
        "\033[92m", this->settings->port,"\033[0m");
    closelog();
}

void server__accept_connections(HttpServerPtr this) {
    // Ignore SIGCHLD to avoid zombie threads
    signal(SIGCHLD, SIG_IGN);

    // ACCEPT connections
    while (1) {
        this->addr_len = sizeof(this->client_addr);
        this->clients[this->slot] = accept(this->listen_fd, (struct sockaddr *)&(this->client_addr), &(this->addr_len));

        if (this->clients[this->slot] < 0) {
            fprintf(stderr, "accept() error");
            exit(EXIT_FAILURE);
        } else {
            process_connection(this);
        }

        while (this->clients[this->slot] != -1) {
            this->slot = (this->slot + 1) % this->settings->max_connections;
        }
    }
}

typedef struct PROC_CONN_INFO {
    int *client_fd;
    HttpServerSettingsPtr setings;
} PROC_CONN_INFO;

void* process_connection_routine(void* arg)
{
    PROC_CONN_INFO *test = (PROC_CONN_INFO*)arg;
    respond(*test->client_fd, test->setings);
    close(*test->client_fd);
    *test->client_fd = -1;
    free(arg);
}

void process_connection(HttpServerPtr this)
{
    PROC_CONN_INFO *test = (PROC_CONN_INFO*)malloc(sizeof(PROC_CONN_INFO));
    test->client_fd = &this->clients[this->slot];
    test->setings = this->settings;
    pthread_t thread;
    pthread_create(&thread, NULL, process_connection_routine, test);
}

void respond(int client_fd, const HttpServerSettingsPtr server_settings) {
    int buf_size = server_settings->buf_size;

    char *buf = (char*)malloc(sizeof(char) * buf_size);
    buf = (char*)memset(buf, '\0', buf_size);
    int rcvd = recv(client_fd, buf, buf_size, 0);

    if (rcvd < 0) // receive error
        fprintf(stderr, ("recv() error\n"));
    else if (rcvd == 0) // receive socket closed
        fprintf(stderr, "Client disconnected upexpectedly.\n");
    else // message received
    {
        HttpRequestPtr http_request = HttpRequest.init(buf);

        FILE *out = fdopen(client_fd, "w");

        // call router
        route(http_request, server_settings, out);
        HttpRequest.destroy(http_request);

        // tidy up
        fflush(out);
        shutdown(client_fd, SHUT_WR);
        close(client_fd);
    }

    free(buf);
}

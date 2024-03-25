#include "logger.h"

#include <time.h>
#include <syslog.h>

#include "httpd.h"


void log_request(int resp_code) {
    time_t rawtime;
    struct tm *timeinfo;
    char date[50];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date, 50, "%d/%b/%Y:%H:%M:%S %z", timeinfo);
    
    syslog(
        LOG_INFO,
        "%s - - [%s] \"%s %s %s\" %d %d \"%s\" \"%s\"",
        request_header("Host"),
        date,
        method,
        uri,
        prot,
        resp_code,
        payload_size,
        request_header("Referer"),
        request_header("User-Agent")
    );
}

#include "httpd.h"

#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>


#define CHUNK_SIZE 1024 // read 1024 bytes at a time

// Public directory settings
#define PUBLIC_DIR_DEFAULT "./webroot"
#define INDEX_HTML "/index.html"
#define NOT_FOUND_HTML "/404.html"
#define PORT_DEFAULT "8000"


char* PORT = PORT_DEFAULT;
char* PUBLIC_DIR = PUBLIC_DIR_DEFAULT;


int main(int argc, char **argv) {
  // char *port = argc == 1 ? "8000" : argv[1];
  
  char opt;
  while ((opt = getopt(argc, argv, "p:r:")) != -1) {
    switch (opt)
    {
    case 'r':
      PUBLIC_DIR = malloc(strlen(optarg));
      strcpy(PUBLIC_DIR, optarg);
      break;
    case 'p':
      PORT = malloc((strlen(optarg)));
      strcpy(PORT, optarg);
      break;
    case '?':
      fprintf(stderr, "Wrong arguments given!!!\n");
      exit(EXIT_FAILURE);
    default:
      exit(EXIT_FAILURE);
      break;
    }
  }
  
  serve_forever(PORT);
  return 0;
}

int file_exists(const char *file_name) {
  struct stat buffer;
  int exists;

  exists = (open(file_name, O_RDONLY))!=-1; //(stat(file_name, &buffer) == 0);
  fprintf(stderr, "\'%s\'", file_name);
  fprintf(stderr, exists ? " FOUND\n" : " NOT FOUND\n");
  return exists;
  // int exists = 0;
  // int fd = open(file_name, O_RDONLY);
  // exists = fd != -1;
  // close(fd);
  // fprintf(stderr, "\'%s\'", file_name);
  // fprintf(stderr, "\'%d\'", exists);
  // return exists;
}

int read_file(const char *file_name) {
  char buf[CHUNK_SIZE];
  FILE *file;
  size_t nread;
  int err = 1;

  file = fopen(file_name, "r");

  if (file) {
    while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
      fwrite(buf, 1, nread, stdout);

    err = ferror(file);
    fclose(file);
  }
  return err;
}

void route() {
  ROUTE_START()

  GET("/") {
    char *index_html;
    index_html = malloc(strlen(PUBLIC_DIR) + strlen(INDEX_HTML));
    sprintf(index_html, "%s%s", PUBLIC_DIR, INDEX_HTML);
    fprintf(stderr, "%s\n", index_html);
    
    HTTP_200;
    if (file_exists(index_html)) {
      read_file(index_html);
    } else {
      printf("Hello! You are using %s\n\n", request_header("User-Agent"));
    }
    free(index_html);
  }

  GET("/test") {
    HTTP_200;
    printf("List of request headers:\n\n");

    header_t *h = request_headers();

    while (h->name) {
      printf("%s: %s\n", h->name, h->value);
      h++;
    }
  }

  POST("/") {
    HTTP_201;
    printf("Wow, seems that you POSTed %d bytes.\n", payload_size);
    printf("Fetch the data using `payload` variable.\n");
    if (payload_size > 0)
      printf("Request body: %s", payload);
  }

  GET(uri) {
    char *file_name;
    fprintf(stderr, "VVVVVVVVVVVVVVVVVVVVV\n");
    file_name = malloc(strlen(PUBLIC_DIR) + strlen(uri));
    sprintf(file_name, "%s%s", PUBLIC_DIR, uri);

    if (file_exists(file_name)) {
      HTTP_200;
      read_file(file_name);
    } else {
      HTTP_404;
      sprintf(file_name, "%s%s", PUBLIC_DIR, NOT_FOUND_HTML);
      if (file_exists(file_name))
        read_file(file_name);
    }
    free(file_name);
  }

  ROUTE_END()
}

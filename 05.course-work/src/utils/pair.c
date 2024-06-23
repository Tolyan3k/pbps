#include "utils/pair.h"

#include <stdlib.h>
#include <string.h>


typedef struct PAIR {
    char *first;
    char *second;
} PAIR;

PairPtr pair__init() {
    PairPtr this = (PairPtr)malloc(sizeof(PAIR));
    this->first = NULL;
    this->second = NULL;
    return this;
}

void pair__destroy(PairPtr this) {
    free(this->second);
    free(this->first);
    free(this);
}

void pair__set_first(PairPtr this, const char *first) {
    free(this->first);
    this->first = (char*)malloc(sizeof(char) * (strlen(first) + 1));
    strncpy(this->first, first, strlen(first));
    this->first[strlen(first)] = '\0';
}

const char* pair__get_first(const PairPtr this) {
    return this->first;
}

void pair__set_second(PairPtr this, const char *second) {
    free(this->second);
    if (second == NULL) {
        this->second = NULL;
        return;
    }
    this->second = (char*)malloc(sizeof(char) * (strlen(second) + 1));
    strncpy(this->second, second, strlen(second));
    this->second[strlen(second)] = '\0';
}

const char* pair__get_second(const PairPtr this) {
    return this->second;
}

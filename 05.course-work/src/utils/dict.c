#include "utils/dict.h"
#include "utils/pair.h"

#include <stdlib.h>
#include <string.h>


typedef struct DICT {
    PairPtr *records;
    size_t size_current;
    size_t size_max;
} DICT;


DictPtr dict__init() {
    DictPtr this = (DictPtr )malloc(sizeof(DICT));
    this->records = NULL;
    this->size_current = 0;
    this->size_max = 0;
    return this;
}

void dict__destroy(DictPtr this) {
    for (int i = 0; i < this->size_current; ++i) {
        Pair.destroy(this->records[i]);
    }
    free(this->records);
    free(this);
}

void dict__set_value(DictPtr this, const char *key, const char *value) {
    PairPtr record = Pair.init();
    Pair.set_first(record, key);
    Pair.set_second(record, value);
    Dict.delete_value(this, key);
    if (this->size_current == this->size_max) {
        int new_size_max = this->size_max + 10;
        PairPtr *new_records = (PairPtr*)malloc(sizeof(PairPtr) * new_size_max);
        for (int i = 0; i < this->size_current; ++i) {
            new_records[i] = this->records[i];
        }
        free(this->records);
        this->records = new_records;
        this->size_max = new_size_max;
    }
    this->records[this->size_current] = record;
    this->size_current += 1;
}

const char* dict__get_value(const DictPtr this, const char *key) {
    for (int i = 0; i < this->size_current; ++i) {
        PairPtr pair = this->records[i];
        if (!strcmp(Pair.get_first(pair), key)) {
            return Pair.get_second(pair);
        }
    }
    return NULL;
}

void dict__delete_value(DictPtr this, const char *key) {
    for (int i = 0; i < this->size_current; ++i) {
        PairPtr pair = this->records[i];
        if (!strcmp(Pair.get_first(pair), key)) {
            Pair.destroy(pair);
            this->records[i] = this->records[this->size_current - 1];
            if (this->size_current > 0) --this->size_current;
        }
    }
}
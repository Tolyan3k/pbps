#include "utils/dict.h"

#include <stdlib.h>
#include <string.h>

#include "utils/pair.h"
#include "utils/vector.h"


typedef struct DICT {
    VectorPtr records;
} DICT;


DictPtr dict__init() {
    DictPtr this = (DictPtr)malloc(sizeof(DICT));
    this->records = Vector.init();
    return this;
}

void dict__destroy(DictPtr this) {
    for (int i = 0; i < Vector.size(this->records); ++i) {
        PairPtr pair = (PairPtr)Vector.get(this->records, i);
        Pair.destroy(pair);
    }
    Vector.destroy(this->records);
    free(this);
}

void dict__set_value(DictPtr this, const char *key, const char *value) {
    PairPtr record = Pair.init();
    Pair.set_first(record, key);
    Pair.set_second(record, value);
    dict__delete_value(this, key);
    Vector.push_back(this->records, record);
}

const char* dict__get_value(const DictPtr this, const char *key) {
    for (int i = 0; i < Vector.size(this->records); ++i) {
        const PairPtr pair = (PairPtr)Vector.get(this->records, i);
        if (!strcmp(Pair.get_first(pair), key)) {
            return Pair.get_second(pair);
        }
    }
    return NULL;
}

void dict__delete_value(DictPtr this, const char *key) {
    for (int i = 0; i < Vector.size(this->records); ++i) {
        PairPtr pair = (PairPtr)Vector.get(this->records, i);
        if (!strcmp(Pair.get_first(pair), key)) {
            Pair.destroy(pair);
            Vector.set(this->records, i, Vector.back(this->records));
            Vector.pop_back(this->records);
        }
    }
}

const VectorPtr dict__get_items(const DictPtr this) {
    return this->records;
}

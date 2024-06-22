#ifndef __UTILS_H__
#define __UTILS_H__

#include <stddef.h>


typedef struct DICT DICT;
typedef struct DICT *DictPtr;
DICT *dict__init();
void dict__destroy(DictPtr this);
void dict__set_value(DictPtr this, const char *key, const char* value);
const char* dict__get_value(const DictPtr this, const char *key);
void dict__delete_value(DictPtr this, const char *key);

static const struct Dict {
    DICT *(*init)(void);
    void (*destroy)(DictPtr this);
    void (*set_value)(DictPtr this, const char *key, const char *value);
    const char *(*get_value)(const DictPtr this, const char *key);
    void (*delete_value)(DictPtr this, const char *key);
} Dict = {
    .init = &dict__init,
    .destroy = &dict__destroy,
    .set_value = &dict__set_value,
    .get_value = &dict__get_value,
    .delete_value = &dict__delete_value,
};


typedef struct PAIR PAIR;
typedef struct PAIR *PairPtr;
PAIR* pair__init();
void pair__destroy(PairPtr this);
void pair__set_first(PairPtr this, const char *first);
const char* pair__get_first(const PairPtr this);
void pair__set_second(PairPtr this, const char *second);
const char* pair__get_second(const PairPtr this);

static const struct Pair {
    PairPtr (*init)(void);
    void (*destroy)(PairPtr this);
    void (*set_first)(PairPtr this, const char *first);
    const char *(*get_first)(const PairPtr this);
    void (*set_second)(PairPtr this, const char *second);
    const char *(*get_second)(const PairPtr this);
} Pair = {
    .init = &pair__init,
    .destroy = &pair__destroy,
    .set_first = &pair__set_first,
    .get_first = &pair__get_first,
    .set_second = &pair__set_second,
    .get_second = &pair__get_second,
};

#endif

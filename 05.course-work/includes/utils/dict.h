#ifndef __UTILS__DICT_H__
#define __UTILS__DICT_H__

#include <stddef.h>

#include "utils/vector.h"


typedef struct DICT DICT;
typedef struct DICT *DictPtr;

DictPtr dict__init();
void dict__destroy(DictPtr this);
void dict__set_value(DictPtr this, const char *key, const char* value);
const char* dict__get_value(const DictPtr this, const char *key);
void dict__delete_value(DictPtr this, const char *key);
const VectorPtr dict__get_items(const DictPtr this);

static const struct Dict {
    DictPtr (*init)(void);
    void (*destroy)(DictPtr this);
    void (*set_value)(DictPtr this, const char *key, const char *value);
    const char *(*get_value)(const DictPtr this, const char *key);
    void (*delete_value)(DictPtr this, const char *key);
    const VectorPtr (*items)(const DictPtr this);
} Dict = {
    .init = &dict__init,
    .destroy = &dict__destroy,
    .set_value = &dict__set_value,
    .get_value = &dict__get_value,
    .delete_value = &dict__delete_value,
    .items = &dict__get_items,
};


#endif

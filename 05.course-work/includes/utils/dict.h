#ifndef __UTILS__DICT_H__
#define __UTILS__DICT_H__


typedef struct DICT DICT;
typedef struct DICT *DictPtr;


DictPtr dict__init();
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


#endif

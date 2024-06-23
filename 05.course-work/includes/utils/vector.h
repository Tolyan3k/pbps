#ifndef __UTILS__VECTOR_H__
#define __UTILS__VECTOR_H__

#include <stddef.h>


typedef struct VECTOR VECTOR;
typedef struct VECTOR *VectorPtr;

VectorPtr vector__init();
void vector__destroy(VectorPtr this);
void vector__push_back(VectorPtr this, void *item_ptr);
void vector__set(VectorPtr this, size_t index, void *item_ptr);
void* vector__get(VectorPtr this, size_t index);
void* vector__back(VectorPtr this);
void vector__pop(VectorPtr this, size_t index);
void vector__pop_back(VectorPtr this);
size_t vector__size(const VectorPtr this);
void vector__resize(VectorPtr this, size_t new_size);
void vector__shrink_to_fit(VectorPtr this);

static const struct Vector {
    VectorPtr (*init)(void);
    void (*destroy)(VectorPtr this);
    void (*push_back)(VectorPtr this, void *item_ptr);
    void (*set)(VectorPtr this, size_t index, void *item_ptr);
    void* (*get)(VectorPtr this, size_t index);
    void* (*back)(VectorPtr this);
    void (*pop)(VectorPtr this, size_t index);
    void (*pop_back)(VectorPtr this);
    size_t (*size)(const VectorPtr this);
    void (*resize)(VectorPtr this, size_t new_size);
    void (*shrink_to_fit)(VectorPtr this);
} Vector = {
    .init = &vector__init,
    .destroy = &vector__destroy,
    .push_back = &vector__push_back,
    .set = &vector__set,
    .get = &vector__get,
    .back = &vector__back,
    .pop = &vector__pop,
    .pop_back = &vector__pop_back,
    .size = &vector__size,
    .resize = &vector__resize,
    .shrink_to_fit = &vector__shrink_to_fit,
};


#endif

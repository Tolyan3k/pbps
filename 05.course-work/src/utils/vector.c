#include "utils/vector.h"

#include <stdlib.h>


typedef struct VECTOR {
    void **array;
    size_t size_cur;
    size_t size_max;
} VECTOR;


VectorPtr vector__init() {
    VectorPtr this = (VectorPtr)malloc(sizeof(VECTOR));
    this->array = NULL;
    this->size_cur = 0;
    this->size_max = 0;
    return this;
}

void vector__destroy(VectorPtr this) {
    free(this->array);
    free(this);
}

void vector__push_back(VectorPtr this, void *item_ptr) {
    if (this->size_cur == this->size_max) {
        int new_size_max = this->size_max + 10;
        vector__resize(this, new_size_max);
    }
    this->array[this->size_cur] = item_ptr;
    this->size_cur += 1;
}

void vector__set(VectorPtr this, size_t index, void *item_ptr) {
    if (index >= this->size_max) {
        return;
    }
    this->array[index] = item_ptr;
}

void* vector__get(VectorPtr this, size_t index) {
    if (this->size_cur <= index) {
        return NULL;
    }
    return this->array[index];
}

void* vector__back(VectorPtr this) {
    if (this->size_cur > 0) {
        return this->array[this->size_cur - 1];
    }
    return NULL;
}

void vector__pop(VectorPtr this, size_t index) {
    if (this->size_cur <= index) {
        return;
    }
    for (int i = index; i + 1 < this->size_cur; ++i) {
        this->array[i] = this->array[i + 1];
    }
    if (this->size_cur > 0) {
        this->size_cur -= 1;
    }
}

void vector__pop_back(VectorPtr this) {
    if (this->size_cur > 0) {
        this->size_cur -= 1;
    }
}

size_t vector__size(const VectorPtr this) {
    return this->size_cur;
}

void vector__resize(VectorPtr this, size_t new_size) {
    if (this->size_cur >= new_size) {
        return;
    }
    void **new_array = (void**)malloc(sizeof(void*) * new_size);
    for (int i = 0; i < this->size_cur; ++i) {
        new_array[i] = this->array[i];
    }
    free(this->array);
    this->array = new_array;
    this->size_max = new_size;
}

void vector__shrink_to_fit(VectorPtr this) {
    vector__resize(this, this->size_cur);
}

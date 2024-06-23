#ifndef __UTILS__PAIR_H__
#define __UTILS__PAIR_H__


typedef struct PAIR PAIR;
typedef struct PAIR *PairPtr;

PairPtr pair__init();
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

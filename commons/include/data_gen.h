//
// Created by badcw on 2022/3/29.
//

#ifndef CLIBS_DATA_GEN_INCLUDE_DATA_GEN_H
#define CLIBS_DATA_GEN_INCLUDE_DATA_GEN_H

#include <limits.h>    // define LONG_MIN and LONG_MAX
#include <stdint.h>    // define uintptr_t
#include <stdtypes.h>  // define ulong_t

#ifdef __cplusplus
namespace data_gen {
#endif

#define MOST_LEFT_RANGE LONG_MIN
#define MOST_RIGHT_RANGE LONG_MAX

typedef enum generator_mode {
    RANGE_MODE        = 1,
    CONTAINS_MODE     = 2,
    NOT_CONTAINS_MODE = 4,

    // shouldn't be used
    ALL_MODE     = 7,
    REV_ALL_MODE = ~((uintptr_t)7),
} generator_mode_t;

typedef struct data_generator {
    ulong_t range_left, range_right;
#ifndef _LP64
    short mode;
#endif /*_LP64*/
    /* last 3 bit is mode, others are contains pointer */
    uintptr_t contains;
    uintptr_t not_contains;
    ulong_t   contains_dir[];
} data_generator_t;

#define MODE_OFFSET ((sizeof(ulong_t)) * 2)

#define GET_MODE(g) (*((uintptr_t)(g) + MODE_OFFSET) & ALL_MODE)
#define GET_CONTAINS(g) (*((uintptr_t)(g) + CONTAINS_OFFSET) & REV_ALL_MODE)

#ifndef _LP64
#define CONTAINS_OFFSET ((sizeof(ulong_t)) * 2 + (sizeof(short)))
#define SET_MODE_ON(g, mod) (*(short*)((uintptr_t)(g) + MODE_OFFSET) |= (mod))
#define SET_MODE_OFF(g, mod) (*(short*)((uintptr_t)(g) + MODE_OFFSET) &= (ALL_MODE ^ (mod)))
#else
#define CONTAINS_OFFSET ((sizeof(ulong_t)) * 2)
#define SET_MODE_ON(g, mod) (*(ulong_t*)((uintptr_t)(g) + MODE_OFFSET) |= (mod))
#define SET_MODE_OFF(g, mod) (*(ulong_t*)((uintptr_t)(g) + MODE_OFFSET) &= (7 ^ (mod)))
#endif

/* Operations of generator */
extern void* gen_gen();

extern void set_range_to_data_generator(data_generator_t* gen, ulong_t left, ulong_t right);
extern void set_contains_to_data_generator(data_generator_t* gen, ulong_t* contains);
extern void set_not_contains_to_data_generator(data_generator_t* gen, ulong_t* not_contains);
extern void set_data_generator_mode_off(data_generator_t* gen, generator_mode_t mode);

/* Do generating */
extern ulong_t gen_one(data_generator_t* gen);
extern ulong_t gen(data_generator_t* gen, int len);

extern void gen_ptr(data_generator_t* gen, void* ptr);

#ifdef __cplusplus
}
#endif

#endif  // CLIBS_DATA_GEN_INCLUDE_DATA_GEN_H

//
// Created by badcw on 2022/3/29.
//

#include <data_gen.h>
#include <stdlib.h>
#include <mt_19937_64.h>

void* gen_gen() {
    data_generator_t* gen = malloc(sizeof(struct data_generator));
    SET_MODE_OFF(gen, 7);
    return gen;
}

void set_range_to_data_generator(data_generator_t* gen, ulong_t left, ulong_t right) {
    SET_MODE_ON(gen, RANGE_MODE);
}

void set_contains_to_data_generator(data_generator_t* gen, ulong_t* contains);

void set_not_contains_to_data_generator(data_generator_t* gen, ulong_t* not_contains);

void set_data_generator_mode_off(data_generator_t* gen, generator_mode_t mode) {
    SET_MODE_OFF(gen, mode);
}

/* Do generating */
ulong_t gen_one(data_generator_t* gen);
ulong_t gen(data_generator_t* gen, int len);

void gen_ptr(data_generator_t* gen, void* ptr);
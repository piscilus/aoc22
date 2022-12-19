/*
 * Day 17 - module to handle rock types.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#ifndef ROCK_H_
#define ROCK_H_

#include <stdlib.h>

#define ROCK_WIDTH_MAX  (4)
#define ROCK_HEIGHT_MAX (4)

typedef struct
{
    char fig[ROCK_HEIGHT_MAX][ROCK_WIDTH_MAX];
    size_t width;
    size_t height;
} rock_t;

/* get (next) rock, loops through all types */
const rock_t*
rock_get();

void
rock_print(const rock_t* r);

#endif /* ROCK_H_ */

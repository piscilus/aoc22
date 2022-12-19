/*
 * Day 17 - module to handle rock types.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "rocks.h"

#include <stdio.h>

const rock_t rocks[] =
{
    {
        .fig = {
                    {'@', '@', '@', '@'},
                    {'.', '.', '.', '.'},
                    {'.', '.', '.', '.'},
                    {'.', '.', '.', '.'},
                },
        .width = 4,
        .height = 1
    },
    {
        .fig = {
                    {'.', '@', '.', '.'},
                    {'@', '@', '@', '.'},
                    {'.', '@', '.', '.'},
                    {'.', '.', '.', '.'},
                },
        .width = 3,
        .height = 3
    },
    {
        .fig = {
                    {'.', '.', '@', '.'},
                    {'.', '.', '@', '.'},
                    {'@', '@', '@', '.'},
                    {'.', '.', '.', '.'},
                },
        .width = 3,
        .height = 3
    },
    {
        .fig = {
                    {'@', '.', '.', '.'},
                    {'@', '.', '.', '.'},
                    {'@', '.', '.', '.'},
                    {'@', '.', '.', '.'},
                },
        .width = 1,
        .height = 4
    },
    {
        .fig = {
                    {'@', '@', '.', '.'},
                    {'@', '@', '.', '.'},
                    {'.', '.', '.', '.'},
                    {'.', '.', '.', '.'},
                },
        .width = 2,
        .height = 2
    }
};

const rock_t*
rock_get()
{
    static size_t count = 0U;

    if (count >= (sizeof(rocks) / sizeof(rock_t)))
        count = 0U;

    return &rocks[count++];
}

void
rock_print(const rock_t* r)
{
    for (size_t y = 0U; y < r->height; y++)
    {
        for (size_t x = 0U; x < r->width; x++)
        {
            putchar(r->fig[y][x]);
        }
        putchar('\n');
    }
}

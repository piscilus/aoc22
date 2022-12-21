/*
 * Main program for advent of code 2022 day 21.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "hashtable.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define CHUNK_SIZE (128U)

typedef enum
{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_NONE
} opsel_t;

typedef struct
{
    char id[5];
    char op_l[5];
    char op_r[5];
    char op_raw;
    opsel_t op;
    uint64_t num;
} data_t;

static int
add_to_hashtab(hashtable_t* ht, data_t* data);

static uint64_t
solve(hashtable_t* ht, char* key);

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 21: Monkey Math\n\n");

    if (argc != 2)
    {
        fprintf(stderr, "Please provide data record file name.");
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen(argv[1], "r");

    if (!fp)
    {
        fprintf(stderr, "Could not open file!");
        exit(EXIT_FAILURE);
    }

    hashtable_t* ht = ht_init(100U);

    char line_buf[CHUNK_SIZE];
    while (fgets(line_buf, CHUNK_SIZE, fp))
    {
        data_t data = {0};
        if (sscanf( line_buf,
                    "%4s: %4s %c %4s",
                    data.id,
                    data.op_l,
                    &data.op_raw,
                    data.op_r) == 4)
        {
            switch(data.op_raw)
            {
                case '+':
                    data.op = OP_ADD;
                    break;
                case '-':
                    data.op = OP_SUB;
                    break;
                case '*':
                    data.op = OP_MUL;
                    break;
                case '/':
                    data.op = OP_DIV;
                    break;
                default:
                    assert(0);
            }
            if (!add_to_hashtab(ht, &data))
                assert(0);
        }
        else if (sscanf(line_buf,
                        "%4s: %llu",
                        data.id,
                        &data.num) == 2)
        {
            data.op = OP_NONE;
            if (!add_to_hashtab(ht, &data))
                assert(0);
        }
    }

    fclose(fp);

    clock_t t1 = clock();

    uint64_t result = solve(ht, "root");

    clock_t t2 = clock();
    printf("Part 1: number = %llu\n", result);

    double elapsed_time = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("solving puzzle took %f s\n", elapsed_time);

    ht_destroy(ht);

    return EXIT_SUCCESS;
}

static int
add_to_hashtab(hashtable_t* ht, data_t* data)
{
    void* d = ht_read(ht, data->id);

    if (d != NULL)
    {
        assert(0);
    }
    else
    {
        data_t* new_data = malloc(sizeof(data_t));
        if (new_data == NULL)
            return 0;
        *new_data = *data;
        if (ht_write(ht, data->id, new_data))
            return 1;
    }

    return 1;
}

static uint64_t
solve(hashtable_t* ht, char* key)
{
    data_t* d = ht_read(ht, key);
    uint64_t result = 0;

    if (d->op == OP_NONE)
        result = d->num;
    else if (d->op == OP_ADD)
        result = solve(ht, d->op_l) + solve(ht, d->op_r);
    else if (d->op == OP_SUB)
        result = solve(ht, d->op_l) - solve(ht, d->op_r);
    else if (d->op == OP_MUL)
        result = solve(ht, d->op_l) * solve(ht, d->op_r);
    else if (d->op == OP_DIV)
        result = solve(ht, d->op_l) / solve(ht, d->op_r);
    else
        assert(0);

    return result;
}

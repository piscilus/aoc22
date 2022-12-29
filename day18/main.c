/*
 * Main program for advent of code 2022 day 18.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHUNK_SIZE (128U)
#define NUM_CUBES  (3000)

typedef union
{
    struct
    {
        int8_t x;
        int8_t y;
        int8_t z;
        int8_t res_;
    } coords;
    int32_t code;
} data_t;

typedef struct cube
{
    data_t data;
    struct cube* next;
} cube_t;

int
hash_table_insert(cube_t* c);
int
hash_table_search(data_t d);
void
hash_table_free(void);
void
hash_table_print(void);

cube_t* hash_table[NUM_CUBES];

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 18: Boiling Boulders\n\n");

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

    for (int i = 0; i < NUM_CUBES; i++)
    {
        hash_table[i] = NULL;
    }

    cube_t cube = {0};
    char line_buf[CHUNK_SIZE];
    while (fgets(line_buf, CHUNK_SIZE, fp))
    {
        if (sscanf( line_buf,
                    "%hhd,%hhd,%hhd",
                    &cube.data.coords.x,
                    &cube.data.coords.y,
                    &cube.data.coords.z) == 3)
        {
            cube.data.coords.res_ = 0;
            int r = hash_table_insert(&cube);
            assert(r >= 0);
        }
        else
        {
            assert(0);
        }
    }

    fclose(fp);

    int count = 0;
    for (int i = 0; i < NUM_CUBES; i++ )
    {
        if (hash_table[i] != NULL)
        {
            cube_t* tmp = hash_table[i];
            while (tmp != NULL)
            {
                /* check if there are cubes on the six possible adjacent positions */
                if (!hash_table_search(
                        (data_t) {
                                .coords.x = tmp->data.coords.x,
                                .coords.y = tmp->data.coords.y,
                                .coords.z = tmp->data.coords.z - 1}))
                    count++;
                if (!hash_table_search(
                        (data_t) {
                                .coords.x = tmp->data.coords.x,
                                .coords.y = tmp->data.coords.y,
                                .coords.z = tmp->data.coords.z + 1}))
                    count++;
                if (!hash_table_search(
                        (data_t) {
                                .coords.x = tmp->data.coords.x,
                                .coords.y = tmp->data.coords.y - 1,
                                .coords.z = tmp->data.coords.z}))
                    count++;
                if (!hash_table_search(
                        (data_t) {
                                .coords.x = tmp->data.coords.x,
                                .coords.y = tmp->data.coords.y + 1,
                                .coords.z = tmp->data.coords.z}))
                    count++;
                if (!hash_table_search(
                        (data_t) {
                                .coords.x = tmp->data.coords.x - 1,
                                .coords.y = tmp->data.coords.y,
                                .coords.z = tmp->data.coords.z}))
                    count++;
                if (!hash_table_search(
                        (data_t) {
                                .coords.x = tmp->data.coords.x + 1,
                                .coords.y = tmp->data.coords.y,
                                .coords.z = tmp->data.coords.z}))
                    count++;
                tmp = tmp->next;
            }
        }
    }

    printf("Part 1: %d\n", count);

    hash_table_free();

    return EXIT_SUCCESS;
}

int
hash_table_insert(cube_t* c)
{
    if (c == NULL)
        return -1;
    int index = c->data.code % NUM_CUBES;
    cube_t* new = (cube_t*) malloc(sizeof(cube_t));
    if (new == NULL)
        return -1;
    new->data = c->data;
    new->next = NULL;
    if (hash_table[index] == NULL)
    {
        hash_table[index] = new;
    }
    else
    {
        cube_t* tmp = hash_table[index];
        while(tmp->next != NULL)
        {
            if (tmp->data.code == c->data.code)
                return 0;
            tmp = tmp->next;
        }
        tmp->next = new;
    }

    return 1;
}

int
hash_table_search(data_t d)
{
    int index = d.code % NUM_CUBES;
    if (hash_table[index] != NULL)
    {
        if (hash_table[index]->data.code == d.code)
            return 1;
        else
        {
            cube_t* tmp = hash_table[index]->next;
            while(tmp != NULL)
            {
                if (tmp->data.code == d.code)
                    return 1;
                tmp = tmp->next;
            }
        }
    }

    return 0;
}

void
hash_table_free(void)
{
    for (int i = 0; i < NUM_CUBES; i++ )
    {
        if (hash_table[i] != NULL)
        {
            cube_t* head = hash_table[i]->next;
            while (head != NULL)
            {
                cube_t* tmp = head;
                head = head->next;
                free(tmp);
            }
        }
    }
}

void
hash_table_print(void)
{
    for (int i = 0; i < NUM_CUBES; i++ )
    {
        if (hash_table[i] != NULL)
        {
            printf("[%i] ", i);
            cube_t* tmp = hash_table[i];
            while (tmp != NULL)
            {
                printf("%d ", tmp->data.code);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
}

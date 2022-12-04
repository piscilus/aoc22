/**
 * \file main.c
 *
 * \copyright (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 *
 * \brief Main program for advent of code 2022 day 3.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUCKSACK_CAPACITY (128U)

typedef struct
{
    size_t items_num;              /* number of items in the rucksack */
    char items[RUCKSACK_CAPACITY]; /* items in the rucksack*/
} rucksack_t;

int compare_compartments(rucksack_t* rucksack);
int determine_priority(char item);
int find_common_badge(rucksack_t rucksacks[]);
int compare_rucksacks(const void* a, const void* b);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 3: Rucksack Reorganization\n\n");

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

    int priorities_sum_p1 = 0;
    int priorities_sum_p2 = 0;
    int rucksack_id = 0;
    int i = 0;
    rucksack_t rucksacks[3] = { 0 };
    while (fgets(rucksacks[2].items, RUCKSACK_CAPACITY, fp))
    {
        rucksacks[2].items_num = strlen(rucksacks[2].items);
        if (rucksacks[2].items_num > 0)
        {
            rucksacks[2].items_num--; // omit EOL

            /* part 1 */
            if ((rucksacks[2].items_num % 2U) == 0U)
            {
                priorities_sum_p1 += compare_compartments(&rucksacks[2]);
            }
            else
            {
                fprintf(stderr, "Invalid length in line %d!", rucksack_id + 1U);
                exit(EXIT_FAILURE);
            }

            /* part 2 */
            if (i < 2)
            {
                /* store two rucksacks */
                memcpy(&rucksacks[i], &rucksacks[2], sizeof(rucksack_t));
                i++;
            }
            else if (i >= 2)
            {
                /* evaluate three rucksacks */
                priorities_sum_p2 += find_common_badge(rucksacks);
                i = 0;
            }
        }

        rucksack_id++;
    }

    printf("part 1: sum of same item priorities = %d\n", priorities_sum_p1);
    printf("part 2: sum of all badge priorities = %d\n", priorities_sum_p2);

    fclose(fp);

    return EXIT_SUCCESS;
}

int compare_compartments(rucksack_t* rucksack)
{
    /* first half / first compartment */
    for (size_t c1i = 0U; c1i < rucksack->items_num / 2U; c1i++)
    {
        /* second half / second compartment */
        for (size_t c2i = rucksack->items_num / 2U; c2i < rucksack->items_num; c2i++)
        {
            /* determine priority of first item found in both compartments */
            if (rucksack->items[c1i] == rucksack->items[c2i])
            {
                return determine_priority(rucksack->items[c1i]);
            }
        }
    }

    return 0;
}

int determine_priority(char item)
{
    /* 1 .. 26 */
    if ((item >= 'a') && (item <= 'z'))
    {
        return item - 96;
    }

    /* 27 .. 52 */
    if ((item >= 'A') && (item <= 'Z'))
    {
        return item - 38;
    }

    return 0;
}

int find_common_badge(rucksack_t rucksacks[])
{
    /* sort to have largest rucksack at lowest index */
    qsort(rucksacks, 3U, sizeof(rucksack_t), compare_rucksacks);

    for (size_t s1 = 0U; s1 < rucksacks[0].items_num; s1++)
    {
        for (size_t s2 = 0U; s2 < rucksacks[1].items_num; s2++)
        {
            /* determine shared items between the first two rucksacks */
            if (rucksacks[0].items[s1] == rucksacks[1].items[s2])
            {
                /* check if shared item is also in third rucksack */
                for (size_t s3 = 0U; s3 < rucksacks[2].items_num; s3++)
                {
                    if (rucksacks[1].items[s2] == rucksacks[2].items[s3])
                    {
                        /* determine priority of first item found in all three
                         * rucksacks:
                         */
                        return determine_priority(rucksacks[2].items[s3]);
                    }
                }
            }
        }

    }

    return 0;
}

int compare_rucksacks(const void* a, const void* b)
{
    const rucksack_t* arg1 = (const rucksack_t*)a;
    const rucksack_t* arg2 = (const rucksack_t*)b;

    if (arg1->items_num < arg2->items_num)
        return 1;

    if (arg1->items_num > arg2->items_num)
        return -1;

    return 0;
}

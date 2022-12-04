/**
 * \file main.c
 *
 * \copyright (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 *
 * \brief Main program for advent of code 2022 day 1.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_SIZE   (32U)
#define DATA_CHUNK_SIZE (2U)

int compare_ints(const void* a, const void* b);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 1: Calorie Counting\n\n");

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

    char line_buf[MAX_LINE_SIZE];
    size_t elf = 0U;
    size_t elves_max = DATA_CHUNK_SIZE;
    int* calories_per_elf = malloc(elves_max * sizeof(int));
    if (calories_per_elf == NULL)
    {
        fprintf(stderr, "Not enough memory!");
        exit(EXIT_FAILURE);
    }

    calories_per_elf[elf] = 0;
    int line = 1;
    while (fgets(line_buf, MAX_LINE_SIZE, fp))
    {
        if (line_buf[0] == '\n')
        {
            elf++;
            if (elf >= elves_max)
            {
                elves_max += DATA_CHUNK_SIZE;
                calories_per_elf = realloc(calories_per_elf, elves_max * sizeof(int));
                if (NULL == calories_per_elf)
                {
                    fprintf(stderr, "Not enough memory!");
                    exit(EXIT_FAILURE);
                }
            }
            calories_per_elf[elf] = 0;
        }
        else
        {
            int calories = 0;
            if (sscanf(line_buf, "%d", &calories) == 1)
            {
                calories_per_elf[elf] += calories;
            }
            else
            {
                fprintf(stderr, "Unexpected data in line %d!", line);
                exit(EXIT_FAILURE);
            }
        }
        line++;
    }

    fclose(fp);

    size_t elves = elf + 1U;
    printf("Entries of %lld elves found.\n", elves);

    /* part 1 */
    int calories_max = calories_per_elf[0];
    for (size_t elf = 1U; elf < elves; elf++)
    {
        if (calories_max < calories_per_elf[elf])
        {
            calories_max = calories_per_elf[elf];
        }
    }
    printf("Most calories carried by an elf: %d\n", calories_max);

    /* part 2 */
    qsort(calories_per_elf, elves, sizeof(int), compare_ints);
    int calories_top_three = 0;
    for (size_t elf = 1U; elf <= 3U; elf++)
    {
        calories_top_three += calories_per_elf[elves-elf];
    }
    printf("Calories carried by top-three elves: %d\n", calories_top_three);

    return EXIT_SUCCESS;
}

int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;

    if (arg1 < arg2)
        return -1;

    if (arg1 > arg2)
        return 1;

    return 0;
}

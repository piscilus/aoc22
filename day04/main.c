/**
 * \file main.c
 *
 * \copyright (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 *
 * \brief Main program for advent of code 2022 day 4.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE (64U)

enum
{
    ELF_FIRST = 0,
    ELF_SECOND,
    NUM_ELVES_
};

typedef struct
{
    int min;
    int max;
} range_t;

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 4: Camp Cleanup\n\n");

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
    int line = 0;
    int count_contained = 0;
    int count_overlap = 0;
    while (fgets(line_buf, MAX_LINE_SIZE, fp))
    {
        range_t ranges[NUM_ELVES_] = { 0 };
        if (sscanf(line_buf, "%d-%d,%d-%d",
                &ranges[ELF_FIRST].min,
                &ranges[ELF_FIRST].max,
                &ranges[ELF_SECOND].min,
                &ranges[ELF_SECOND].max) == 4)
        {
            /* part 1 */

            /* check if first range is within second range */
            if (   (ranges[ELF_FIRST].min >= ranges[ELF_SECOND].min)
                && (ranges[ELF_FIRST].max <= ranges[ELF_SECOND].max))
            {
                count_contained++;
            }
            /* check if second range is within first range */
            else if (   (ranges[ELF_SECOND].min >= ranges[ELF_FIRST].min)
                     && (ranges[ELF_SECOND].max <= ranges[ELF_FIRST].max))
            {
                count_contained++;
            }

            /* part 2 */

            /* check if first range has overlap with second range */
            if (   (   (ranges[ELF_FIRST].min >= ranges[ELF_SECOND].min)
                    && (ranges[ELF_FIRST].min <= ranges[ELF_SECOND].max))
                || (   (ranges[ELF_FIRST].max >= ranges[ELF_SECOND].min)
                    && (ranges[ELF_FIRST].max <= ranges[ELF_SECOND].max)))
            {
                count_overlap++;
            }
            /* check if second range has overlap with first range */
            else if (   (   (ranges[ELF_SECOND].min >= ranges[ELF_FIRST].min)
                         && (ranges[ELF_SECOND].min <= ranges[ELF_FIRST].max))
                     || (   (ranges[ELF_SECOND].max >= ranges[ELF_FIRST].min)
                         && (ranges[ELF_SECOND].max <= ranges[ELF_FIRST].max)))
            {
                count_overlap++;
            }
        }
        else
        {
            fprintf(stderr, "Unexpected data in line %d!", line);
            exit(EXIT_FAILURE);
        }
        line++;
    }
    printf("part 1: number of ranges contained by another one = %d\n", count_contained);
    printf("part 2: number of ranges with overlap = %d\n", count_overlap);

    fclose(fp);

    return EXIT_SUCCESS;
}

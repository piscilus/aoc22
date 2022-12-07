/*
 * Main program for advent of code 2022 day 7.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "compare.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE (128) /* max line length of input file, incl. EOL */
#define MAX_NUM_DIRS  (500)
#define TOTAL_DISK_SPACE (70000000)
#define MIN_FREE_SPACE   (30000000)

static int dir_sizes[MAX_NUM_DIRS] = {0};
static int depth = 0;

static int scan_dirs_recursively(FILE *fp);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 7: No Space Left On Device\n\n");

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

    int dir_size_total = 0;
    dir_size_total = scan_dirs_recursively(fp);

    fclose(fp);

    /* part 1 */
    int part1 = 0;
    for (int i = 0; i < MAX_NUM_DIRS; i++)
    {
        if (dir_sizes[i] <= 100000)
        {
            part1 += dir_sizes[i];
        }
    }
    printf("part 1: total size of all directories not larger than 100000 = %d", part1);

    /* part 2*/
    int free_space = TOTAL_DISK_SPACE - dir_size_total;
    qsort(dir_sizes, MAX_NUM_DIRS, sizeof(int), compare_ints);
    for (int i = 0; i < MAX_NUM_DIRS; i++)
    {
        if (free_space + dir_sizes[i] >= MIN_FREE_SPACE)
        {
            printf("part 2: smallest directory to delete to get enough free space = %d", dir_sizes[i]);
            break;
        }
    }

    return EXIT_SUCCESS;
}

static int scan_dirs_recursively(FILE *fp)
{
    char line[MAX_LINE_SIZE];
    int pos = depth++;

    assert(pos < MAX_NUM_DIRS);

    while (fgets(line, MAX_LINE_SIZE, fp))
    {
        if (line[0] == '$') /* identify command */
        {
            size_t cmdlen = strlen(line);
            assert(cmdlen >= 3U);
            if (line[2] == 'c') /* identify cd */
            {
                assert(cmdlen >= 6U);
                if (line[5] == '.') /* change up */
                {
                    break;
                }
                else /* enter dir */
                {
                    /* accumulate all directory sizes */
                    dir_sizes[pos] += scan_dirs_recursively(fp);
                }
            }
            /* ignore ls */
        }
        else if ((line[0] >= '0') && (line[0] <= '9'))
        {
            int dir_size = 0;
            if (sscanf(line, "%d", &dir_size) == 1)
                dir_sizes[pos] += dir_size;
        }
        /* ignore dir */
    }

    return dir_sizes[pos];
}

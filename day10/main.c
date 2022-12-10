/*
 * Main program for advent of code 2022 day 10.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

/* spaghetti (>_<) */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_LINE_SIZE  (16)
#define CHECK_INTERVAL (40U)
#define CRT_X          (40U)
#define CRT_Y          (6U)

typedef enum
{
    READ_INSTRUCTION,
    PROCESS_NOOP,
    PROCESS_ADDX1,
    PROCESS_ADDX2
} state_t;

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 10: Cathode-Ray Tube\n\n");

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

    char line_buf[MAX_LINE_SIZE] = {0};

    unsigned int cycles = 1U;
    unsigned int check = 20U;
    unsigned int result = 0;
    int register_x = 1;
    int num = 0;

    char crt[CRT_Y][CRT_X];

    /* init CRT */
    for (unsigned int y = 0U; y < CRT_Y; y++)
        for (unsigned int x = 0U; x < CRT_X; x++)
            crt[y][x] = '-';

    state_t state = READ_INSTRUCTION;

    struct
    {
        unsigned int x;
        unsigned int y;
    } crt_pos = {0};

    int running = 1;
    while (running)
    {
        if (crt_pos.x >= CRT_X)
        {
            crt_pos.x = 0U;
            crt_pos.y++;
        }
        if (((register_x - 1) <= (int)crt_pos.x) && ((register_x + 1) >= (int)crt_pos.x))
            crt[crt_pos.y % CRT_Y][crt_pos.x] = '#';
        else
            crt[crt_pos.y % CRT_Y][crt_pos.x] = '.';

        switch (state)
        {
            case READ_INSTRUCTION:
                if (fgets(line_buf, MAX_LINE_SIZE, fp))
                {
                    if (line_buf[0] == 'n') /* noop */
                    {
                        state = PROCESS_NOOP;
                    }
                    else if (sscanf(line_buf, "addx %d", &num) == 1)
                    {
                        state = PROCESS_ADDX1;
                    }
                    else
                    {
                        assert(0);
                    }
                }
                else
                {
                    running = 0;
                }
                break;
            case PROCESS_NOOP:
                state = READ_INSTRUCTION;
                cycles++;
                crt_pos.x++;
                break;
            case PROCESS_ADDX1:
                state = PROCESS_ADDX2;
                cycles++;
                crt_pos.x++;
                break;
            case PROCESS_ADDX2:
                register_x += num;
                state = READ_INSTRUCTION;
                cycles++;
                crt_pos.x++;
                break;
            default:
                assert(0);
        }
        if (cycles == check)
        {
            check += CHECK_INTERVAL;
            result += (cycles * register_x);
        }
    }

    printf("Part 1: sum of all signal strengths = %u\n", result);

    printf("Part 2:\n");
    for (unsigned int y = 0U; y < CRT_Y; y++)
    {
        for (unsigned int x = 0U; x < CRT_X; x++)
            printf("%c", crt[y][x]);
        printf("\n");
    }
    // ZFBFHGUP

    fclose(fp);

    return EXIT_SUCCESS;
}

/*
 * Main program for advent of code 2022 day 5.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE   (64U)  /* max line length of input file */
#define MAX_STACK_SIZE  (100U) /* max size of a stack */
#define NUM_STACKS      (9U)   /* (max) number of stacks*/

typedef enum
{
    READ_STACKS,
    MOVE_DATA
} state_t;

typedef struct
{
    size_t size;
    char data[MAX_STACK_SIZE];
} stack_t;

typedef struct
{
    int from;
    int to;
    int num;
} move_t;

void print_stacks(const stack_t stacks[]);
void reverse(char s[]);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 5: Supply Stacks\n\n");

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

    int line = 0;
    char line_buf[MAX_LINE_SIZE];
    stack_t stacks_p1[NUM_STACKS] = {0};
    stack_t stacks_p2[NUM_STACKS] = {0};
    state_t state = READ_STACKS;
    while (fgets(line_buf, MAX_LINE_SIZE, fp))
    {
        /* use empty line to switch from initial stack to move operations */
        if (line_buf[0] == '\n')
        {
            state = MOVE_DATA;
            /* reverse stack data to have top most element as last char in string */
            for (size_t s = 0U; s < NUM_STACKS; s++)
            {
                if (stacks_p1[s].size > 0)
                    reverse(stacks_p1[s].data);
            }
            printf("initial stacks:\n");
            print_stacks(stacks_p1);
            memcpy(stacks_p2, stacks_p1, sizeof(stacks_p2));
            continue; /* skip processing of empty line */
        }
        switch (state)
        {
            case READ_STACKS:
                {
                    int n = 0;
                    size_t i = 1U;
                    size_t max = strlen(line_buf);
                    while (i < max)
                    {
                        if (line_buf[i] >= 'A' && line_buf[i] <= 'Z')
                        {
                            if (stacks_p1[n].size < MAX_STACK_SIZE)
                            {
                                stacks_p1[n].data[stacks_p1[n].size] = line_buf[i];
                                stacks_p1[n].size++;
                            }
                            else
                            {
                                fprintf(stderr, "Too much data, increase max size of stack!");
                                exit(EXIT_FAILURE);
                            }
                        }
                        n++;
                        i += 4;
                    }
                }
                break;
            case MOVE_DATA:
                {
                    move_t move = {0};
                    if (sscanf(line_buf, "move %d from %d to %d", &move.num, &move.from, &move.to) == 3)
                    {
                        /* TODO: sanity checks to avoid invalid array indexes */
                        move.from--;
                        move.to--;
                        for (int i = 0; i < move.num; i++)
                        {
                            stacks_p1[move.from].size--;
                            stacks_p1[move.to].data[stacks_p1[move.to].size] = stacks_p1[move.from].data[stacks_p1[move.from].size];
                            stacks_p1[move.to].size++;
                        }
                        stacks_p2[move.from].size -= move.num;
                        memcpy(&stacks_p2[move.to].data[stacks_p2[move.to].size], &stacks_p2[move.from].data[stacks_p2[move.from].size], move.num);
                        stacks_p2[move.to].size += move.num;
                    }
                    else
                    {
                        fprintf(stderr, "Unexpected data in line %d!", line);
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            default:
                fprintf(stderr, "Unexpected state");
                exit(EXIT_FAILURE);
        }
        line++;
    }

    printf("\nPart 1: Final string = ");
    for (size_t s = 0U; s < NUM_STACKS; s++)
    {
        printf("%c", stacks_p1[s].data[stacks_p1[s].size-1]);
    }
    printf("\nPart 2: Final string = ");
    for (size_t s = 0U; s < NUM_STACKS; s++)
    {
        printf("%c", stacks_p2[s].data[stacks_p2[s].size-1]);
    }
    printf("\n");

    fclose(fp);

    return EXIT_SUCCESS;
}

void print_stacks(const stack_t stacks[])
{
    for (size_t s = 0U; s < NUM_STACKS; s++)
    {
        printf("#%lld: [%lld] %s\n", s, stacks[s].size, stacks[s].data);
    }
}

void reverse(char s[])
{
    size_t length = strlen(s);
    char c;
    size_t i, j;

    for (i = 0U, j = length - 1U; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/*
 * Main program for advent of code 2022 day 11.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

/*
 * WIP
 * part 1 solved but currently not working due to preparations for part 2!
 */

#include "queue.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_LINE_SIZE  (64)
#define MAX_MONKEY     (20)
#define ROUNDS         (10000)

typedef enum
{
    ADDITION,
    MULTIPLICATION
} operation_t;

typedef enum
{
    SELF,
    VALUE
} operand_select;

typedef struct
{
    queue_t* items;
    uint64_t divisor;
    int next_monkey_true;
    int next_monkey_false;
    operation_t operation;
    operand_select opselect;
    uint64_t operand;
    int count;
} monkey_t;

int compare_monkeys(const void* a, const void* b);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 11: Monkey in the Middle\n\n");

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
    int num_monkey = 0;
    monkey_t monkeys[MAX_MONKEY] = {0};
    monkeys[num_monkey].items = queue_init(sizeof(int64_t));
    assert(monkeys[num_monkey].items != NULL);
    while (fgets(line_buf, MAX_LINE_SIZE, fp) != NULL)
    {
        assert(num_monkey < MAX_MONKEY);
        if (line_buf[0] == '\n')
        {
            num_monkey++;
            monkeys[num_monkey].items = queue_init(sizeof(int64_t));
            assert(monkeys[num_monkey].items != NULL);
        }
        else if (line_buf[2] == 'O')
        {
            assert(strlen(line_buf) >= 26U);
            /*           10        20
             * 012345678901234567890123456
             *   Operation: new = old * o/digit
             *   ^                    ^ ^
             *              operator -´ `- old or digit
             */
            switch (line_buf[23])
            {
                case '+':
                    monkeys[num_monkey].operation = ADDITION;
                    break;
                case '*':
                    monkeys[num_monkey].operation = MULTIPLICATION;
                    break;
                default:
                    assert(0);
            }
            if (line_buf[25] == 'o') /* old */
            {
                monkeys[num_monkey].opselect = SELF;
            }
            else
            {
                assert(isdigit(line_buf[25]));
                monkeys[num_monkey].opselect = VALUE;
                long v = atol(&line_buf[25]);
                assert(v >= 0);
                monkeys[num_monkey].operand = (uint64_t) v;
            }
        }
        else if (line_buf[2] == 'S')
        {
            char* p = line_buf;
            while (*p)
            {
                if (   isdigit(*p)
                    || ((*p == '-') && isdigit(*(p+1))) )
                {
                    errno = 0;
                    long v = strtol(p, &p, 10);
                    assert(errno == 0);
                    int r = queue_enqueue(monkeys[num_monkey].items, &v);
                    assert(r);
                }
                else
                    p++;
            }
        }
        else
        {
            /* simple try to parse the other stuff */
            sscanf(line_buf, "  Test: divisible by %llu", &monkeys[num_monkey].divisor);
            sscanf(line_buf, "    If true: throw to monkey %d", &monkeys[num_monkey].next_monkey_true);
            sscanf(line_buf, "    If false: throw to monkey %d", &monkeys[num_monkey].next_monkey_false);
        }
    }
    num_monkey++;

    for (int round = 0; round < ROUNDS; round++)
    {
        for (int m = 0; m < num_monkey; m++)
        {
            uint64_t worrylvl;
            while( queue_dequeue(monkeys[m].items, &worrylvl) )
            {
                uint64_t operand;
                monkeys[m].count++;
                switch (monkeys[m].opselect)
                {
                    case SELF:
                        operand = worrylvl;
                        break;
                    case VALUE:
                        operand = monkeys[m].operand;
                        break;
                    default:
                        assert(0);
                }
                // uint64_t before = worrylvl;
                switch(monkeys[m].operation)
                {
                    case ADDITION:
                        worrylvl += operand;
                        break;
                    case MULTIPLICATION:
                        worrylvl *= operand;
                        break;
                    default:
                        assert(0);
                }
                // if ( worrylvl < before)
                //     fprintf(stderr, "%llu %llu %llu\n", before, operand, worrylvl);

                worrylvl /= 3UL;
                if ((worrylvl % monkeys[m].divisor) == 0UL)
                {
                    assert(monkeys[m].next_monkey_true < num_monkey);
                    int r = queue_enqueue(monkeys[monkeys[m].next_monkey_true].items, &worrylvl);
                    assert(r);
                }
                else
                {
                    assert(monkeys[m].next_monkey_false < num_monkey);
                    int r = queue_enqueue(monkeys[monkeys[m].next_monkey_false].items, &worrylvl);
                    assert(r);
                }
            }
        }
    }

    /* note: monkeys cannot be identified after sort */
    qsort(monkeys, num_monkey, sizeof(monkey_t), compare_monkeys);
    for (int i = 0; i < num_monkey; i++)
    {
        printf("%d\n", monkeys[i].count);
        queue_destroy(monkeys[i].items);
    }

    printf("Part 1: %d\n", monkeys[num_monkey-1].count * monkeys[num_monkey-2].count);

    fclose(fp);

    return EXIT_SUCCESS;
}

int compare_monkeys(const void* a, const void* b)
{
    monkey_t arg1 = *(const monkey_t*)a;
    monkey_t arg2 = *(const monkey_t*)b;

    if (arg1.count < arg2.count)
        return -1;

    if (arg1.count > arg2.count)
        return 1;

    return 0;
}



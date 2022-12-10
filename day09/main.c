/*
 * Main program for advent of code 2022 day 9.
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

#define MAX_LINE_SIZE (16)

typedef struct
{
    int32_t x;
    int32_t y;
} pos_t;

typedef union
{
    pos_t pos;
    int64_t code;
} visited_t;

typedef struct
{
    visited_t *positions;
    size_t used;
    size_t size;
} track_t;

int compare_ints(const void* a, const void* b);

void initTrack(track_t* t, size_t size);
void addPosition(track_t* t, visited_t pos);
void freeTrack(track_t* t);

void move_head(char dir, pos_t* pos);
int move_tail(visited_t head, visited_t* tail);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 9: Rope Bridges\n\n");

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

    track_t track_p1;
    track_t track_p2;
    initTrack(&track_p1, 100);
    initTrack(&track_p2, 100);

    visited_t head = {0};
    visited_t tail_p1 = {0};
    visited_t tail_p2[9] = {0};

    addPosition(&track_p1, tail_p1);
    addPosition(&track_p2, tail_p2[0]);
    while (fgets(line_buf, MAX_LINE_SIZE, fp))
    {
        char dir = 'x';
        int steps = 0;
        if (sscanf(line_buf, "%c %d", &dir, &steps) == 2)
        {
            assert(steps >= 0);
            while (steps > 0)
            {
                /* part 1 */
                move_head(dir, &head.pos);
                if (move_tail(head, &tail_p1))
                    addPosition(&track_p1, tail_p1);
                /* part 2 */
                move_tail(head, &tail_p2[0]);
                for(int n = 1; n < 8; n++)
                    move_tail(tail_p2[n - 1], &tail_p2[n]);
                if (move_tail(tail_p2[7], &tail_p2[8]))
                    addPosition(&track_p2, tail_p2[8]);
                steps--;
            }
        }
        else
        {
            assert(0);
        }
    }

    qsort(track_p1.positions, track_p1.used, sizeof(visited_t), compare_ints);
    qsort(track_p2.positions, track_p2.used, sizeof(visited_t), compare_ints);

    int unique = 1;
    for (size_t i = 1U; i < track_p1.used; i++)
        if (track_p1.positions[i].code > track_p1.positions[i - 1U].code)
            unique++;
    printf("Part 1: Unique positions of tail = %d\n", unique);

    unique = 1;
    for (size_t i = 1U; i < track_p2.used; i++)
        if (track_p2.positions[i].code > track_p2.positions[i - 1U].code)
            unique++;
    printf("Part 2: Unique positions of tail = %d\n", unique);

    fclose(fp);
    freeTrack(&track_p1);
    freeTrack(&track_p2);

    return EXIT_SUCCESS;
}

int compare_ints(const void* a, const void* b)
{
    int64_t arg1 = *(const int64_t*)a;
    int64_t arg2 = *(const int64_t*)b;

    if (arg1 < arg2)
        return -1;

    if (arg1 > arg2)
        return 1;

    return 0;
}

void initTrack(track_t* t, size_t size)
{
    t->positions = malloc(size * sizeof(visited_t));
    assert(t->positions != NULL);
    t->used = 0U;
    t->size = size;
}

void addPosition(track_t* t, visited_t pos)
{
    if (t->used >= t->size)
    {
        t->size *= 2U;
        t->positions = realloc(t->positions, t->size * sizeof(visited_t));
        assert(t->positions != NULL);
    }
    t->positions[t->used++] = pos;
}

void freeTrack(track_t* t)
{
    free(t->positions);
    t->positions = NULL;
    t->used = 0U;
    t->size = 0U;
}

void move_head(char dir, pos_t* pos)
{
    switch (dir)
    {
        case 'U':
            pos->y--;
            break;
        case 'R':
            pos->x++;
            break;
        case 'D':
            pos->y++;
            break;
        case 'L':
            pos->x--;
            break;
        default:
            assert(0);
    }
}

int move_tail(visited_t head, visited_t* tail)
{
    if (tail->pos.y - head.pos.y > 1) /* up */
    {
        tail->pos.y--;
        if ((tail->pos.x - head.pos.x) > 1) /* diag left */
            tail->pos.x--;
        else if ((head.pos.x - tail->pos.x) > 1) /* diag right */
            tail->pos.x++;
        else
            tail->pos.x = head.pos.x;
    }
    else if (head.pos.y - tail->pos.y > 1) /* down */
    {
        tail->pos.y++;
        if ((tail->pos.x - head.pos.x) > 1) /* diag left */
            tail->pos.x--;
        else if ((head.pos.x - tail->pos.x) > 1) /* diag right */
            tail->pos.x++;
        else
            tail->pos.x = head.pos.x;
    }
    else if (tail->pos.x - head.pos.x > 1) /* left */
    {
        tail->pos.x--;
        if ((tail->pos.y - head.pos.y) > 1) /* diag up */
            tail->pos.y--;
        else if ((head.pos.y - tail->pos.y) > 1) /* diag down */
            tail->pos.y++;
        else
            tail->pos.y = head.pos.y;
    }
    else if (head.pos.x - tail->pos.x > 1) /* right */
    {
        tail->pos.x++;
        if ((tail->pos.y - head.pos.y) > 1) /* diag up */
            tail->pos.y--;
        else if ((head.pos.y - tail->pos.y) > 1) /* diag down */
            tail->pos.y++;
        else
            tail->pos.y = head.pos.y;
    }
    else
    {
        return 0; /* no movement */
    }

    return 1; /* moved */
}

/*
 * Main program for advent of code 2022 day 8.
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

#define MAX_XY (200)

static int data[MAX_XY][MAX_XY] = {0};

int visible_from_left(int y, int x);
int visible_from_right(int y, int x, int max_x);
int visible_from_top(int y, int x);
int visible_from_bottom(int y, int x, int max_y);
int view_to_top(int y, int x);
int view_to_right(int y, int x, int max_x);
int view_to_bottom(int y, int x, int max_y);
int view_to_left(int y, int x);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 8: Treetop Tree House\n\n");

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

    int i = 0;
    struct
    {
        int x;
        int y;
    } pos = {0};
    int max_x = 0;
    while ((i =fgetc(fp)) != EOF)
    {
        if (i == '\n')
        {
            max_x = pos.x; /* simply assume same length of all lines*/
            pos.x = 0;
            pos.y++;
        }
        else
        {
            assert(pos.y < MAX_XY);
            assert(pos.x < MAX_XY);
            data[pos.y][pos.x] = i - '0';
            pos.x++;
        }
    }

    int max_y = pos.y;
    int visible_trees = 0;
    int view = 0;

    /* iterate over inner trees */
    for (int y = 1; y < (max_y - 1); y++)
    {
        for (int x = 1; x < (max_x - 1); x++)
        {
            int v = 0;
            if (   visible_from_left(y, x)
                || visible_from_right(y, x, (max_x - 1))
                || visible_from_top(y, x)
                || visible_from_bottom(y, x, (max_y - 1)))
                visible_trees++;
            v = view_to_top(y, x);
            v *= view_to_right(y, x, max_x);
            v *= view_to_bottom(y, x, max_y);
            v *= view_to_left(y, x);
            if (v > view)
                view = v;
        }
    }

    visible_trees += (2 * max_x) + (2 * max_y) - 4; /* + outer */

    printf("part 1: number of visible trees from outside = %d\n", visible_trees);
    printf("part 2: highest scenic score = %d\n", view);

    fclose(fp);

    return EXIT_SUCCESS;
}

int visible_from_left(int y, int x)
{
    for (int i = 0; i < x; i++)
        if (data[y][i] >= data[y][x])
            return 0;

    return 1;
}

int visible_from_right(int y, int x, int max_x)
{
    for (int i = max_x; i > x; i--)
        if (data[y][i] >= data[y][x])
            return 0;
    return 1;
}

int visible_from_top(int y, int x)
{
    for (int i = 0; i < y; i++)
        if (data[i][x] >= data[y][x])
            return 0;
    return 1;
}

int visible_from_bottom(int y, int x, int max_y)
{
    for (int i = max_y; i > y; i--)
        if (data[i][x] >= data[y][x])
            return 0;
    return 1;
}

int view_to_top(int y, int x)
{
    int v = 0;

    for (int i = y - 1; i >= 0; i--)
    {
        if (data[i][x] < data[y][x])
        {
            v++;
        }
        else if (data[i][x] >= data[y][x])
        {
            v++;
            break;
        }
        else
        {
            break;
        }
    }

    return v;
}

int view_to_right(int y, int x, int max_x)
{
    int v = 0;

    for (int i = x + 1; i < max_x; i++)
    {
        if (data[y][i] < data[y][x])
        {
            v++;
        }
        else if (data[y][i] >= data[y][x])
        {
            v++;
            break;
        }
        else
        {
            break;
        }
    }

    return v;
}

int view_to_bottom(int y, int x, int max_y)
{
    int v = 0;

    for (int i = y + 1; i < max_y; i++)
    {
        if (data[i][x] < data[y][x])
        {
            v++;
        }
        else if (data[i][x] >= data[y][x])
        {
            v++;
            break;
        }
        else
        {
            break;
        }
    }

    return v;
}

int view_to_left(int y, int x)
{
    int v = 0;

    for (int i = x - 1; i >= 0; i--)
    {
        if (data[y][i] < data[y][x])
        {
            v++;
        }
        else if (data[y][i] >= data[y][x])
        {
            v++;
            break;
        }
        else
        {
            break;
        }
    }

    return v;
}

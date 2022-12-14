/*
 * Main program for advent of code 2022 day 14.
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

#define MAX_LINE_SIZE (1024U)
#define GRID_MAX_X    (200U)
#define GRID_MAX_Y    (200U)
#define GRID_MID_X    (500U)
#define GRID_CENTER_X (GRID_MAX_X / 2U)
#define GRID_SUB_X    (GRID_MID_X - GRID_CENTER_X)

typedef struct
{
    size_t x;
    size_t y;
} coords_t;

typedef struct
{
    char grid[GRID_MAX_Y][GRID_MAX_X];
    coords_t min;
    coords_t max;
} grid_t;

static const coords_t sand_source = {.x = 500U, .y = 0U};

size_t grid_normalize_x(size_t x);

void grid_init(grid_t* grid, coords_t source);
int grid_create();

void grid_print(const grid_t* grid);

void grid_draw_line(grid_t* grid, const coords_t* from, const coords_t* to);

int grid_drop_sand(grid_t* grid);
int grid_move_sand(grid_t* grid);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 14: Regolith Reservoir\n\n");

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

    grid_t grid = {0};
    grid_init(&grid, sand_source);

    if (!grid_create(&grid, fp))
    {
        fprintf(stderr, "Could not create grid from file!");
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    /* adjust grid size by creating additional columns left and right and a
     * row at the bottom -> sand that enters this area falls down and stops the
     * program
     */
    assert(grid.min.x > 0);
    grid.min.x--;
    grid.max.x += 2U;
    grid.max.y += 2U;

    int running = 1;
    int sand = 0;
    while (running > 0)
    {
        running = grid_drop_sand(&grid);
        if (running >= 0)
        {
            sand++;
            running = grid_move_sand(&grid);
        }
        else
        {
            printf("Cannot drop more sand, source blocked!\n");
        }
    }

    printf("Print final grid:\n");
    grid_print(&grid);
    assert(sand > 0);
    printf("Part 1: amount of sand before it flows into the abyss = %d\n", sand - 1);

    return EXIT_SUCCESS;
}

size_t grid_normalize_x(size_t x)
{
    assert(x >= GRID_SUB_X);
    size_t x_n = x - GRID_SUB_X;
    assert(x_n < GRID_MAX_X);
    return x_n;
}

void grid_init(grid_t* grid, coords_t source)
{
    assert(grid != NULL);
    assert(source.y < GRID_MAX_Y);

    for (size_t y = 0U; y < GRID_MAX_Y; y++)
        for (size_t x = 0U; x < GRID_MAX_X; x++)
            grid->grid[y][x] = '.';

    grid->max.x = 0U;
    grid->max.y = 0U;
    grid->min.x = GRID_MAX_X;
    grid->min.y = 0U;

    grid->grid[source.y][grid_normalize_x(source.x)] = '+';
}

int grid_create(grid_t* grid, FILE* fp)
{
    char line_buf[MAX_LINE_SIZE] = {0};
    while (fgets(line_buf, MAX_LINE_SIZE, fp) != NULL)
    {
        size_t s = strlen(line_buf);
        assert(s > 0U);
        if (line_buf[s - 1] == '\n')
            line_buf[s - 1] = 0;
        char *token = strtok(line_buf, " -> ");
        int f = 0;
        coords_t point_prev;
        while (token)
        {
            coords_t point_now = {0};
            if (sscanf(token, "%llu,%llu", &point_now.x, &point_now.y) != 2)
                return 0;
            point_now.x = grid_normalize_x(point_now.x);
            if (point_now.x > grid->max.x)
                grid->max.x = point_now.x;
            if (point_now.y > grid->max.y)
                grid->max.y = point_now.y;
            if (point_now.x < grid->min.x)
                grid->min.x = point_now.x;
            if (f > 0)
                grid_draw_line(grid, &point_prev, &point_now);
            f++;
            point_prev = point_now;
            token = strtok(NULL, " -> ");
        }
    }

    return 1;
}

void grid_print(const grid_t* grid)
{
    assert((grid->max.x - grid->min.x) >= 6U); /* min length for x axis legend */

    printf("    %03llu", grid->min.x);
    for (size_t n = 0U; n < (grid->max.x - grid->min.x - 6U); n++ )
        putchar(' ');
    printf("%03llu\n", grid->max.x - 1U);
    for (size_t y = grid->min.y; y < grid->max.y; y++)
    {
        if ((y == grid->min.y) || ((y + 1U) >= grid->max.y))
            printf("%03llu ", y);
        else
            printf("    ");
        for (size_t x = grid->min.x; x < grid->max.x; x++)
            printf("%c", grid->grid[y][x]);
        printf("\n");
    }
}

void grid_draw_line(grid_t* grid, const coords_t* from, const coords_t* to)
{
    if (from->x == to->x) /* vertical line */
    {
        if (from->y <= to->y) /* draw down */
            for (size_t y = from->y; y <= to->y; y++)
                grid->grid[y][from->x] = '#';
        else /* draw up */
            for (size_t y = to->y; y <= from->y; y++)
                grid->grid[y][from->x] = '#';
    }
    else if (from->y == to->y) /* horizontal line */
    {
        if (from->x <= to->x) /* draw right */
            for (size_t x = from->x; x <= to->x; x++)
                grid->grid[from->y][x] = '#';
        else /* draw left */
            for (size_t x = to->x; x <= from->x; x++)
                grid->grid[from->y][x] = '#';
    }
    else
        assert(0); /* not a vertical or horizontal line */
}

int grid_drop_sand(grid_t* grid)
{
    if (grid->grid[1][GRID_CENTER_X] == 'o')
    {
        return -1; /* source blocked */
    }
    else
    {
        for (size_t y = 1U; y < grid->max.y; y++)
        {
            if (grid->grid[y][GRID_CENTER_X] != '.')
            {
                grid->grid[y - 1U][GRID_CENTER_X] = 'o';
                return 1;
            }
        }
    }

    return 0;
}

int grid_move_sand(grid_t* grid)
{
    int sand_moving = 1;
    while (sand_moving) /* could result in an infinite loop if there is a bug or false input :-) */
    {
        sand_moving = 0;
        for (size_t y = grid->min.y; y < grid->max.y; y++)
        {
            for (size_t x = grid->min.x; x < grid->max.x; x++)
            {
                /* looking for sand */
                if (grid->grid[y][x] == 'o')
                {
                    /* check if space below sand is free (highest priority)*/
                    if ((grid->grid[y + 1U][x] == '.'))
                    {
                        /* scan the column below */
                        for (size_t yy = (y + 1U); yy < grid->max.y; yy++)
                        {
                            if (grid->grid[yy][x] == '.')
                            {
                                /* free, check if this is the very last row */
                                if ((yy + 1U) >= (grid->max.y - 1U))
                                {
                                    /* sand will fall into abyss*/
                                    return 0;
                                }
                                /* else look further down */
                            }
                            else
                            {
                                /* reach an obstacle, drop the sand on top */
                                grid->grid[y][x] = '.';
                                grid->grid[yy - 1U][x] = 'o';
                                sand_moving = 1;
                                break;
                            }
                        }
                    }
                    /* check if the space diagonal down left is free (2nd
                     * highest priority)
                     */
                    else if ((x > 0) && (grid->grid[y + 1U][x - 1U] == '.'))
                    {
                        grid->grid[y][x] = '.';
                        grid->grid[y + 1U][x - 1U] = 'o';
                        sand_moving = 1;
                    }
                    /* check if the space diagonal down right is free (lowest
                     * priority)
                     */
                    else if (((x + 1U) < grid->max.x) && (grid->grid[y + 1U][x + 1U] == '.'))
                    {
                        grid->grid[y][x] = '.';
                        grid->grid[y + 1U][x + 1U] = 'o';
                        sand_moving = 1;
                    }
                }
            }
        }
    }

    return 1;
}
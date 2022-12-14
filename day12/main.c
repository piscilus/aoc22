/*
 * Main program for advent of code 2022 day 12.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "compare.h"
#include "queue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAP_SIZE (200U)

typedef struct
{
    size_t x;
    size_t y;
} coords_t;

typedef struct
{
    coords_t pos;
    int distance;
} path_t;

typedef struct
{
    int heightmap[MAP_SIZE][MAP_SIZE];
    int visited[MAP_SIZE][MAP_SIZE];
    coords_t size;
    coords_t start;
    coords_t end;
} grid_t;

static void parse_input(FILE* fp, grid_t* grid);

static void prepare_grid(grid_t* grid);

static void print_grid_map(const grid_t* grid);
static void print_grid_visited(const grid_t* grid);

static int bfs(grid_t* grid);

static int try_move(queue_t* q, grid_t* grid, const path_t* p, path_t* next);

static int check_up(queue_t* q, grid_t* grid, const path_t* p);
static int check_right(queue_t* q, grid_t* grid, const path_t* p);
static int check_down(queue_t* q, grid_t* grid, const path_t* p);
static int check_left(queue_t* q, grid_t* grid, const path_t* p);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 12: Hill Climbing Algorithm\n\n");

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
    parse_input(fp, &grid);
    fclose(fp);

    // not recommended to print the large contest grid :-)
    // print_grid_map(&grid);
    // print_grid_visited(&grid);

    int result = bfs(&grid);

    printf("Part 1: shortest path = %d\n", result);

    size_t result_max = 100U;
    int* results = malloc(result_max * sizeof(int));
    assert(results != NULL);
    size_t num_start_pos = 0U;
    /* search for all position of lowest height and perform bfs */
    for (size_t y = 0U; y < grid.size.y; y++)
    {
        for (size_t x = 0U; x < grid.size.x; x++)
        {
            if (grid.heightmap[y][x] == 0)
            {
                grid.start.x = x;
                grid.start.y = y;
                prepare_grid(&grid);
                int r = bfs(&grid);
                if (r > 0) /* store only valid results */
                {
                    num_start_pos++;
                    if (num_start_pos >= result_max)
                    {
                        result_max += 100U;
                        results = realloc(results, result_max * sizeof(int));
                        assert(results != NULL);
                    }
                    results[num_start_pos - 1U] = r;
                }
            }
        }
    }

    qsort(results, num_start_pos, sizeof(int), compare_ints);
    printf("Part 2: shortest path = %d\n", results[0]);
    free(results);

    return EXIT_SUCCESS;
}

static void parse_input(FILE* fp, grid_t* grid)
{
    assert(fp != NULL);
    assert(grid != NULL);

    int c = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
        {
            grid->size.x = 0U;
            grid->size.y++;
        }
        else
        {
            assert(grid->size.x < MAP_SIZE);
            assert(grid->size.y < MAP_SIZE);
            switch (c)
            {
                case 'S':
                    /* assume that only a single S exists on the grid */
                    grid->start.x = grid->size.x;
                    grid->start.y = grid->size.y;
                    grid->heightmap[grid->start.y][grid->start.x] = 0;
                    break;
                case 'E':
                    /* assume that only a single E exists on the grid */
                    grid->end.x = grid->size.x;
                    grid->end.y = grid->size.y;
                    grid->heightmap[grid->end.y][grid->end.x] = 'z' - 'a';
                    break;
                default:
                    assert((c >= 'a') && (c <= 'z'));
                    grid->heightmap[grid->size.y][grid->size.x] = c - 'a';
                    break;
            }
            grid->size.x++;
        }
    }
    grid->size.y++;
}

static void prepare_grid(grid_t* grid)
{
    for (size_t y = 0U; y < grid->size.y; y++)
    {
        for (size_t x = 0U; x < grid->size.x; x++)
        {
            grid->visited[y][x] = 0;
        }
    }
}

static void print_grid_map(const grid_t* grid)
{
    assert(grid != NULL);

    printf("\nHEIGHTMAP\n");
    printf("grid size: x = %llu y = %llu\n", grid->size.x, grid->size.y);

    for (size_t y = 0U; y < grid->size.y; y++)
    {
        for (size_t x = 0U; x < grid->size.x; x++)
            printf("%02d ", grid->heightmap[y][x]);
        printf("\n");
    }
}

static void print_grid_visited(const grid_t* grid)
{
    assert(grid != NULL);

    printf("\nVISITED LOCATIONS\n");
    printf("grid size: x = %llu y = %llu\n", grid->size.x, grid->size.y);

    for (size_t y = 0U; y < grid->size.y; y++)
    {
        for (size_t x = 0U; x < grid->size.x; x++)
        {
            if ((x == grid->start.x) && (y == grid->start.y))
                printf("S ");
            else if ((x == grid->end.x) && (y == grid->end.y))
                printf("E ");
            else
                printf("%d ", grid->visited[y][x]);
        }
        printf("\n");
    }
}

/* https://en.wikipedia.org/wiki/Breadth-first_search */
static int bfs(grid_t* grid)
{
    assert(grid != NULL);

    int r;
    path_t p;
    p.distance = 0;
    p.pos = grid->start;

    queue_t* q = queue_init(sizeof(path_t));
    assert(q != NULL);

    r = queue_enqueue(q, &p);
    assert(r);
    grid->visited[p.pos.y][p.pos.x] = 1;

    while (0U < queue_count(q))
    {
        r = queue_dequeue(q, &p);
        assert(r);

        if ((p.pos.x == grid->end.x) && (p.pos.y == grid->end.y))
        {
            queue_destroy(q);
            return p.distance;
        }

        r = check_up(q, grid, &p);
        assert(r >= 0);
        r = check_right(q, grid, &p);
        assert(r >= 0);
        r = check_down(q, grid, &p);
        assert(r >= 0);
        r = check_left(q, grid, &p);
        assert(r >= 0);
    }

    queue_destroy(q);
    return 0;
}

static int try_move(queue_t* q, grid_t* grid, const path_t* p, path_t* next)
{
    /*
     * check if designated path can be entered:
     * - must not been visited before
     * - must be at most one higher than current elevation (or any lower)
     */
    if (   (grid->visited[next->pos.y][next->pos.x] == 0)
        && (grid->heightmap[next->pos.y][next->pos.x] <= (grid->heightmap[p->pos.y][p->pos.x] + 1)))
    {
        next->distance++;
        if (queue_enqueue(q, next))
        {
            grid->visited[next->pos.y][next->pos.x] = 1;
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return 0;
    }
}

static int check_up(queue_t* q, grid_t* grid, const path_t* p)
{
    path_t next;

    if (p->pos.y > 0U)
    {
        next = *p;
        next.pos.y--;
        return try_move(q, grid, p, &next);
    }
    else
    {
        return 0;
    }
}

static int check_right(queue_t* q, grid_t* grid, const path_t* p)
{
    path_t next;

    if ((p->pos.x + 1U) < grid->size.x)
    {
        next = *p;
        next.pos.x++;
        return try_move(q, grid, p, &next);
    }
    else
    {
        return 0;
    }
}

static int check_down(queue_t* q, grid_t* grid, const path_t* p)
{
    path_t next;

    if ((p->pos.y + 1U) < grid->size.y)
    {
        next = *p;
        next.pos.y++;
        return try_move(q, grid, p, &next);
    }
    else
    {
        return 0;
    }
}

static int check_left(queue_t* q, grid_t* grid, const path_t* p)
{
    path_t next;

    if (p->pos.x > 0U)
    {
        next = *p;
        next.pos.x--;
        return try_move(q, grid, p, &next);
    }
    else
    {
        return 0;
    }
}

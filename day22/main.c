/*
 * Main program for advent of code 2022 day 22.
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

#define CHUNK_SIZE (256U)

enum
{
    LEFT  = 0,
    RIGHT = 1
};

typedef enum /* number equals score for result */
{
    EAST  = 0,
    SOUTH = 1,
    WEST  = 2,
    NORTH = 3
} compass_t;

typedef struct
{
    size_t x;
    size_t y;
} coords_t;

typedef struct
{
    char     map[256][256];
    coords_t max;
} grid_t;

typedef struct
{
    coords_t  pos;
    compass_t facing;
} player_t;

static const char dir[4] =
{
    [EAST]  = 'E',
    [SOUTH] = 'S',
    [WEST]  = 'W',
    [NORTH] = 'N'
};

static const compass_t mapdir[2][4] =
{
    [LEFT] = {
        [NORTH] = WEST,
        [EAST]  = NORTH,
        [SOUTH] = EAST,
        [WEST]  = SOUTH
    },
    [RIGHT] = {
        [NORTH] = EAST,
        [EAST]  = SOUTH,
        [SOUTH] = WEST,
        [WEST]  = NORTH
    }
};

static void
map_find_start(grid_t* m, player_t* p);
static void
map_print(grid_t* m);
static void
map_file(grid_t* m);

static void
player_turn(player_t* p, char d);
static int
player_move(player_t* p, grid_t* m, int s);

static void
player_move_right(player_t* p, grid_t* m, int s);
static void
player_move_left(player_t* p, grid_t* m, int s);
static void
player_move_up(player_t* p, grid_t* m, int s);
static void
player_move_down(player_t* p, grid_t* m, int s);

static int
wrap_r_to_l(const player_t* p, const grid_t* m, size_t* new_x);
static int
wrap_l_to_r(const player_t* p, const grid_t* m, size_t* new_x);
static int
wrap_b_to_t(const player_t* p, const grid_t* m, size_t* new_y);
static int
wrap_t_to_b(const player_t* p, const grid_t* m, size_t* new_y);

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 22: Monkey Map\n\n");

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

    grid_t map;
    player_t player;

    for (size_t y = 0U; y < CHUNK_SIZE; y++)
    {
        for (size_t x = 0U; x < CHUNK_SIZE; x++)
        {
            map.map[y][x] = ' ';
        }
    }
    map.max.x = 0U;
    map.max.y = 0U;

    char line_buf[CHUNK_SIZE];
    while (fgets(line_buf, CHUNK_SIZE, fp))
    {
        if (line_buf[0] == '\n') /* consume empty line seperator*/
        {
            break;
        }
        size_t len = strlen(line_buf) -  1U;
        memcpy(map.map[map.max.y], line_buf, len);
        if (len > map.max.x)
            map.max.x = len;
        map.max.y++;
    }

    map_find_start(&map, &player);
    player.facing = EAST;

    printf("Start position: x=%llu y=%llu f=%c\n", player.pos.x, player.pos.y, dir[player.facing]);
    printf("Map size: max_x=%llu max_y=%llu\n", map.max.x, map.max.y);

    // map_print(&map);

    char c;
    int num;
    int count = 0;
    while ((count = fscanf(fp, "%d%c", &num, &c)) >= 1)
    {
        if (count == 2)
        {
            // printf("mov: steps=%d dir=%c turn: %c\n", num, dir[player.facing], c);
            player_move(&player, &map, num);
            player_turn(&player, c);
        }
        else if (count == 1)
        {
            // printf("mov: steps=%d dir=%c\n", num, dir[player.facing]);
            player_move(&player, &map, num);
        }
        else
        {
            assert(0);
        }
        // printf("pos: x=%llu y=%llu f=%c\n", player.pos.x, player.pos.y, dir[player.facing]);
    }
    map.map[player.pos.y][player.pos.x] = 'E'; /* set end marker */

    // map_to_file(&map);

    size_t pw = (1000U * (player.pos.y + 1U))
              + (4U    * (player.pos.x + 1U))
              + (size_t) player.facing;

    printf("End position: x=%llu y=%llu f=%d\n", player.pos.x, player.pos.y, player.facing);
    printf("Part 1: password = %llu\n", pw);

    fclose(fp);

    return EXIT_SUCCESS;
}

static void
map_find_start(grid_t* m, player_t* p)
{
    for (size_t y = 0U; y < m->max.y; y++)
    {
        for (size_t x = 0U; x < m->max.x; x++)
        {
            if (m->map[y][x] == '.')
            {
                p->pos.x = x;
                p->pos.y = y;
                return;
            }
        }
    }

    assert(0); /* not found, must not happen */
}

static void
map_print(grid_t* m)
{
    for (size_t y = 0U; y < m->max.y; y++)
    {
        for (size_t x = 0U; x < m->max.x; x++)
        {
            printf("%c", m->map[y][x]);
        }
        putchar('\n');
    }
}

static void
map_to_file(grid_t* m)
{
    FILE* fp = fopen("map.txt", "w+");
    if (!fp)
        assert(0);

    for (size_t y = 0U; y < m->max.y; y++)
    {
        for (size_t x = 0U; x < m->max.x; x++)
        {
            fprintf(fp, "%c", m->map[y][x]);
        }
        fputc('\n', fp);
    }

    fclose(fp);
}

static void
player_turn(player_t* p, char d)
{
    switch (d)
    {
        case 'L':
            p->facing = mapdir[LEFT][p->facing];
            break;
        case 'R':
            p->facing = mapdir[RIGHT][p->facing];
            break;
        default:
            assert(0);
    }
}

static int
player_move(player_t* p, grid_t* m, int s)
{
    if (s <= 0)
        return 0;

    switch (p->facing)
    {
        case NORTH:
            player_move_up(p, m, s);
            break;
        case EAST:
            player_move_right(p, m, s);
            break;
        case SOUTH:
            player_move_down(p, m, s);
            break;
        case WEST:
            player_move_left(p, m, s);
            break;
        default:
            assert(0);
    }

    return 1;
}

static void
player_move_right(player_t* p, grid_t* m, int s)
{
    assert(s > 0);

    int moving = 1;
    while (moving)
    {
        if (   ((p->pos.x + 1U) >= m->max.x)
            || (m->map[p->pos.y][p->pos.x + 1U] == ' ') )
        {
            size_t new_x = 0U;
            int r = wrap_r_to_l(p, m, &new_x);
            assert(r >= 0);
            if (r == 0)
            {
                moving = 0;
            }
            else
            {
                m->map[p->pos.y][p->pos.x] = '>';
                p->pos.x = new_x;
                s--;
                if (s <= 0)
                    moving = 0;
            }
        }
        else
        {
            if (m->map[p->pos.y][p->pos.x + 1U] == '#')
            {
                moving = 0;
            }
            else
            {
                m->map[p->pos.y][p->pos.x] = '>';
                p->pos.x++;
                s--;
                if (s <= 0)
                    moving = 0;
            }
        }
    }
}

static void
player_move_left(player_t* p, grid_t* m, int s)
{
    assert(s > 0);

    int moving = 1;
    while (moving)
    {
        if (   (p->pos.x == 0U)
            || (m->map[p->pos.y][p->pos.x - 1U] == ' '))
        {
            size_t new_x = 0U;
            int r = wrap_l_to_r(p, m, &new_x);
            assert(r >= 0);
            if (r == 0)
            {
                moving = 0;
            }
            else
            {
                m->map[p->pos.y][p->pos.x] = '<';
                p->pos.x = new_x;
                s--;
                if (s <= 0)
                    moving = 0;
            }
        }
        else
        {
            if (m->map[p->pos.y][p->pos.x - 1U] == '#')
            {
                moving = 0;
            }
            else
            {
                m->map[p->pos.y][p->pos.x] = '<';
                p->pos.x--;
                s--;
                if (s <= 0)
                    moving = 0;
            }
        }
    }
}

static void
player_move_up(player_t* p, grid_t* m, int s)
{
    assert(s > 0);

    int moving = 1;
    while (moving)
    {
        if (   (p->pos.y == 0U)
            || (m->map[p->pos.y - 1U][p->pos.x] == ' '))
        {
            size_t new_y = 0U;
            int r = wrap_t_to_b(p, m, &new_y);
            assert(r >= 0);
            if (r == 0)
            {
                moving = 0;
            }
            else
            {
                m->map[p->pos.y][p->pos.x] = '^';
                p->pos.y = new_y;
                s--;
                if (s <= 0)
                    moving = 0;
            }
        }
        else
        {
            if (m->map[p->pos.y - 1U][p->pos.x] == '#')
            {
                moving = 0;
            }
            else
            {
                m->map[p->pos.y][p->pos.x] = '^';
                p->pos.y--;
                s--;
                if (s <= 0)
                    moving = 0;
            }
        }
    }
}

static void
player_move_down(player_t* p, grid_t* m, int s)
{
    assert(s > 0);

    int moving = 1;
    while (moving)
    {
        if (   ((p->pos.y + 1U) >= m->max.y)
            || (m->map[p->pos.y + 1U][p->pos.x] == ' ') )
        {
            size_t new_y = 0U;
            int r = wrap_b_to_t(p, m, &new_y);
            assert(r >= 0);
            if (r == 0)
            {
                moving = 0;
            }
            else
            {
                m->map[p->pos.y][p->pos.x] = 'v';
                p->pos.y = new_y;
                s--;
                if (s <= 0)
                    moving = 0;
            }
        }
        else
        {
            if (m->map[p->pos.y + 1U][p->pos.x] == '#')
            {
                moving = 0;
            }
            else
            {
                m->map[p->pos.y][p->pos.x] = 'v';
                p->pos.y++;
                s--;
                if (s <= 0)
                    moving = 0;
            }
        }
    }
}

static int
wrap_r_to_l(const player_t* p, const grid_t* m, size_t* new_x)
{
    for (size_t i = 0U; i < p->pos.x; i++)
    {
        switch (m->map[p->pos.y][i])
        {
            case ' ':
                break;
            case '#':
                return 0;
            case '.':
            case '<':
            case '>':
            case '^':
            case 'v':
                *new_x = i;
                return 1;
            default:
                return -1;
        }
    }

    return -1;
}

static int
wrap_l_to_r(const player_t* p, const grid_t* m, size_t* new_x)
{
    for (size_t i = m->max.x; i-- > 0U; )
    {
        switch (m->map[p->pos.y][i])
        {
            case ' ':
                break;
            case '#':
                return 0;
            case '.':
            case '<':
            case '>':
            case '^':
            case 'v':
                *new_x = i;
                return 1;
            default:
                return -1;
        }
    }

    return -1;
}

static int
wrap_b_to_t(const player_t* p, const grid_t* m, size_t* new_y)
{
    for (size_t i = 0U; i < p->pos.y; i++)
    {
        switch (m->map[i][p->pos.x])
        {
            case ' ':
                break;
            case '#':
                return 0;
            case '.':
            case '<':
            case '>':
            case '^':
            case 'v':
                *new_y = i;
                return 1;
            default:
                return -1;
        }
    }

    return -1;
}

static int
wrap_t_to_b(const player_t* p, const grid_t* m, size_t* new_y)
{
    for (size_t i = m->max.y; i-- > 0U; )
    {
        switch (m->map[i][p->pos.x])
        {
            case ' ':
                break;
            case '#':
                return 0;
            case '.':
            case '<':
            case '>':
            case '^':
            case 'v':
                *new_y = i;
                return 1;
            default:
                return -1;
        }
    }

    return -1;
}

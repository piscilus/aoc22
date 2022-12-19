/*
 * Main program for advent of code 2022 day 17.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "rocks.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CHUNK_SIZE (128U)
#define NUM_ROCKS  (2022U)
#define CAVE_WIDTH (7U)

typedef struct
{
    char *data;
    size_t used;
    size_t size;
} jet_pattern_t;

typedef struct
{
    char** data;
    size_t used;
    size_t height;
    size_t width;
} chamber_t;

typedef struct
{
    size_t x;
    size_t y;
} coords_t;

typedef struct
{
    const rock_t* rock;
    coords_t pos;
} object_t;

/* initialize/prepare chamber once */
static int
chamber_init(chamber_t* c);

/* extend height of chamber if necessary */
static int
chamber_extend(chamber_t* c, size_t h);

/* fill unused area of chamber with given character */
static void
chamber_fill(chamber_t* c, char f);

/* print current status of chamber */
static void
chamber_print(const chamber_t* c);

/* add a landed rock to chamber grid */
static int
chamber_add_rock(chamber_t* c, const object_t* r);

/* spawn a new rock that will start falling down */
static int
chamber_spawn_rock(chamber_t* c, object_t* o);

/* check for collision at given position*/
static int
check_collision(const chamber_t* c, const rock_t* r, size_t r_x, size_t r_y);

/* delete chamber object, cleanup memory */
static void
chamber_delete(chamber_t* c);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 17: Pyroclastic Flow\n\n");

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

    jet_pattern_t jp;

    jp.size = CHUNK_SIZE;
    jp.data = (char*) malloc(jp.size * sizeof(char));
    assert(jp.data != NULL);
    jp.used = 0U;

    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        if (jp.used >= jp.size)
        {
            jp.size += CHUNK_SIZE;
            jp.data = (char*) realloc(jp.data, jp.size * sizeof(char));
            assert(jp.data != NULL);
        }

        jp.data[jp.used] = (char) c;
        jp.used++;
    }

    fclose(fp);

    chamber_t chamber;

    if (chamber_init(&chamber) < 0)
    {
        fprintf(stderr, "Failed to initialize chamber.");
        exit(EXIT_FAILURE);
    }

    size_t jet = 0U;
    for (size_t rocks = 0U; rocks < NUM_ROCKS; rocks++)
    {
        object_t object;

        if (chamber_spawn_rock(&chamber, &object) < 0)
        {
            fprintf(stderr, "Failed to spawn new rock.");
            exit(EXIT_FAILURE);
        }

        // printf("rock #%llu\n", rocks);

        int moving = 1;
        size_t alternate = 0U; /* alternate between gravity and jet streams */
        while (moving > 0)
        {
            moving = 0;
            if ((alternate % 2U) == 0U) /* pushed by jet */
            {
                if (jp.data[jet % jp.used] == '>') /* push right */
                {
                    if ((object.pos.x + object.rock->width) < chamber.width)
                    {
                        if (!check_collision(&chamber, object.rock, object.pos.x + 1, object.pos.y))
                        {
                            object.pos.x++;
                        }
                    }
                }
                else
                {
                    assert(jp.data[jet % jp.used] == '<'); /* push left */
                    if (object.pos.x > 0U)
                    {
                        if (!check_collision(&chamber, object.rock, object.pos.x - 1, object.pos.y))
                        {
                            object.pos.x--;
                        }
                    }
                }
                jet++;
                moving = 1;
            }
            else /* fall down */
            {
                if ((object.pos.y - object.rock->height) > 0U)
                {
                    if (!check_collision(&chamber, object.rock, object.pos.x, object.pos.y - 1))
                    {
                        object.pos.y--;
                        moving = 1;
                    }
                }
            }
            alternate++;
        }
        chamber_add_rock(&chamber, &object);

        // chamber_print(&chamber);
        // putchar('\n');
    }

    printf("Part 1: height of rocks = %llu\n", chamber.used - 1U); /* minus floor */

    chamber_delete(&chamber);
    free(jp.data);

    return EXIT_SUCCESS;
}

static int
chamber_init(chamber_t* c)
{
    assert(c != NULL);

    c->width = CAVE_WIDTH;
    c->height = 1U; /* height of floor */
    c->data = (char**) malloc(c->height * sizeof(char*));
    if (c->data == NULL)
        return -1;
    for (size_t y = 0U; y < c->height; y++)
    {
        c->data[y] = (char*) malloc(c->width * sizeof(char));
        if (c->data[y] == NULL)
            return -1;
    }
    c->used = 0U;

    /* add floor pattern */
    chamber_fill(c, '-');
    c->used = 1U;

    return 1;
}

static int
chamber_extend(chamber_t* c, size_t h)
{
    assert(c != NULL);
    assert(h > 0U);

    if ((c->height - c->used) >= (h + 3U))
        return 0;

    size_t add_size = (c->used + h + 3U - c->height);
    c->height += add_size;

    c->data = (char**) realloc(c->data, c->height * sizeof(char*));
    if (c->data == NULL)
        return -1;
    for (size_t y = c->height - add_size; y < c->height; y++)
    {
        c->data[y] = (char*) malloc(c->width * sizeof(char));
        if (c->data[y] == NULL)
            return -1;
    }

    chamber_fill(c, '.');

    return 1;
}

static void
chamber_fill(chamber_t* c, char f)
{
    assert(c != NULL);

    for (size_t y = c->used; y < c->height; y++)
    {
        for (size_t x = 0U; x < c->width; x++)
        {
            c->data[y][x] = f;
        }
    }
}

static void
chamber_print(const chamber_t* c)
{
    assert(c != NULL);

    for (size_t y = c->height; y-- > 0U; )
    {
        for (size_t x = 0U; x < c->width; x++)
        {
            putchar(c->data[y][x]);
        }
        putchar('\n');
    }
}

static int
chamber_add_rock(chamber_t* c, const object_t* r)
{
    assert(c != NULL);
    assert(r->pos.y >= r->rock->height);

    for (size_t y = r->pos.y; y-- > (r->pos.y - r->rock->height); )
    {
        for (size_t x = r->pos.x; x < (r->pos.x + r->rock->width); x++)
        {
            if (r->rock->fig[r->pos.y - y - 1U][x - r->pos.x] == '@')
            {
                c->data[y + 1U][x] = '#';
            }
        }
    }

    if ( r->pos.y >= c->used)
        c->used += r->pos.y - c->used + 1U;

    return 1;
}

static int
chamber_spawn_rock(chamber_t* c, object_t* o)
{
    assert(c != NULL);

    o->rock = rock_get();

    if (chamber_extend(c, o->rock->height) < 0)
        return -1;

    o->pos.x = 2U;
    o->pos.y = c->used + 3U + o->rock->height - 1U;

    return 1;
}

static int
check_collision(const chamber_t* c, const rock_t* r, size_t r_x, size_t r_y)
{
    assert(r_y >= r->height);
    assert(r_y < c->height);

    r_y++;
    for (size_t y = r_y; y-- > (r_y - r->height); )
    {
        for (size_t x = r_x; x < (r_x + r->width); x++)
        {
            if ((r->fig[r_y - y - 1U][x - r_x] == '@') && (c->data[y][x] != '.'))
            {
                return 1;
            }
        }
    }

    return 0;
}

static void
chamber_delete(chamber_t* c)
{
    for (size_t y = 0U; y < c->height; y++)
    {
        free(c->data[y]);
    }
    free(c->data);

    c->width = 0U;
    c->height = 0U;
    c->used = 0U;
}

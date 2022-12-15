/*
 * Main program for advent of code 2022 day 15.
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

#define MAX_LINE_SIZE (100U)
#define MAX_SENSORS   (50U)
#define ROW_EXAMPLE   (10)
#define ROW_CONTEST   (2000000)

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ABS(n)  (((n) < 0) ? -(n) : (n))

enum
{
    YES,
    NO
};

typedef struct
{
    int64_t x;
    int64_t y;
} coords_t;

typedef struct
{
    coords_t self;   /* coords of sensor itself... */
    coords_t beacon; /* ... and its beacon*/
} sensor_t;

typedef struct
{
    coords_t max;
    coords_t min;
} grid_t;

/* https://en.wikipedia.org/wiki/Taxicab_geometry */
static inline int64_t manhattan_distance(int64_t x1, int64_t y1, int64_t x2, int64_t y2)
{
    return ABS((x2) - (x1)) + ABS((y2) - (y1));
}

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 15: Beacon Exclusion Zone\n\n");

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

    size_t sc = 0U;
    sensor_t sensors[MAX_SENSORS] = {0};

    char line_buf[MAX_LINE_SIZE] = {0};
    while (fgets(line_buf, MAX_LINE_SIZE, fp) != NULL)
    {
        if (sscanf(
                line_buf,
                "Sensor at x=%lld, y=%lld: closest beacon is at x=%lld, y=%lld",
                &sensors[sc].self.x,
                &sensors[sc].self.y,
                &sensors[sc].beacon.x,
                &sensors[sc].beacon.y) == 4)
        {
            sc++;
        }
        else
        {
            fprintf(stderr, "Could not parse input!");
            exit(EXIT_FAILURE);
        }
    }

    fclose(fp);

    grid_t grid = {0};
    grid.min.x = INT64_MAX;
    grid.min.y = INT64_MAX;

    printf("Number of sensors: %llu\n\n", sc);

    /* determine size of grid */
    for (size_t i = 0U; i < sc; i++)
    {
        grid.min.x = MIN(sensors[i].self.x, grid.min.x);
        grid.min.x = MIN(sensors[i].beacon.x, grid.min.x);

        grid.max.x = MAX(sensors[i].self.x, grid.max.x);
        grid.max.x = MAX(sensors[i].beacon.x, grid.max.x);

        grid.min.y = MIN(sensors[i].self.y, grid.min.y);
        grid.min.y = MIN(sensors[i].beacon.y, grid.min.y);

        grid.max.y = MAX(sensors[i].self.y, grid.max.y);
        grid.max.y = MAX(sensors[i].beacon.y, grid.max.y);
    }

    printf("min x = %lld\n", grid.min.x);
    printf("max x = %lld\n", grid.max.x);
    int64_t dx = grid.max.x - grid.min.x;
    printf("   dx = %lld\n\n", dx);
    printf("min y = %lld\n", grid.min.y);
    printf("max y = %lld\n", grid.max.y);
    int64_t dy = grid.max.y - grid.min.y;
    printf("   dy = %lld\n\n", dy);

    int64_t rows_wo_beacon = 0;

    /* for every point on the row ... */
    for (int64_t x = grid.min.x - (dy / 2); x <= grid.max.x + (dy / 2); x++)
    {
        /* ... check all sensors and their beacons */
        int has_beacon = YES;
        for (size_t s = 0U; s < sc; s++)
        {
            /* if beacon on row */
            if ((x == sensors[s].beacon.x) && (ROW_CONTEST == sensors[s].beacon.y))
            {
                has_beacon = YES;
                break;
            }
            /* if sensor is closer to point than to the beacon -> no beacon possible */
            else if (  manhattan_distance(sensors[s].self.x, sensors[s].self.y, x, ROW_CONTEST)
                    <= manhattan_distance(sensors[s].self.x, sensors[s].self.y, sensors[s].beacon.x, sensors[s].beacon.y))
            {
                has_beacon = NO;
                break;
            }
        }
        if (has_beacon == NO)
            rows_wo_beacon++;
    }

    printf("Part 1: position without beacons on row %lld = %lld", ROW_CONTEST, rows_wo_beacon);

    return EXIT_SUCCESS;
}

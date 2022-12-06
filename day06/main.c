/*
 * Main program for advent of code 2022 day 6.
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

#define MAX_LINE_SIZE (4097U) /* max line length of input file, incl. EOL */

static size_t find_start_of_packet(const char datastream[], size_t len, size_t marker);
static int unique(const char s[], size_t len);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 6: Tuning Trouble\n\n");

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

    char line_buf[MAX_LINE_SIZE];
    while (fgets(line_buf, MAX_LINE_SIZE, fp))
    {
        size_t len = strlen(line_buf);
        size_t start_of_packet;

        assert(len > 0U);
        len--; /* ignore EOL */

        printf("part 1:\n");
        start_of_packet = find_start_of_packet(line_buf, len, 4U);
        if (start_of_packet)
            printf("> first marker after character = %lld\n", start_of_packet);
        else
            printf("> no marker found!\n");

        printf("part 2:\n");
        start_of_packet = find_start_of_packet(line_buf, len, 14U);
        if (start_of_packet)
            printf("> first marker after character = %lld\n", start_of_packet);
        else
            printf("> no marker found!\n");
        printf("\n");
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

static size_t find_start_of_packet(const char datastream[], size_t len, size_t marker)
{
    assert(marker > 0);
    assert(len > marker);

    for (size_t i = marker - 1U; i < len; i++)
        if (unique(&datastream[i - marker + 1U], marker))
            return i + 1U;

    return 0U;
}

int unique(const char s[], size_t len)
{
    int lut[128] = {0};

    for (size_t i = 0U; i < len; i++)
    {
        int n = (int) s[i];
        if (lut[n] != 0)
            return 0;
        else
            lut[n] = 1;
    }

    return 1;
}
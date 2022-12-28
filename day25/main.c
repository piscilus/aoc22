/*
 * Main program for advent of code 2022 day 25.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUFSIZE (128U)

static int
char_conv(char in);

static long long
strtoll_snafu(const char* nptr, char** endptr);

static char*
lltostr_snafu(char *str, long long val);

int main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 23: Unstable Diffusion\n\n");

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

    char line_buf[BUFSIZE];
    char* end;
    long long res = 0;
    while (fgets(line_buf, BUFSIZE, fp))
    {
        errno = 0;
        long long r =  strtoll_snafu(line_buf, &end);
        assert(errno == 0);
        assert((*end == '\n') || (*end == '\0'));
        res += r;
    }
    fclose(fp);

    memset(line_buf, '\0', BUFSIZE);
    lltostr_snafu(line_buf, res);
    printf("Part 1: SNAFU number = %s\n", line_buf);

    return EXIT_SUCCESS;
}

static int
char_conv(char in)
{
    switch (in)
    {
        case '=':
            return 0;
        case '-':
            return 1;
        case '0':
            return 2;
        case '1':
            return 3;
        case '2':
            return 4;
        default:
            return EOF;
    }
}

static long long
strtoll_snafu(const char* nptr, char** endptr)
{
    int base = 5;
    const char *s = nptr, *endp;
    _Bool neg = 0, overflow = 0;
    unsigned long long n = 0UL, cutoff;
    int cutlim;

    endp = nptr;

    while (isspace(*s))
    {
        s++;
    }

    if (*s == '+')
    {
        s++;
    }
    else if (*s == '-')
    {
        neg = 1;
        s++;
    }

    cutoff = (neg) ? -(LLONG_MIN / base) : LLONG_MAX / base;
    cutlim = (neg) ? -(LLONG_MIN % base) : LLONG_MAX % base;

    while (1)
    {
        int c = char_conv(*s);
        if ((c < 0) || (c >= base)) break;
        endp = ++s;
        if (overflow)
        {
            if (endptr) continue;
            break;
        }
        if ((n > cutoff) || ((n == cutoff) && (c > cutlim)))
        {
            overflow = 1; continue;
        }
        n = n * base + c - 2;
    }

    if (endptr)
    {
         *endptr = (char *)endp;
    }

    if (overflow)
    {
        errno = ERANGE;
        return ((neg) ? LLONG_MIN : LLONG_MAX);
    }

    return (long long)((neg) ? -n : n);
}

static char*
lltostr_snafu(char *str, long long val)
{
    lldiv_t r;

    if (val < 0)
        assert(0);

    r = lldiv(val + 2, 5);
    if (r.quot > 0)
        str = lltostr_snafu(str, r.quot);

    assert(r.rem < 5);
    *str++ = "=-012"[(int)r.rem];
    *str = '\0';

    return str;
}

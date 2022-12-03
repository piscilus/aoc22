/**
 * \file main.c
 *
 * \copyright (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 *
 * \brief Main program for advent of code 2022 day 2.
 */

/*---- Includes --------------------------------------------------------------*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/*---- Local macro definitions -----------------------------------------------*/
#define MAX_LINE_SIZE (1024)

#define SCORE_LOSS (0)
#define SCORE_DRAW (3)
#define SCORE_WIN  (6)

#define SCORE_ROCK     (1)
#define SCORE_PAPER    (2)
#define SCORE_SCISSORS (3)


/*---- Local type definitions ------------------------------------------------*/
typedef enum
{
    R = 0,  /* rock */
    P,      /* paper */
    S,      /* scissors */
    NUM_CHOICE_
} choice_t;

typedef enum
{
    X = 0,
    Y,
    Z,
    NUM_MY_CHOICE_
} choice_mine_t;


/*---- Local function prototypes ---------------------------------------------*/


/*---- Global constants ------------------------------------------------------*/


/*---- Global data -----------------------------------------------------------*/


/*---- Local constants -------------------------------------------------------*/


/*---- Local data ------------------------------------------------------------*/
/* part 1: get battle score by choices */
const int score_battle_p1[NUM_CHOICE_][NUM_CHOICE_] =
{
    [R] =
    {
        [R] = SCORE_DRAW,  /* rock vs. rock */
        [P] = SCORE_WIN,   /* rock vs. paper */
        [S] = SCORE_LOSS   /* rock vs. scissors */
    },
    [P] =
    {
        [R] = SCORE_LOSS,  /* paper vs. rock */
        [P] = SCORE_DRAW,  /* paper vs. paper */
        [S] = SCORE_WIN,   /* paper vs. scissors */
    },
    [S] =
    {
        [R] = SCORE_WIN,   /* scissors vs. rock */
        [P] = SCORE_LOSS,  /* scissors vs. paper */
        [S] = SCORE_DRAW   /* scissors vs. scissors */
    }
};

/* part 1:get choice by tip */
const choice_t choice_p1[NUM_CHOICE_] =
{
    [X] = R,
    [Y] = P,
    [Z] = S
};

/* part 1/2: get choice score by choice */
const int score_choice[NUM_CHOICE_] =
{
    [R] = SCORE_ROCK,
    [P] = SCORE_PAPER,
    [S] = SCORE_SCISSORS
};

/* part 2: get choice by goal and opponent */
const choice_t choice_by_goal[NUM_MY_CHOICE_][NUM_CHOICE_] =
{
    [X] = /* loss */
    {
        [R] = S,
        [P] = R,
        [S] = P
    },
    [Y] = /* draw */
    {
        [R] = R,
        [P] = P,
        [S] = S
    },
    [Z] = /* win */
    {
        [R] = P,
        [P] = S,
        [S] = R
    }
};

/* part 2: get battle score by goal */
const choice_t score_battle_p2[NUM_MY_CHOICE_] =
{
    [X] = SCORE_LOSS,
    [Y] = SCORE_DRAW,
    [Z] = SCORE_WIN
};


/*---- Exported functions ----------------------------------------------------*/
int main(int argc, char *argv[])
{
    char linebuf[MAX_LINE_SIZE];

    printf("Advent of Code 2022 - Day 2: Rock Paper Scissors\n\n");

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

    int line = 1;
    int score_part1 = 0;
    int score_part2 = 0;
    while (fgets(linebuf, MAX_LINE_SIZE, fp))
    {
        char opponent = '\0';
        char me = '\0';
        if (sscanf(linebuf, "%c %c", &opponent, &me) == 2)
        {
            switch (opponent)
            {
                case 'A': /* rock */
                    switch(me)
                    {
                        case 'X':
                            score_part1 += score_battle_p1[R][choice_p1[X]] + score_choice[X];
                            score_part2 += score_battle_p2[X] + score_choice[choice_by_goal[X][R]];
                            break;
                        case 'Y':
                            score_part1 += score_battle_p1[R][choice_p1[Y]] + score_choice[Y];
                            score_part2 += score_battle_p2[Y] + score_choice[choice_by_goal[Y][R]];
                            break;
                        case 'Z':
                            score_part1 += score_battle_p1[R][choice_p1[Z]] + score_choice[Z];
                            score_part2 += score_battle_p2[Z] + score_choice[choice_by_goal[Z][R]];
                            break;
                        default:
                            fprintf(stderr, "Invalid selection by myself %c in line %d!", me, line);
                            exit(EXIT_FAILURE);
                            break;
                    }
                    break;
                case 'B': /* paper */
                    switch(me)
                    {
                        case 'X':
                            score_part1 += score_battle_p1[P][choice_p1[X]] + score_choice[X];
                            score_part2 += score_battle_p2[X] + score_choice[choice_by_goal[X][P]];
                            break;
                        case 'Y':
                            score_part1 += score_battle_p1[P][choice_p1[Y]] + score_choice[Y];
                            score_part2 += score_battle_p2[Y] + score_choice[choice_by_goal[Y][P]];
                            break;
                        case 'Z':
                            score_part1 += score_battle_p1[P][choice_p1[Z]] + score_choice[Z];
                            score_part2 += score_battle_p2[Z] + score_choice[choice_by_goal[Z][P]];
                            break;
                        default:
                            fprintf(stderr, "Invalid selection by myself %c in line %d!", me, line);
                            exit(EXIT_FAILURE);
                            break;
                    }
                    break;
                case 'C': /* scissors */
                    switch(me)
                    {
                        case 'X':
                            score_part1 += score_battle_p1[S][choice_p1[X]] + score_choice[X];
                            score_part2 += score_battle_p2[X] + score_choice[choice_by_goal[X][S]];
                            break;
                        case 'Y':
                            score_part1 += score_battle_p1[S][choice_p1[Y]] + score_choice[Y];
                            score_part2 += score_battle_p2[Y] + score_choice[choice_by_goal[Y][S]];
                            break;
                        case 'Z':
                            score_part1 += score_battle_p1[S][choice_p1[Z]] + score_choice[Z];
                            score_part2 += score_battle_p2[Z] + score_choice[choice_by_goal[Z][S]];
                            break;
                        default:
                            fprintf(stderr, "Invalid selection by myself %c in line %d!", me, line);
                            exit(EXIT_FAILURE);
                            break;
                    }
                    break;
                default:
                    fprintf(stderr, "Invalid selection by opponent %c in line %d!", opponent, line);
                    exit(EXIT_FAILURE);
                    break;
            }
        }
        else
        {
            fprintf(stderr, "Unexpected data in line %d!", line);
            exit(EXIT_FAILURE);
        }
        line++;
    }

    printf("score part 1: %d\n", score_part1);
    printf("score part 2: %d\n", score_part2);

    fclose(fp);

    return EXIT_SUCCESS;
}


/*---- Local functions -------------------------------------------------------*/


/*----------------------------------------------------------- END OF FILE ----*/

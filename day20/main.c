/*
 * Main program for advent of code 2022 day 20.
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

#define CHUNK_SIZE (128U)

typedef struct
{
    size_t idx;
    int num;
} data_t;

typedef struct node
{
    data_t data;
    struct node* prev;
    struct node* next;
} node_t;

int
list_add_tail(node_t** node, data_t* data);

void
list_print(node_t* node);

node_t*
list_find_idx(node_t* node, size_t idx);

node_t*
list_find_num(node_t* node, int num);

node_t*
list_get_node(node_t* node, size_t n);

int
list_move_node(node_t** node, size_t idx);

void
list_free(node_t** node);

int
main(int argc, char *argv[])
{
    printf("Advent of Code 2022 - Day 20: Grove Positioning System\n\n");

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

    node_t* head = NULL;

    char line_buf[CHUNK_SIZE];
    data_t data = {0};
    size_t n = 0U;
    while (fgets(line_buf, CHUNK_SIZE, fp))
    {
        if (sscanf(line_buf, "%d", &data.num) == 1)
        {
            int r = list_add_tail(&head, &data);
            assert(r != 0);
            n++;
            data.idx++;
        }
        else
            assert(0);
    }

    fclose(fp);

    // list_print(head);

    for (size_t i = 0U; i < n; i++)
    {
        int r = list_move_node(&head, i);
        assert(r > 0);
        // putchar('\n');
        // list_print(head); // DEBUG
        // putchar('\n');
    }

    // list_print(head);

    int result = 0;

    node_t* node = list_get_node(head, 1000U);
    assert(node != NULL);
    // printf("%d\n", node->data.num);
    result += node->data.num;

    node = list_get_node(head, 2000U);
    assert(node != NULL);
    // printf("%d\n", node->data.num);
    result += node->data.num;

    node = list_get_node(head, 3000U);
    assert(node != NULL);
    // printf("%d\n", node->data.num);
    result += node->data.num;

    printf("Part 1: %d\n", result);
    // -258 is wrong

    list_free(&head);

    return EXIT_SUCCESS;
}

int
list_add_tail(node_t** node, data_t* data)
{
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL)
        return 0;

    new_node->data = *data;

    if (*node == NULL)
    {
        new_node->prev = new_node;
        new_node->next = new_node;
        *node = new_node;
    }
    else
    {
        new_node->prev = (*node)->prev;
        new_node->next = *node;
        (*node)->prev->next = new_node;
        (*node)->prev = new_node;
    }

    return 1;
}

void
list_print(node_t* node)
{
    if (node == NULL)
    {
        printf("list empty\n");
        return;
    }

    node_t* tmp = node;
    do
    {
        printf("[%lli] %d\n", tmp->data.idx, tmp->data.num);
        tmp = tmp->next;
    }
    while(tmp != node);
}

node_t*
list_find_idx(node_t* node, size_t idx)
{
    if (node == NULL)
        return NULL;

    node_t* tmp = node;
    do
    {
        if (tmp->data.idx == idx)
            return tmp;
        tmp = tmp->next;
    }
    while(tmp != node);

    return NULL;
}

node_t*
list_find_num(node_t* node, int num)
{
    if (node == NULL)
        return NULL;

    node_t* tmp = node;
    do
    {
        if (tmp->data.num == num)
            return tmp;
        tmp = tmp->next;
    }
    while(tmp != node);

    return NULL;
}

node_t*
list_get_node(node_t* node, size_t n)
{
    if (node == NULL)
        return NULL;

    node_t* tmp = list_find_num(node, 0);
    if (tmp == NULL)
        return NULL;
    while (n > 0U)
    {
        tmp = tmp->next;
        n--;
    }

    return tmp;
}

int
list_move_node(node_t** node, size_t idx)
{
    assert(*node != NULL);

    node_t* tmp = list_find_idx(*node, idx);
    if (tmp == NULL)
        return 0;
    int steps = tmp->data.num;
    node_t* repl = tmp;

    if (steps > 0)
    {
        /* move tmp after repl */
        while (steps > 0)
        {
            repl = repl->next;
            steps--;
        }
        if (repl == tmp) //TODO handle this case
            return 1;
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        repl->next->prev = tmp;
        tmp->next = repl->next;
        repl->next = tmp;
        tmp->prev = repl;
    }
    else if (steps < 0)
    {
        /* move tmp before repl */
        while (steps < 0)
        {
            repl = repl->prev;
            steps++;
        }
        if (repl == tmp) //TODO handle this case
            return 1;
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        repl->prev->next = tmp;
        tmp->prev = repl->prev;
        repl->prev = tmp;
        tmp->next = repl;
    }

    return 1;
}

void
list_free(node_t** head)
{
    if (*head == NULL)
        return;

    node_t* tmp = *head;
    do
    {
        node_t* next = tmp->next;
        free(tmp);
        tmp = next->next;
    }
    while (tmp != *head);

    *head = NULL;
}

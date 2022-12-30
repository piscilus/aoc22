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

typedef struct
{
    node_t* head;
    node_t* tail;
} list_t;

list_t*
list_init(void);

int
list_add_tail(list_t* l, data_t* d);

void
list_print(const list_t* l);

node_t*
list_find_idx(const list_t* l, size_t idx);

node_t*
list_find_num(const list_t* l, int num);

node_t*
list_get_node(const list_t* l, size_t n);

int
list_move_node(list_t* l, size_t idx);

void
list_free(list_t* l);

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

    list_t* list = list_init();
    assert(list != NULL);

    char line_buf[CHUNK_SIZE];
    data_t data = {0};
    size_t n = 0U;
    while (fgets(line_buf, CHUNK_SIZE, fp))
    {
        if (sscanf(line_buf, "%d", &data.num) == 1)
        {
            int r = list_add_tail(list, &data);
            assert(r != 0);
            n++;
            data.idx++;
        }
        else
            assert(0);
    }

    fclose(fp);

    list_print(list);

    for (size_t i = 0U; i < n; i++)
    {
        int r = list_move_node(list, i);
        assert(r > 0);
        putchar('\n');
        list_print(list); // DEBUG
        putchar('\n');
    }

    // sth. wrong with head?!

    // list_print(list);

    int result = 0;

    node_t* node = list_get_node(list, 1000U);
    assert(node != NULL);
    printf("%d\n", node->data.num);
    result += node->data.num;

    node = list_get_node(list, 2000U);
    assert(node != NULL);
    printf("%d\n", node->data.num);
    result += node->data.num;

    node = list_get_node(list, 3000U);
    assert(node != NULL);
    printf("%d\n", node->data.num);
    result += node->data.num;

    printf("Part 1: %d\n", result);

    list_free(list);

    return EXIT_SUCCESS;
}

list_t*
list_init(void)
{
    list_t* new_list = (list_t*)malloc(sizeof(list_t));
    if (new_list == NULL)
        return NULL;
    new_list->head = NULL;
    new_list->tail = NULL;

    return new_list;
}

int
list_add_tail(list_t* l, data_t* d)
{
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL)
        return 0;

    new_node->data = *d;

    if (l->tail == NULL)
    {
        assert(l->head == NULL);
        new_node->prev = new_node;
        new_node->next = new_node;
        l->tail = new_node;
        l->head = new_node;
    }
    else
    {
        new_node->next = l->head;
        new_node->prev = l->tail;
        l->tail->next = new_node;
        l->head->prev = new_node;
        l->tail = new_node;
    }

    return 1;
}

void
list_print(const list_t* l)
{
    if (l->head == NULL)
    {
        printf("list empty\n");
        return;
    }

    node_t* tmp = l->head;
    do
    {
        printf("[%lli] %d\n", tmp->data.idx, tmp->data.num);
        tmp = tmp->next;
    }
    while(tmp != l->head);
}

node_t*
list_find_idx(const list_t* l, size_t idx)
{
    assert(l != NULL);
    if (l->head == NULL)
        return NULL;

    node_t* tmp = l->head;
    do
    {
        if (tmp->data.idx == idx)
            return tmp;
        tmp = tmp->next;
    }
    while(tmp != l->head);

    return NULL;
}

node_t*
list_find_num(const list_t* l, int num)
{
    assert(l != NULL);
    if (l->head == NULL)
        return NULL;

    node_t* tmp = l->head;
    do
    {
        if (tmp->data.num == num)
            return tmp;
        tmp = tmp->next;
    }
    while(tmp != l->head);

    return NULL;
}

node_t*
list_get_node(const list_t* l, size_t n)
{
    assert(l != NULL);
    if (l->head == NULL)
        return NULL;

    node_t* tmp = list_find_num(l, 0);
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
list_move_node(list_t* l, size_t idx)
{
    assert(l != NULL);

    node_t* tmp = list_find_idx(l, idx);
    printf("%llu %d\n", idx, tmp->data.num); // DEBUG
    if (tmp == NULL)
        return 0;
    int steps = tmp->data.num;
    node_t* repl = tmp;

    if (steps > 0)
    {
        while (steps > 0)
        {
            repl = repl->next;
            steps--;
        }
        // if (repl->next == tmp) // DEBUG
        //     return 1;
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        repl->next->prev = tmp;
        tmp->next = repl->next;
        repl->next = tmp;
        tmp->prev = repl;
    }
    else if (steps < 0)
    {
        steps++;
        while (steps < 0)
        {
            repl = repl->prev;
            steps++;
        }
        // if (repl->prev == tmp) // DEBUG
        //     return 1;
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
list_free(list_t* l)
{
    if ((l == NULL) || (l->head == NULL))
        return;

    node_t* tmp = l->head;
    do
    {
        node_t* next = tmp->next;
        free(tmp);
        tmp = next->next;
    }
    while (tmp != l->head);

    free(l);
    l = NULL;
}

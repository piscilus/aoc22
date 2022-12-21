/*
 * Simple hash table implementation.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#include "hashtable.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

typedef struct
{
    const char* key;
    void* value;
} data_t;

struct hashtable
{
    data_t* data;
    size_t size;
    size_t used;
};

static uint32_t
calc_hash(const char* key);

static const char*
write(data_t* data, size_t size, const char* key, void* new_data, size_t* used);

static int
extend_table(hashtable_t* ht);

hashtable_t*
ht_init(size_t size)
{
    hashtable_t* ht = malloc(sizeof(hashtable_t));
    if (ht == NULL)
        return NULL;

    ht->size = size;
    ht->used = 0U;

    ht->data = calloc(ht->size, sizeof(data_t));
    if (ht->data == NULL)
    {
        free(ht);
        return NULL;
    }

    return ht;
}

const char*
ht_write(hashtable_t* ht, const char* key, void* data)
{
    assert(data != NULL);

    if (ht->used >= (ht->size / 2U))
        if (!extend_table(ht))
            return NULL;

    return write(ht->data, ht->size, key, data, &ht->used);
}

void*
ht_read(hashtable_t* ht, const char* key)
{
    uint32_t hash = calc_hash(key);
    size_t index = (size_t)(hash & (uint32_t)(ht->size - 1U));

    while (ht->data[index].key != NULL)
    {
        if (strcmp(key, ht->data[index].key) == 0)
            return ht->data[index].value;
        index++;
        if (index >= ht->size)
            index = 0U;
    }

    return NULL;
}

void
ht_destroy(hashtable_t* ht)
{
    for (size_t i = 0U; i < ht->size; i++)
        free((void*)ht->data[i].key);

    free(ht->data);
    free(ht);
}

static uint32_t
calc_hash(const char* key) /* djb2 */
{
    uint32_t hash = 5381U;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

static const char*
write(data_t* data, size_t size, const char* key, void* new_data, size_t* used)
{
    uint32_t hash = calc_hash(key);
    size_t i = (size_t)(hash & (uint32_t)(size - 1U));

    while (data[i].key != NULL)
    {
        if (strcmp(key, data[i].key) == 0)
        {
            data[i].value = new_data;
            return data[i].key;
        }
        i++;
        if (i >= size)
            i = 0U;
    }

    if (used != NULL)
    {
        key = strdup(key);
        if (key == NULL)
            return NULL;
        (*used)++;
    }

    data[i].key = (char*)key;
    data[i].value = new_data;

    return key;
}

static int
extend_table(hashtable_t* ht)
{
    size_t new_size = ht->size * 2U;

    if (new_size < ht->size)
        return 0;

    data_t* new_data = calloc(new_size, sizeof(data_t));
    if (new_data == NULL)
        return 0;

    for (size_t i = 0U; i < ht->size; i++)
    {
        data_t entry = ht->data[i];
        if (entry.key != NULL)
        {
            (void) write(new_data, new_size, entry.key, entry.value, NULL);
        }
    }

    free(ht->data);

    ht->data = new_data;
    ht->size = new_size;

    return 1;
}

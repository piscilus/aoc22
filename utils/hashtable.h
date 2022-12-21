/*
 * Simple hash table implementation.
 *
 * (C) Copyright 2022 "piscilus" Julian Kraemer
 *
 * Distributed under MIT license.
 * See file LICENSE for details or copy at https://opensource.org/licenses/MIT
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdlib.h>

typedef struct hashtable hashtable_t;

hashtable_t*
ht_init(size_t size);

const char*
ht_write(hashtable_t* ht, const char* key, void* data);

void*
ht_read(hashtable_t* ht, const char* key);

void
ht_destroy(hashtable_t* ht);


#endif /* HASHTABLE_H_ */

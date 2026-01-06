
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "hash_table.h"
#include "prime.h"

#define HT_INITIAL_BASE_SIZE 53
#define HT_PRIME_1 163
#define HT_PRIME_2 157

static ht_item HT_DELETED_ITEM = {NULL, NULL};

static ht_item* ht_new_item(const char* k, const char* v) {
  ht_item* i = malloc(sizeof(ht_item));
  if (i == NULL) {
    return NULL;
  }
  
  i->key = strdup(k);
  if (i->key == NULL) {
    free(i);
    return NULL;
  }
  
  i->value = strdup(v);
  if (i->value == NULL) {
    free(i->key);
    free(i);
    return NULL;
  }
  
  return i;
}

ht_hash_table* ht_new() {
  ht_hash_table* ht = ht_new_sized(HT_INITIAL_BASE_SIZE);
  if (ht == NULL) {
    fprintf(stderr, "Error: failed to create hash table\n");
  }
  return ht;
}

static void ht_del_item(ht_item* i) {
  free(i->key);
  free(i->value);
  free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if (item != NULL) {
      ht_del_item(item);
    }
  }
  free(ht->items);
  free(ht);
}

static long int_pow(int base, int exp) {
  long result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

// Function hashes string `s` via prime `a` (larger than ASCII 128) into bucket < `m`
static int ht_hash(const char *s, const int a, const int m) {
  long hash = 0;
  const int len_s = strlen(s);
  for (int i = 0; i < len_s; i++) {
    hash += int_pow(a, len_s - (i + 1)) * s[i];
    hash = hash % m; // Prevent overflow by taking modulo inside loop
  }
  return (int) hash;
}

static int ht_get_hash(const char *s, const int num_buckets, const int attempt) {
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  if (hash_b % num_buckets == 0)
    hash_b = 1;
  return (hash_a + (attempt * hash_b)) % num_buckets;
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
  const int load = ht->count * 100 / ht->size;
  if (load > 70) ht_resize_up(ht);

  int idx = ht_get_hash(key, ht->size, 0);
  ht_item* cur_item = ht->items[idx];
  int attempts = 1;
  
  while (cur_item != NULL && attempts < ht->size) {
    if (cur_item != &HT_DELETED_ITEM) {
      if (strcmp(cur_item->key, key) == 0) {
        // Update existing key - just replace the value
        char* new_value = strdup(value);
        if (new_value == NULL) {
          fprintf(stderr, "Error: memory allocation failed\n");
          return;
        }
        free(cur_item->value);
        cur_item->value = new_value;
        return;
      }
    }
    idx = ht_get_hash(key, ht->size, attempts);
    cur_item = ht->items[idx];
    attempts++;
  }
  
  if (attempts >= ht->size) {
    fprintf(stderr, "Error: hash table is full\n");
    return;
  }
  
  // Insert new item
  ht_item* item = ht_new_item(key, value);
  if (item == NULL) {
    fprintf(stderr, "Error: failed to create hash table item\n");
    return;
  }
  
  ht->items[idx] = item;
  ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
  int idx = ht_get_hash(key, ht->size, 0);
  ht_item* item = ht->items[idx];
  int attempts = 1;
  
  while (item != NULL && attempts < ht->size) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        return item->value;
      }
    }
    idx = ht_get_hash(key, ht->size, attempts);
    item = ht->items[idx];
    attempts++;
  }
  return NULL;
}

void ht_delete(ht_hash_table *ht, const char *key) {
  const int load = ht->count * 100 / ht->size;
  if (load < 10) ht_resize_down(ht);
  
  int index = ht_get_hash(key, ht->size, 0);
  ht_item *item = ht->items[index];
  int i = 1;
  
  while (item != NULL && i < ht->size) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(item->key, key) == 0) {
        ht_del_item(item);
        ht->items[index] = &HT_DELETED_ITEM;
        ht->count--;
        return;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
}

static ht_hash_table* ht_new_sized(const int base_size) {
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));
  if (ht == NULL) {
    return NULL;
  }
  
  ht->base_size = base_size;
  ht->size = next_prime(ht->base_size);
  ht->count = 0;
  
  ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
  if (ht->items == NULL) {
    free(ht);
    return NULL;
  }
  
  return ht;
}

static void ht_resize(ht_hash_table* ht, const int base_size) {
  if (base_size < HT_INITIAL_BASE_SIZE) return;

  ht_hash_table* new_ht = ht_new_sized(base_size);
  if (new_ht == NULL) {
    fprintf(stderr, "Error: failed to resize hash table\n");
    return;
  }

  for (int i = 0; i < ht->size; i++) {
    ht_item* item = ht->items[i];
    if (item != NULL && item != &HT_DELETED_ITEM) {
      ht_insert(new_ht, item->key, item->value);
    }
  }

  ht->base_size = new_ht->base_size;
  ht->count = new_ht->count;

  // To delete new_ht give it ht's size and items
  const int tmp_size = ht->size;
  ht->size = new_ht->size;
  new_ht->size = tmp_size;

  ht_item** tmp_items = ht->items;
  ht->items = new_ht->items;
  new_ht->items = tmp_items;

  ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* ht) {
  const int new_size = ht->base_size * 2;
  ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table *ht) {
  const int new_size = ht->base_size / 2;
  ht_resize(ht, new_size);
}


#include "hash_table.h"

int main(int argc, char** argv) {
  ht_hash_table* ht = ht_new();
  ht_del_hash_table(ht);
  return 0;
}
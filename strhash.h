#ifndef _STRHASH
#define _STRHASH
/* a stackable hashing structure */
struct shash;

struct sh_entry{
      char tag[100];
      struct shash* subhash;

      /* linked list for multiple tags in the same bucket */
      struct sh_entry* next;
};

struct shash{
      int nbux;
      struct sh_entry** entries;
};

void init_shash(struct shash* h);
void insert_shash(struct shash* h, char** cur_path, int cur_depth, char* data);
#endif

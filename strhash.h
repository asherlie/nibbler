#ifndef _STRHASH
#define _STRHASH

/* a stackable hashing structure */
struct shash;

struct sh_entry{
      /* TODO: data should be a char* that is dynamically allocated */
      char* tag, * data;
      struct shash* subhash;

      /* last should only be set in first entry of bucket */
      /* linked list for multiple tags in the same bucket */
      struct sh_entry* next,
                     * last;
};

struct shash{
      struct shash* parent;

      int nbux,
          retries_used;
      /* last is used for O(1) insertion */
      struct sh_entry** entries;//, * last;
};

void init_shash(struct shash* h);
struct sh_entry* insert_shash(struct shash* h, char** cur_path, int cur_depth, char* data);

/* ind_shash() returns a struct sh_entry that is located at:
 * h[path[0]][path[1]][path[2]][...]
 */
struct sh_entry* ind_shash(struct shash* h, char** path, int depth, int index);
struct sh_entry* find_entry(struct shash* h, char** path, int n);
#endif

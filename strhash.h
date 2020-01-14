/* a stackable hashing structure */
struct shash;

struct sh_entry{
      char tag[20];
      struct shash* subhash;
};

struct shash{
      int nbux;
      struct sh_entry* entries;
};

void init_shash(struct shash* h);
void insert_shash(struct shash* h, char* tag);

#include "strhash.h"
#include <stdlib.h>

void init_shash(struct shash* h){
      h->nbux = 251;
      h->entries = calloc(h->nbux, sizeof(struct sh_entry));
}

/*void insert_shash(struct shash* h, char* tag){*/
void insert_shash(struct shash* h, char** cur_path, int cur_depth, char* tag){
      
      int ind;

      struct shash* sub_h = h;
      for(int i = 0; i < cur_depth; ++i){
            ind = *tag%h->nbux; 

            if(!sub_h->entries[ind]){
                  sub_h->entries[ind] = calloc(1, sizeof(struct sh_entry));
                  sub_h->entries[ind]->subhash = calloc(1, sizeof(struct shash));
                  init_shash(sub_h->entries[ind]->subhash);
            }
            sub_h = sub_h->entries[ind]->subhash;
      }

      /* at this point, sub_h will point to the appropriate shash* */
      /* might as well just use 255 indices for chars */
      /*int ind = *tag%h->nbux; */
      /* this is bad... */
      /*
       * struct sh_entry* e = &h->entries[ind];
       * if(!e){
       *       e = calloc(sizeof(struct sh_entry), 1);
       *       e->subhash = calloc(sizeof(struct shash), 1);
       *       init_shash(e->subhash);
       * }
      */
}
/*
 * 
 * counter = 0
 * for i in whole string
 *       ++counter at open brace
 *       --counter at close brace
 * 
 *       each open brace we insert a new tag 
*/

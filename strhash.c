#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strhash.h"

void init_shash(struct shash* h){
      h->nbux = 251;
      h->entries = calloc(h->nbux, sizeof(struct sh_entry));
      h->retries_used = 0;
      h->parent = h;
}

_Bool strtoi(const char* str, int* i){
     char* res;
     unsigned int r = strtol(str, &res, 10);
     if(*res)return 0;
     *i = (int)r;
     return 1;
}


/* follows a path of tags, indexes into the last path member */
struct sh_entry* grab_singlepass(struct shash* h, char** path, int n, int index){
      /*
       *int bucket = (*path)[1]%h->nbux;
       *oops - on the first entry we don't do a strcmp
       */
      /*int bucket;*/
      /*struct sh_entry* e = h->entries[bucket];*/
      /* creating a spoof entry with h as subhash */
      struct sh_entry E;
      E.subhash = h;
      struct sh_entry* e = &E;
      for(int i = 0; i < n; ++i){
            int bucket = path[i][1]%e->subhash->nbux;
            e = e->subhash->entries[bucket];
            for(; e; e = e->next){
                  /*printf("comparing %s %s: %i\n", e->tag, path[i], strcasecmp(e->tag, path[i]));*/
                  if(!strcasecmp(e->tag, path[i]))break;
            }
            /*printf("returned: %p\n", (void*)e);*/
            if(!e)return NULL;
      }

      int nfound = 0;

      while(e && nfound < index){
            if(!strcasecmp(e->tag, path[n-1]))++nfound;
            e = e->next;
      }

      return e;
}

struct sh_entry* find_entry(struct shash* h, char** path, int n){
      int ind;

      /* n is a strict upper bound for the number of
       * necessary calls to ind_shash
       */
      char** calls[n];
      /* call depth keeps track of the length
       * of subcalls
       */
      int call_depth[n], call_ind[n], subcalls = 1;
      memset(call_depth, 0, sizeof(int)*n);
      memset(call_ind, 0, sizeof(int)*n);

      calls[0] = path;
      call_depth[0] = 1;

      /* can we start at i = 1? */
      for(int i = 1; i < n; ++i){
            if(strtoi(path[i], &ind)){
                  call_ind[subcalls-1] = ind;
                  /* if there's more to parse, create new subcall */
                  if(i != n-1)calls[subcalls++] = path+i+1;
            }
            else{
                  ++call_depth[subcalls-1];
            }
      }

      struct shash* hh = h;
      struct sh_entry* e = NULL;
      for(int i = 0; i < subcalls; ++i){
            e = grab_singlepass(hh, calls[i], call_depth[i], call_ind[i]);
            if(!e)return NULL;
            hh = e->subhash;
      }
      return e;
}

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

_Bool streq(char* x, char* y){
      return !strcmp(x, y);
}

/*void insert_shash(struct shash* h, char* tag){*/
struct sh_entry* insert_shash(struct shash* h, char** cur_path, int cur_depth, char* data){
      (void)data;
      /*
       * this whole function needs to be broken up
       * there should be an abstract function that just inserts an item into hash
       * this will call that often recurseivly
      */
      
      int ind;
      /*_Bool eq;*/

      char** _cur_path = cur_path;
      int _cur_depth = cur_depth;

      struct shash* sub_h = h;
      /* e is declared here to have a convenient pointer to our sh_entry
       * so that we can insert data
       */
      struct sh_entry* e = NULL;
      for(int i = 0; i < _cur_depth; ++i){
            ind = (*_cur_path[i])%h->nbux; 

            if(!sub_h->entries[ind]){
                  e = sub_h->entries[ind] = calloc(1, sizeof(struct sh_entry));
                  e->subhash = calloc(1, sizeof(struct shash));
                  init_shash(e->subhash);

                  /* if we've just initialized bucket, insert and get out */
                  memcpy(e->tag, _cur_path[i], 100);
                  sub_h = e->subhash;

                  --_cur_depth;
                  ++_cur_path;
                  --i;
                  /*if(i == _cur_depth)return;*/
                  continue;
            }

            /* otherwise, we still need to find the last index of linked list */
            /* TODO: keep a last pointer */
            /*each bucket has a linked list*/
            e = sub_h->entries[ind];

            for(; e->next; e = e->next){

                  if(streq(cur_path[i], e->tag)){
                  /* if cur_path[i] is already in current tag, increment sub_h, continue */
                        /*sub_h = e->next->subhash;*/
                        sub_h = e->subhash;
                        /*continue;*/
                        break;
                   }
            }
            /* is this only true if we've found tag?
             * otherwise we can just set a flag if streq()
             * and replace this condition
             */
            /* we're allowing duplicates in the lowest depth */
            if(sub_h == e->subhash && i < cur_depth-1)continue;

            /* this shouldn't be reached if we've found a bucket */
            e->next = calloc(1, sizeof(struct sh_entry));
            e->next->subhash = calloc(1, sizeof(struct shash));
            init_shash(e->next->subhash);
            memcpy(e->next->tag, _cur_path[i], 100);

            sub_h = e->next->subhash;
            /* ensuring e is pointing to our newly created entry */
            e = e->next;
      }
      return e;
}

/* index */
struct sh_entry* ind_shash(struct shash* h, char** path, int depth, int index){
      struct shash* hh = h;
      int ind;
      /*_Bool found;*/
      int found;
      struct sh_entry* e = NULL;
      for(int i = 0; i < depth; ++i){
            ind = *path[i]%hh->nbux;
            if(!hh->entries[ind])return NULL;
            found = 0;
            for(e = hh->entries[ind]; e; e = e->next){
                  if(streq(e->tag, path[i])){
                        hh = e->subhash;
                        ++found;
                        if(i < depth-1 || found == index+1)
                        break;
                  }
            }
            if(!found)return NULL;
      }
      /* by now, hh should contain the appropriate depth */
      return e;
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

_Bool strtoi(const char* str, int* i){
     char* res;
     unsigned int r = strtol(str, &res, 10);
     if(*res)return 0;
     *i = (int)r;
     return 1;
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

      #if 0
      for(int i = 0; i < subcalls; ++i){
            printf("subcall: %i ind: %i\n", i, call_ind[i]);
            for(int j = 0; j < call_depth[i]; ++j){
                  printf("  %s\n", calls[i][j]);
            }
      }
      #endif
      struct shash* hh = h;
      struct sh_entry* e;
      for(int i = 0; i < subcalls; ++i){
            /*printf("ind_shash(hh, calls[i], )");*/
            e = ind_shash(hh, calls[i], call_depth[i], call_ind[i]);

            if(!e)return NULL;
            hh = e->subhash;
      }
      return e;
}

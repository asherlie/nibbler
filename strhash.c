#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strhash.h"

void init_shash(struct shash* h){
      h->nbux = 251;
      h->entries = calloc(h->nbux, sizeof(struct sh_entry));
}

_Bool streq(char* x, char* y){
      return !strcmp(x, y);
}

/*void insert_shash(struct shash* h, char* tag){*/
void insert_shash(struct shash* h, char** cur_path, int cur_depth, char* data){
      /* TODO: insert data */
      (void)data;

/*
 * this whole function needs to be broken up
 * there should be an abstract function that just inserts an item into hash
 * this will call that often recurseivly or some shit
*/
      
      int ind;
      /*_Bool eq;*/

      char** _cur_path = cur_path;
      int _cur_depth = cur_depth;

      struct shash* sub_h = h;
      for(int i = 0; i < _cur_depth; ++i){
            ind = (*_cur_path[i])%h->nbux; 

            /*printf("hashing %c, checking mod %i\n", *_cur_path[i], ind);*/
            /*
             * printf("checking if bucket \"%s\" exists in h: ", _cur_path[i]);
             * for(int x = 0; x < i; ++x){
             *       printf("%s/", _cur_path[x]);
             * }
             * printf("\\...");
            */
            if(!sub_h->entries[ind]){
                  /*puts("doesn't");*/
                  /*printf("inserted bucket %s\n", cur_path[i]);*/
                  sub_h->entries[ind] = calloc(1, sizeof(struct sh_entry));
                  sub_h->entries[ind]->subhash = calloc(1, sizeof(struct shash));
                  init_shash(sub_h->entries[ind]->subhash);

                  /* if we've just initialized bucket, insert and get out */
                  memcpy(sub_h->entries[ind]->tag, _cur_path[i], 100);
                  sub_h = sub_h->entries[ind]->subhash;

                  --_cur_depth;
                  ++_cur_path;
                  continue;
            }
            /*else puts("DOES");*/

            /* otherwise, we still need to find the last index of linked list */
            /* TODO: keep a last pointer */
            /*each bucket has a linked list*/
            struct sh_entry* e = sub_h->entries[ind];

            /* recurse */
            /*sub_h = sub_h->entries[ind]->subhash;*/

            /* e is initialized to NULL with calloc */
            /*if(!e->)*/

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
            if(sub_h == e->subhash)continue;

            /* this shouldn't be reached if we've found a bucket */
            e->next = calloc(1, sizeof(struct sh_entry));
            e->next->subhash = calloc(1, sizeof(struct shash));
            init_shash(e->next->subhash);
            memcpy(e->next->tag, _cur_path[i], 100);

            sub_h = e->next->subhash;
            /*printf("found %p->%p\n", (void*)e, (void*)e->next);*/
            /*
             * if(!eq){
             *       e->next = calloc(sizeof(gt));
             * }
            */

      }
      /* TODO: insert data somewhere in this final hash */
      /*sub_h->*/
      
      /* tag will be implicitly added at this point */

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

struct shash* ind_shash(struct shash* h, char** path, int depth){
/*struct strhash* ind_shash(struct shash* h, char** path, int depth){*/
/*
 *       struct shash* hh = h;
 *       int ind;
 *       for(char* i = *path; *i; ++i){
 *             printf("first char: %c\n", *i);
 *             ind = *i%hh->nbux;
 *             if(!hh->entries[ind])return NULL;
 *             struct sh_entry* e;
 *             for(e = hh->entries[ind]; e; e = e->next){
 *                   printf("comparing %s and %s\n", i, e->tag);
 *                   if(streq(e->tag, i)){
 *                         hh = e->subhash;
 *                         break;
 *                   }
 *             }
 *       }
 * 
*/
      struct shash* hh = h;
      int ind;
      _Bool found;
      for(int i = 0; i < depth; ++i){
            ind = *path[i]%hh->nbux;
            if(!hh->entries[ind])return NULL;
            struct sh_entry* e;
            found = 0;
            for(e = hh->entries[ind]; e; e = e->next){
                  printf("comparing %s and %s\n", path[i], e->tag);
                  if(streq(e->tag, path[i])){
                        hh = e->subhash;
                        found = 1;
                        break;
                  }
            }
            if(!found)return NULL;
      }
      /* by now, hh should contain the appropriate depth */
      return hh;
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

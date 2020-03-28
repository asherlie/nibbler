#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tagger.h"

void taggem(struct shash* h, struct web_page* w){
      char tag[100] = {0};
      _Bool in_tag = 0;
      int t_ind = 0;
      struct shash* current = h;
      for(unsigned int i = 0; i < w->bytes; ++i){
            char c = w->data[i];
            /* start tag */
            if(c == '<'){
                  in_tag = 1;
                  t_ind = 0;
                  memset(tag, 0, 100);
            }
            else if(c == '>'){
                  if(in_tag){
                        in_tag = 0;
                        if(tag[t_ind-1] == '/')continue;

                        /* current must be set to parent shash */
                        if(tag[0] == '/'){
                              current = current->parent;
                              continue;
                        }

                        /* inserting a tag into current->entries */
                        printf("tag: \"%21s\" @ %p\n", tag, (void*)current);
                        /*if(!current)puts("ANTICIPATED");*/

                        int bucket = (tag[1])%current->nbux;
                        /* if bucket doesn't exist, create it */
                        if(!current->entries[bucket]){
                              current->entries[bucket] = calloc(1, sizeof(struct sh_entry));
                              memcpy(current->entries[bucket]->tag, tag, t_ind);
                              init_shash(current->entries[bucket]->subhash = malloc(sizeof(struct shash)));
                              current->entries[bucket]->last = current->entries[bucket];
                              current->entries[bucket]->last->next = NULL;
                              /*current->last = current->entries[bucket];*/
                              /*current->entries[bucket]->subhash->last = ;*/
                              /*current->entries[bucket]->subhash->*/
                        }
                        /* otherwise, insert in back of LL */
                        else{
                              /* abstract this */
                              current->entries[bucket]->last->next = calloc(1, sizeof(struct sh_entry));
                              current->entries[bucket]->last = current->entries[bucket]->last->next;
                              /*current->entries[bucket]->last->next = NULL;*/
                              memcpy(current->entries[bucket]->last->tag, tag, t_ind);
                              init_shash(current->entries[bucket]->last->subhash = malloc(sizeof(struct shash)));

                        }

                        /* updating current */
                        /*
                         * current->entries[bucket]->subhash->parent = current;
                         * current = current->entries[bucket]->subhash;
                        */

                        current->entries[bucket]->last->subhash->parent = current;
                        current = current->entries[bucket]->last->subhash;

                        /*printf("bucket %i\n", bucket);*/
                        /*current->entries[];*/
                        /* tag end */
                  }
            }
            else if(in_tag){
                  tag[t_ind++] = c;
            }
      }
}

_Bool tag_page(struct shash* h, struct web_page* w){
      char* raw_page = w->data;
      size_t strlen = w->bytes;
      /*
       * FILE* fp = fopen("ex", "r");
      */
      char c;

      /* keeping track of size and capacity of tag and data, respectively */
      int t_ind = 0, t_cap = 100,
          d_ind = 0, d_cap = 500;

      size_t str_off = 0;

      /* tag can be malloc'd because it is memset() every iteration,
       * data can be malloc'd because it is memcpy'd only to the point
       * of d_ind each iteration
       */
      char* tag = malloc(sizeof(char)*t_cap), * data = malloc(sizeof(char)*d_cap);

      /* int depth refers to current depth in tags */
      /* char** cur_path stores a list of tag strings that lead to current */
      int depth = 0;
      /* TODO dynamically resize cur_path */
      char** cur_path = malloc(sizeof(char*)*500);

      /* no need to pre-alloc these - they can be allocated
       * to the perfect size immediately before memcpying
       */
      /*for(int i = 0; i < 500; ++i)cur_path[i] = calloc(1, t_);*/

      /* return values from insert_shash() are stored in e 
       * this allows us to insert data into the previous
       * entry once a '/' is detected
       */
      struct sh_entry* e = NULL;

      while(str_off < strlen){
            c = raw_page[str_off++];
            if(c == '<'){
                  if(raw_page[str_off] == '!')continue;
                  t_ind = 0;
                  /* there's no need to zero tag if t_ind is set
                   * to zero before copying
                   */
                  /*memset(tag, 0, t_cap);*/

                  while((c = raw_page[str_off++]) != '>'){
                        if(t_ind == t_cap){
                              t_cap *= 2;
                              char* tmp = malloc(sizeof(char)*t_cap);
                              memcpy(tmp, tag, t_ind);
                              free(tag);
                              tag = tmp;
                        }
                        tag[t_ind++] = c;

                  }
                  /* self contained tags are skipped for now */
                  if(tag[t_ind-1] == '/'){
                        /* TODO: possibly insert this tag with the internal
                         *       text as data segment 
                         */
                        /* insert_shash(h, ); */
                        continue;
                  }

                  if(*tag != '/'){
                        cur_path[depth] = malloc(t_ind+1);
                        memcpy(cur_path[depth++], tag, t_ind);
                        cur_path[depth-1][t_ind] = 0;
                  }
                  else{
                        if(--depth < 0)return 0;
                        /* if we have data to add */
                        /*
                         * data must be added after the '/' has been found
                         * otherwise, we don't have the data 
                         */
                        /*if(d_ind && e){*/
                        if(d_ind){
                              memcpy(e->data, data, d_ind);
                              e->data[d_ind] = 0;
                              d_ind = 0;
                        }
                  }

                  e = insert_shash(h, cur_path, depth, NULL);
            }
            else{
                  if(d_ind == d_cap){
                        d_cap *= 2;
                        char* tmp = malloc(sizeof(char)*d_cap);
                        memcpy(tmp, data, d_ind);
                        free(data);
                        data = tmp;
                  }
                  data[d_ind++] = c;
            }
      }
      return 1;
}

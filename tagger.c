#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tagger.h"

void taggem(struct shash* h, struct web_page* w){
      /* TODO: get rid of 1500 sz buffer */
      char tag[100] = {0}, data[1500] = {0}, * pdata;
      _Bool in_tag = 0;
      int t_ind = 0, d_ind = 0;
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
                              /*printf("data: \"%s\"\n", data);*/
                              pdata = malloc(d_ind);
                              memcpy(pdata, data, d_ind);
                              memset(data, 0, d_ind);
                              d_ind = 0;
                              current = current->parent;
                              continue;
                        }

                        /* inserting a tag into current->entries */
                        /*printf("tag: \"%s\" @ %p\n", tag, (void*)current);*/
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
                        pdata = current->entries[bucket]->last->data;

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
            /* if this is the first char of data and is WS, ignore */
            else if(d_ind || (c != ' ' && c != '\n')){
                  data[d_ind++] = c;
            }
      }
}

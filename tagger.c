#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tagger.h"

void taggem(struct shash* h, struct web_page* w, _Bool strip_tags){
      int t_ind = 0, d_ind = 0, t_cap = 100, d_cap = 500;
      char* tag = calloc(1, t_cap), * data = calloc(1, d_cap), ** pdata;
      _Bool in_tag = 0;
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
                        if(tag[0] == '!' || tag[t_ind-1] == '/')continue;

                        /* current must be set to parent shash */
                        if(tag[0] == '/'){
                              /*printf("%s: %s\n", tag, data);*/
                              if(pdata){
                                    *pdata = data;
                                    pdata = 0;
                              }
                              /*printf("setting %p to (%s)\n", pdata, data);*/
                              data = calloc(1, d_cap);
                              d_ind = 0;
                              current = current->parent;
                              continue;
                        }

                        if(strip_tags){
                              char* sp = strchr(tag, ' ');
                              if(sp)*sp = 0;
                        }

                        if(strip_tags && tag[t_ind-1] == ' ')tag[--t_ind] = 0;
                        /*printf("tag: %s\n", tag);*/

                        /* inserting a tag into current->entries */
                        int bucket = (tag[1])%current->nbux;
                        /* if bucket doesn't exist, create it */
                        if(!current->entries[bucket]){
                              current->entries[bucket] = calloc(1, sizeof(struct sh_entry));
                              /*memcpy(current->entries[bucket]->tag, tag, t_ind);*/
                              current->entries[bucket]->tag = tag;
                              /*printf("(%s) - ", tag);*/
                              tag = calloc(1, t_cap);
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
                              /*memcpy(current->entries[bucket]->last->tag, tag, t_ind);*/
                              current->entries[bucket]->last->tag = tag;
                              /*printf("(%s) - ", tag);*/
                              tag = calloc(1, t_cap);
                              init_shash(current->entries[bucket]->last->subhash = malloc(sizeof(struct shash)));

                        }

                        pdata = &current->entries[bucket]->last->data;

                        /* updating current */
                        current->entries[bucket]->last->subhash->parent = current;
                        current = current->entries[bucket]->last->subhash;
                  }
            }
            else if(in_tag){
                  if(t_ind == t_cap){
                        t_cap *= 2;
                        char* tmp_tag = calloc(1, t_cap);
                        memcpy(tmp_tag, tag, t_ind);
                        free(tag);
                        tag = tmp_tag;
                  }
                  tag[t_ind++] = c;
            }
            /* if this is the first char of data and is WS, ignore */
            else if(d_ind || (c != ' ' && c != '\n')){
                  if(d_ind == d_cap){
                        d_cap *= 2;
                        char* tmp_dat = calloc(1, d_cap);
                        memcpy(tmp_dat, data, d_ind);
                        free(data);
                        data = tmp_dat;
                  }
                  data[d_ind++] = c;
            }
      }
}

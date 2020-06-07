#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tagger.h"

_Bool self_closing(char* tag){
      return
            !strcasecmp(tag, "area") ||
            !strcasecmp(tag, "base") ||
            !strcasecmp(tag, "br") ||
            !strcasecmp(tag, "col") ||
            !strcasecmp(tag, "embed") ||
            !strcasecmp(tag, "hr") ||
            !strcasecmp(tag, "img") ||
            !strcasecmp(tag, "input") ||
            !strcasecmp(tag, "link") ||
            !strcasecmp(tag, "meta") ||
            !strcasecmp(tag, "param") ||
            !strcasecmp(tag, "source") ||
            !strcasecmp(tag, "track") ||
            !strcasecmp(tag, "wbr");
}

/* TODO: add support for self closing tags without slashes at the end
 * list here: http://xahlee.info/js/html5_non-closing_tag.html
 * as of now they are NOT supported
 */ 
/*we must be able to detect invalid html files*/
void taggem(struct shash* h, struct web_page* w, _Bool strip_tags, _Bool enforce_lowcase){
      int t_ind = 0, d_ind = 0, t_cap = 100, d_cap = 500;
      char* tag = calloc(1, t_cap), * data = calloc(1, d_cap), ** pdata = NULL;
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
                        /* we separate this from self_closing() because these checks
                         * do not require stripped tags
                         */
                        if(tag[0] == '!' || tag[t_ind-1] == '/')continue;

                        /* current must be set to parent shash */
                        if(tag[0] == '/'){
                              /*printf("%s: %s\n", tag, data);*/
                              if(pdata){
                                    #if !1
                                    for some reason data is empty string when title is added
                                    for https://en.wikipedia.org/wiki/Shor%27s_algorithm
                                    title should be "Shor's algorithm - Wikipedia"
                                    printf("set title of \"%s\" to: \"%s\"\n", tag, data);
                                    #endif
                                    /*if(data[0] == 'S' && data[17] == '-')puts("FOOO");*/
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

                        if(self_closing(tag))continue;

                        /*printf("tag: %s\n", tag);*/

                        /* inserting a tag into current->entries */
                        /*int bucket = (tag[1])%current->nbux;*/
                        int bucket = ((enforce_lowcase && (tag[1] >= 'A' && tag[1] <= 'Z')) ? tag[1]+32 : tag[1])%current->nbux;
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

                              /* ??? */
                              pdata = &current->entries[bucket]->data;
                              current->entries[bucket]->subhash->parent = current;
                              current = current->entries[bucket]->subhash;
                              /*current->last = current->entries[bucket];*/
                              /*current->entries[bucket]->subhash->last = ;*/
                              /*current->entries[bucket]->subhash->*/
                        }
                        /* otherwise, insert in back of LL */
                        /*
                         *ok don't put it in back - it really should go in the entry
                         *with the proper tag IF IT EVEN EXISTS
                         */
                        else{
                              /* abstract this */
                              // how do we handle indices [0] :)
                              struct sh_entry* last_tag = NULL;
                              for(struct sh_entry* sh = current->entries[bucket]; sh; sh = sh->next){
                                    if(!strcasecmp(sh->tag, tag)){
                                        last_tag = sh;
                                        while(last_tag){
                                            if(!last_tag->next || strcasecmp(last_tag->next->tag, tag)){
                                                goto FOUND;
                                            }
                                            last_tag = last_tag->next;
                                        }
                                    }
                              }
                              FOUND:;
                              #if !1
                              current->entries[bucket]->last->next = calloc(1, sizeof(struct sh_entry));
                              current->entries[bucket]->last = current->entries[bucket]->last->next;
                              /*current->entries[bucket]->last->next = NULL;*/
                              /*memcpy(current->entries[bucket]->last->tag, tag, t_ind);*/
                              current->entries[bucket]->last->tag = tag;
                              /*printf("(%s) - ", tag);*/
                              tag = calloc(1, t_cap);
                              init_shash(current->entries[bucket]->last->subhash = malloc(sizeof(struct shash)));
                              #endif

                              if(!last_tag)last_tag = current->entries[bucket]->last;
                              struct sh_entry* tmp = calloc(1, sizeof(struct sh_entry));
                              tmp->tag = tag;
                              tag = calloc(1, t_cap);
                              tmp->next = last_tag->next;
                              init_shash(tmp->subhash = malloc(sizeof(struct shash)));
                              last_tag->next = tmp;
                              if(last_tag == current->entries[bucket]->last)current->entries[bucket]->last = last_tag->next;

                              /* ??? */
                              pdata = &last_tag->next->data;
                              last_tag->next->subhash->parent = current;
                              current = last_tag->next->subhash;
                        }

                        #if !1
                        ok... this last business is incorrect - in the next chunk of code too... bucket->last can be
                        a completely different tag that just has the same second char
                        above too... fuck. this is gonna be a big bugfix
                        #endif
                        /*pdata = &current->entries[bucket]->last->data;*/
                        /*printf("setting pdata to %s\n", current->parent->entries[bucket]->last);*/
                        #if !1
                        pdata = &current->parent->entries[bucket]->last->data;
                        #endif
                        /*pdata = &current->parent- */
                        /*pdata = current->parent->parent->entries[bucket]->*/

                        /* updating current */
                        /*current->entries[bucket]->last->subhash->parent = current;*/
                        /*current = current->entries[bucket]->last->subhash;*/
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

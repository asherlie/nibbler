#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tagger.h"

_Bool tag_page(struct shash* h, struct web_page* w){
      char* raw_page = w->data;
      size_t strlen = w->bytes;
      /*
       * FILE* fp = fopen("ex", "r");
      */
      char c;

      /*
       * struct shash h;
       * init_shash(&h);
      */

      /* TODO: insert contents of tags in strhash */
      char str[2000] = {0};
      (void)str;

      int ind = 0, d_ind = 0;
      size_t str_off = 0;
      char tag[100], data[500];

      /* int depth refers to current depth in tags */
      /* char** cur_path stores a list of tag strings that lead to current */
      /* TODO: dynamically resize both tag and data if necessary */
      int depth = 0;
      char** cur_path = malloc(sizeof(char*)*500);
      for(int i = 0; i < 500; ++i)cur_path[i] = calloc(1, 100);

      /* return values from insert_shash() are stored in e 
       * this allows us to insert data into the previous
       * entry once a '/' is detected
       */
      struct sh_entry* e = NULL;

      while(str_off < strlen){
            c = raw_page[str_off++];
            if(c == '<'){
                  if(raw_page[str_off] == '!')continue;
                  ind = 0;
                  memset(tag, 0, 100);

                  while((c = raw_page[str_off++]) != '>')
                        tag[ind++] = c;

                  /* self contained tags are skipped for now */
                  if(tag[ind-1] == '/'){
                        /* TODO: possibly insert this tag with the internal
                         *       text as data segment 
                         */
                        /* insert_shash(h, ); */
                        continue;
                  }

                  if(*tag != '/'){
                        /*printf("");*/
                        memcpy(cur_path[depth++], tag, ind);
                        cur_path[depth-1][ind] = 0;
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

                  /*
                   * printf("\ndepth: %i\n", depth);
                   * for(int i = 0; i < depth; ++i)printf("%s, ", cur_path[i]);
                  */

                  /* tag is memcpy'd, no need to throw on heap */
                  /* adding NUL char to data */
                  /*data[d_ind] = 0;*/
                  /*
                   *printf("inserting shash cur tag: %s data: %s in tag ", tag, data);
                   *for(int i = 0; i < depth; ++i)printf("%s, ", cur_path[i]);
                   *puts("");
                   */
                  e = insert_shash(h, cur_path, depth, data);
                  /*d_ind = 0;*/
            }
            else data[d_ind++] = c;
      }
      return 1;
      /*printf("%i\n", in_tag);*/
}

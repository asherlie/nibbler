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
      int in_tag = 0;
      char c;

      /*
       * struct shash h;
       * init_shash(&h);
      */

      /* TODO: insert contents of tags in strhash */
      char str[2000] = {0};
      (void)str;

      int ind = 0;
      size_t str_off = 0;
      char tag[100];

      /* int depth refers to current depth in tags */
      /* char** cur_path stores a list of tag strings that lead to current */
      /* TODO: dynamically resize */
      int depth = 0;
      char** cur_path = malloc(sizeof(char*)*500);
      for(int i = 0; i < 500; ++i)cur_path[i] = calloc(1, 100);

      while(str_off < strlen){
            c = raw_page[str_off++];
      /*while((c = fgetc(fp)) != EOF){*/
            if(c == '<'){
                  if(raw_page[str_off] == '!')continue;
                  ind = 0;
                  memset(tag, 0, 100);

                  ++in_tag;
                  /*while((c = fgetc(fp)) != '>'){*/
                  while((c = raw_page[str_off++]) != '>'){
                  /*while((c = fgetc(fp)) != '>'){*/
                        tag[ind++] = c;
                  }

                  /* self contained tags are skipped for now */
                  if(tag[ind-1] == '/')continue;

                  if(*tag != '/'){
                        memcpy(cur_path[depth++], tag, ind);
                        cur_path[depth-1][ind] = 0;
                  }
                  else if(--depth < 0)return 0;


                  /*
                   * printf("\ndepth: %i\n", depth);
                   * for(int i = 0; i < depth; ++i)printf("%s, ", cur_path[i]);
                  */

                  /* tag is memcpy'd, no need to throw on heap */
                  insert_shash(h, cur_path, depth, "some data");
                  /*printf("%i: tag in prog: %s\n", in_tag, tag);*/
            }
            if(c == '>'){
            /*we pop up a level of interest in hash struct*/
                  --in_tag;
            }
      }
      return 1;
      /*printf("%i\n", in_tag);*/
}

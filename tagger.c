#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tagger.h"

void tag_page(struct shash* h, struct web_page* w){
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
                  ind = 0;
                  memset(tag, 0, 100);

                  ++in_tag;
                  /*while((c = fgetc(fp)) != '>'){*/
                  while((c = raw_page[str_off++]) != '>'){
                  /*while((c = fgetc(fp)) != '>'){*/
                        tag[ind++] = c;
                  }
                  /* self contained tags get inserted as a final step */
                  if(tag[ind-1] == '/'){
                  }
                  if(*tag != '/')memcpy(cur_path[depth++], tag, 100);
                  else --depth;

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
      printf("%i\n", in_tag);
}

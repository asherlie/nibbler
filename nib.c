#include <stdlib.h>
#include <curl/curl.h>

#include "strhash.h"
/*#include "tagger.h"*/
#include "dl.h"

/* TODO:
 * write process_web_page(struct web_page* w, void* k)
 */
int main(int a, char** b){
      if(a < 2)return 1;
      curl_global_init(CURL_GLOBAL_ALL);
      struct web_page* w = dl_pages(b+1, 1);
      struct shash h;
      init_shash(&h);

      /* TODO: tagging of multiple pages should be done in parallel */
      tag_page(&h, w);

      if(a < 3)return 0;
      struct shash* found = ind_shash(&h, b+2, 1);
      if(!found)puts("didn't find");
      else puts("found");

      curl_global_cleanup();

      return 0;
}

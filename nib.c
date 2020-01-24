#include <stdlib.h>
#include <curl/curl.h>

#include "strhash.h"
/*#include "tagger.h"*/
#include "dl.h"
#include <time.h>

/* TODO:
 * write process_web_page(struct web_page* w, void* k)
 */
/* TODO: test this benchmark speed test on ub server - will more threads make a big difference?
 * i'd like to have 100 {downloads, tags, parses} finished in under 1 second
 * oh wait, this isn't even representative - dl_pages takes in many pages
 * test this out with dl_pages taking in 100 at once to see if it's significantly faster
 * than expected
 * .015 for one page is current benchmark
 */
int main(int a, char** b){
      if(a < 2)return 1;
      curl_global_init(CURL_GLOBAL_ALL);
      clock_t st = clock();
      /*struct web_page* w = dl_pages(b+1, 1);*/
      char* pages[] = {
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com",
      "example.com", "example.com", "example.com", "example.com", "example.com"
      };
      struct shash* w = dl_pages(pages, 100);
      (void)w;
      double el0 = ((double)clock()-st)/CLOCKS_PER_SEC;
      printf("dl took %lf\n", el0);
      struct shash h[100];
      for(int i = 0; i < 100; ++i)
            init_shash(h+i);

      #if 0
      /* TODO: tagging of multiple pages should be done in parallel - write tag_pages() */
      st = clock();
      for(int i = 0; i < 100; ++i){
            if(!tag_page(h+i, w)){
                  
                  puts("malformed page");
                  /*return EXIT_FAILURE;*/
            }
      }
      #endif
      double el1 = ((double)clock()-st)/CLOCKS_PER_SEC;
      printf("tagging took %lf\n", el1);

      if(a < 3)return 0;
      struct shash* found;
      /*struct shash* found = ind_shash(&h, b+2, a-2);*/
      st = clock();
      for(int i = 0; i < 100; ++i){
            found = ind_shash(h+i, b+2, a-2);
      }
      double el2 = ((double)clock()-st)/CLOCKS_PER_SEC;
      printf("ind_shash took %lf\nall computation took %lf seconds\n", el2, el0+el1+el2);
      if(!found)puts("didn't find");
      else puts("found");

      curl_global_cleanup();

      return 0;
}

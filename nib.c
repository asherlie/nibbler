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
      clock_t st = clock();
      /*struct shash* w = dl_pages(pages, 100);*/
      struct shash* w = dl_pages(pages, 1);
      double el0 = ((double)clock()-st)/CLOCKS_PER_SEC;
      printf("dl and tagging took %lf\n", el0);

      if(a < 3)return 0;
      struct shash* found;
      /*struct shash* found = ind_shash(&h, b+2, a-2);*/
      st = clock();
      /*for(int i = 0; i < 100; ++i){*/
      for(int i = 0; i < 1; ++i){
            found = ind_shash(w+i, b+2, a-2);
      }
      double el1 = ((double)clock()-st)/CLOCKS_PER_SEC;
      printf("ind_shash took %lf\nall computation took %lf seconds\n", el1, el0+el1);
      if(!found)puts("didn't find");
      else puts("found");

      curl_global_cleanup();

      return 0;
}

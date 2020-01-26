#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <time.h>

#include "dl.h"
#include "strhash.h"

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
      curl_global_init(CURL_GLOBAL_ALL);

      int npages = 500;

      char* pages[npages];
      memset(pages, 0, npages);

      char ex[] = "example.com";
      for(int i = 0; i < npages; ++i)
            pages[i] = ex;
      /*
       * use of clock() was giving inaccurate timings due to multiple threads using more
       * cpu time at once
      */
      printf("attempting to download %i pages\n", npages);
      struct timespec st, fin;
      clock_gettime(CLOCK_MONOTONIC, &st);
      struct shash* w = dl_pages(pages, npages, 1);
      clock_gettime(CLOCK_MONOTONIC, &fin);

      double el0 = fin.tv_sec - st.tv_sec;
      el0 += (fin.tv_nsec-st.tv_nsec)/1000000000.0;

      int tries = 0;
      for(int i = 0; i < npages; ++i){
            tries += w[i].retries_used;
      }

      printf("dl and tagging took %lf with %i retries\n", el0, tries);

      if(a < 2)return 0;

      clock_gettime(CLOCK_MONOTONIC, &fin);

      int fail = 0, found = 0;
      for(int i = 0; i < npages; ++i){
            if(!w[i].entries)++fail;
            found += (_Bool)ind_shash(w+i, b+2, a-2);
      }
      clock_gettime(CLOCK_MONOTONIC, &fin);

      double el1 = fin.tv_sec - st.tv_sec;
      el1 += (fin.tv_nsec-st.tv_nsec)/1000000000.0;

      printf("%i/%i malformed pages\n", fail, npages);
      printf("found: %i/%i\n", found, npages);
      printf("ind_shash took %lf\nall computation took %lf seconds\n", el1, el0+el1);
      if(!found)puts("didn't find");
      else puts("found");

      curl_global_cleanup();

      return 0;
}

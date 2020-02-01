#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <time.h>

#include "dl.h"
#include "strhash.h"

void test(){
      FILE* fp = fopen("ex", "r");
      char ex[1257] = {0};
      int ind = 0;
      while((ex[ind++] = fgetc(fp)) != EOF);

      struct web_page w;
      w.bytes = 1256;
      w.data = ex;

      struct shash h;
      init_shash(&h);

      if(!tag_page(&h, &w))puts("failed to tag");


      struct sh_entry* e;

      {
      char* path[] = {"html", "head"};

      if(!(e = ind_shash(&h, path, 2)))puts("failed to find hh");
      /*else printf("tag: %s, data: %s\n", e->tag, e->data);*/
      }

      {
      char* path[] = {"html", "body", "div", "h1"};

      if(!(e = ind_shash(&h, path, 4)))puts("failed to find hb");
      /*else printf("tag: %s, data: %s\n", e->tag, e->data);*/
      }
}

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
      test();
      return 0;

      curl_global_init(CURL_GLOBAL_ALL);

      int npages = 1;

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

      int tries = 0, failures = 0;
      for(int i = 0; i < npages; ++i){
            tries += w[i].retries_used;
            if(!w[i].entries)++failures;
      }

      printf("dl and tagging took %lf with %i retries\n", el0, tries);
      printf("%i/%i malformed pages\n", failures, npages);

      if(a < 2)return 0;

      clock_gettime(CLOCK_MONOTONIC, &st);

      int found = 0;
      for(int i = 0; i < npages; ++i)
            found += (_Bool)ind_shash(w+i, b+1, a-1);
      clock_gettime(CLOCK_MONOTONIC, &fin);

      double el1 = fin.tv_sec - st.tv_sec;
      el1 += (fin.tv_nsec-st.tv_nsec)/1000000000.0;

      printf("found: %i/%i\n", found, npages);
      printf("ind_shash took %lf\nall computation took %lf seconds\n", el1, el0+el1);
      if(!found)puts("didn't find");
      else puts("found");

      curl_global_cleanup();

      return 0;
}

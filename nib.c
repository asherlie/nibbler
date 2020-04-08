#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <time.h>

#include "dl.h"
#include "strhash.h"

char* strip_ws(char* str){
      char* ret;
      for(ret = str; *ret && (*ret == '\n' || *ret == ' '); ++ret);
      return ret;
}

/*
 *we can have a function that calls print()
 *that sets up an arg print takes for the list of tags we're in
 */

void _find_paths(struct sh_entry* e, char** path, int depth, int index, int ip){
      if(!e)return;
      int _depth = depth;
      if(e->data){
            /*for(int i = 0; i < depth-ip; ++i)printf("\"%s\", ", path[i]);*/
            /*printf("%i -> %i\n", 0, ip);*/
            for(int i = 0; i < ip; ++i)printf("\"%s\", ", path[i]);
            if(index)printf("%i, ", index);
            /*printf("%i -> %i\n", ip, depth);*/
            for(int i = ip; i < depth; ++i)printf("\"%s\", ", path[i]);
            /*for(int i = 0; i < depth; ++i)printf("\"%s\", ", path[i]);*/
            printf("\"%s\": \"%s\"\n", e->tag, e->data);
      }
      else path[_depth++] = e->tag;
      if(e->next)_find_paths(e->next, path, _depth, index+1, ip);
      for(int i = 0; i < e->subhash->nbux; ++i){
            _find_paths(e->subhash->entries[i], path, _depth, index, ip+1);
      }
}
void find_paths(struct shash* h){
      char* buf[1000];
      struct sh_entry E;
      E.data = 0;
      char s_tag[] = "**BEGIN**";
      E.tag = s_tag;
      E.subhash = h;
      _find_paths(&E, buf, 0, 0, 0);
}
void print(struct sh_entry* e){
      if(!e)return;
      if(e->data)printf("%s: \"%s\"\n", e->tag, e->data);
      else printf("%s, ", e->tag);
      if(e->next)print(e->next);
      for(int i = 0; i < e->subhash->nbux; ++i){
            print(e->subhash->entries[i]);
      }
      /*
       *if(e->subhash->entries[])
       *print(e->subhash);
       */
}

void test(int a, char** b){
      /*FILE* fp = fopen("ex", "r");*/
      FILE* fp = fopen("fog", "r");
      /*FILE* fp = fopen("x", "r");*/
      /*FILE* fp = fopen("mal", "r");*/
      /*FILE* fp = fopen("hn", "r");*/
      /*char ex[1700] = {0};*/
      char ex[33981] = {0};
      int ind = 0;
      while((ex[ind++] = fgetc(fp)) != EOF);

      struct web_page w;
      w.bytes = 1256;
      w.bytes = 1379;
      w.data = ex;
      w.bytes = ind;

      struct shash h;
      init_shash(&h);

      taggem(&h, &w, 1, 1);

      find_paths(&h);
      /*print(&E);*/

      /*char* pth[] = {"html", "body", "div", "h1", "1"};*/
      /*char* pth[] = {"html", "body", "div", "p", "1", "a", "0"};*/
      /*struct sh_entry* ee = find_entry(&h, pth, 7);*/
      struct sh_entry* ee = find_entry(&h, b, a);
      if(!ee)puts("failed to find entry");
      else printf("%s: %s\n", ee->tag, ee->data);
}

/*
 *prints all data entries next to their full paths
 *helpful for finding patterns
 */
/*
 *void all_paths_to_data(struct shash* h){
 *      struct shash* hh = h;
 *      [>if(h-><]
 *      hh->entries[i]->subhash;
 *}
 */

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
      test(a-1, b+1);
      return 0;

      if(a < 2)return EXIT_FAILURE;

      curl_global_init(CURL_GLOBAL_ALL);

      int npages = 10;

      char* pages[npages];
      memset(pages, 0, npages);

      /*char ex[] = "example.com";*/
      for(int i = 0; i < npages; ++i)
            pages[i] = b[1];
      /*
       * use of clock() was giving inaccurate timings due to multiple threads using more
       * cpu time at once
      */
      printf("attempting to download %i pages\n", npages);
      struct timespec st, fin;
      clock_gettime(CLOCK_MONOTONIC, &st);
      struct shash* w = dl_pages(pages, npages, 100);
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

      if(a < 3)return 0;

      clock_gettime(CLOCK_MONOTONIC, &st);

      int found = 0;
      struct sh_entry* e[npages];
      for(int i = 0; i < npages; ++i)
            found += (_Bool)(e[i] = find_entry(w+i, b+2, a-2));
      clock_gettime(CLOCK_MONOTONIC, &fin);

      double el1 = fin.tv_sec - st.tv_sec;
      el1 += (fin.tv_nsec-st.tv_nsec)/1000000000.0;

      printf("found: %i/%i\n", found, npages);
      printf("ind_shash took %lf\nall computation took %lf seconds\n", el1, el0+el1);
      for(int i = 0; i < npages; ++i){
            printf("%i: ", i);
            if(!e[i])puts("not found");
            else printf("%s: \"%s\"\n", e[i]->tag, e[i]->data);
      }
      /*
       *if(!found)puts("didn't find");
       *else printf("%s: \"%s\"\n", (*e)->tag, (*e)->data);
       */

      curl_global_cleanup();

      return 0;
}

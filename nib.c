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

struct index_tracker{
      int index, index_pos;
};

/*void _recfp(struct sh_entry* e, char** path, int depth, int index, int index_pos){*/
void _recfp(struct sh_entry* e, char** path, int depth, struct index_tracker* it, int it_sz){
      if(!e)return;
      int _depth = depth, _it_sz = it_sz;
      /*_Bool ident = !strcasecmp(e->nex);*/
      path[_depth++] = e->tag;
      if(e->next){
            _Bool ident = /*!e->data &&*/ !strcasecmp(e->next->tag, path[_depth-1]);
            /*_recfp(e->next, path, _depth-ident, index+ident, index_pos);*/
            if(ident){
                  /*it[_it_sz].index = it[_it_sz-1].index;*/
                  /* increment current index if we're doing some nice iteration */
                  ++it[_it_sz-1].index;
                  _recfp(e->next, path, _depth-ident, it, _it_sz);
                  --it[_it_sz-1].index;
            }

            /* if the index counting for this subpath is done
             * we're going to get started on our next index of it
             */
            else{
                  /* we're assuming that it is already zeroed so
                   * index, index_pos will be 0, 0
                   */
                  /*_recfp(e->next, path, _depth-ident, it, ++_it_sz);*/
                  _recfp(e->next, path, _depth-ident, it, _it_sz+1);
            }

            /*_depth -= ident;*/
      }
      /* if the index counting for this subpath is done */
      else if(it[_it_sz-1].index){
      /*else{*/
            /* ? */
            ++_it_sz;
      }
      for(int i = 0; i < e->subhash->nbux; ++i){
            /*_recfp(e->subhash->entries[i], path, _depth, index, index_pos+1);*/
            if(e->subhash->entries[i])++it[_it_sz-1].index_pos;
            _recfp(e->subhash->entries[i], path, _depth, it, _it_sz);
            if(e->subhash->entries[i])--it[_it_sz-1].index_pos;
            /*--it[_it_sz-1].index_pos;*/
      }
      /*return;*/
      /*ip is always off by one - if */
      if(e->data){
            /*
             *int sum = 0;
             *for(int i = 0; i < _it_sz; ++i){
             *      sum += it[i].index_pos;
             *      [>printf("IT: %i %i - ", it[i].index, it[i].index_pos);<]
             *      printf("IT[%i]: %i %i - ", i, it[i].index, sum);
             *}
             *puts("");
             */
            int it_ind = 0;
            int ip_sum = it[it_ind].index_pos;
            /*for(int i = 0; i < _depth; ++i){*/
            for(int i = 0; i < depth; ++i){
                  /*printf("%i == %i\n", i, it[it_ind].index_pos);*/
                  printf("%s, ", path[i]);
                  /*if(it[it_ind].index && i == it[it_ind].index_pos)printf("%i, ", it[it_ind++].index);*/
                  if(it[it_ind].index && i == ip_sum){
                        printf("%i, ", it[it_ind++].index);
                        ip_sum += it[it_ind].index_pos;
                  }
            }
            printf("%s", e->tag);
            /*if(it[it_ind].index && depth == it[it_ind].index_pos)printf(", %i", it[it_ind].index);*/
            if(it[it_ind].index && depth == ip_sum)printf(", %i", it[it_ind].index);
            /*printf("%s: %s\n", e->tag, e->data);*/
            printf(": %s\n", e->data);
      }
}

void recfp(struct shash* h){
      struct sh_entry E;
      E.subhash = h;
      char start[] = "BEGIN";
      E.data = NULL;
      E.tag = start;
      char* buf[1000];
      struct index_tracker it[1000] = {0};
      _recfp(&E, buf, 0, it, 0);
      return;
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
            printf("(");
            /*for(int i = 0; i < ip; ++i)printf("\"%s\", ", path[i]);*/
            for(int i = 0; i < ip; ++i)printf("%s, ", path[i]);
            if(index)printf("%i, ", index);
            /*printf("%i -> %i\n", ip, depth);*/
            for(int i = ip; i < depth; ++i)printf("%s, ", path[i]);
            /*for(int i = 0; i < depth; ++i)printf("\"%s\", ", path[i]);*/
            /*printf("\"%s\": \"%s\"\n", e->tag, e->data);*/
            printf("%s): \"%s\"\n", e->tag, e->data);
            /* yeah? */
            /*return;*/
      }
      /*
       * many repeats are added because with each ->next we also add to path
       * even though it's the same tag
       * but wait sometimes it's not the same tag
       * it's just in the same second char bucket
       *
       * we could call _find_paths() with _depth-1 if our new path is ident to old
       */
      /*for some reason index or ip is -1*/
      else path[_depth++] = e->tag;
      /*if(e->next)_find_paths(e->next, path, _depth, index+1, ip);*/
      /*_Bool ident = !e->data && !strcasecmp(path[_depth-1], path[_depth-2]);*/
      if(e->next){
            /* if e->next is the same string, index is incremented */
            /*_Bool ident = !e->data && !strcasecmp(path[_depth-1], path[_depth-2]);*/
            _Bool ident = /*!e->data && */!strcasecmp(e->next->tag, path[_depth-1]);
            /*_find_paths(e->next, path, depth-ident, index+ident, ip);*/
            _find_paths(e->next, path, _depth-ident, index+ident, ip);
            /*
             *int adj_d = _depth-ident;
             *int adj_index = index+ident;
             *_find_paths(e->next, path, adj_d, adj_index, ip);
             */
      }
      for(int i = 0; i < e->subhash->nbux; ++i){
            /*_find_paths(e->subhash->entries[i], path, _depth, index, ip+1);*/
            /*_find_paths(e->subhash->entries[i], path, _depth, index, ip+1);*/
            /*if(!i)ip += 1;*/
            _find_paths(e->subhash->entries[i], path, _depth, index, ip+1);
            /*_find_paths(e->subhash->entries[i], path, _depth-ident, index+ident, ip+1);*/
      }
}
void find_paths(struct shash* h){
      char* buf[1000];
      struct sh_entry E;
      E.data = 0;
      char s_tag[] = "**BEGIN**";
      E.tag = s_tag;
      E.subhash = h;
      /*_find_paths(&E, buf, 0, 1, 1);*/
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
      /*FILE* fp = fopen("fog", "r");*/
      /*FILE* fp = fopen("x", "r");*/
      FILE* fp = fopen("ash", "r");
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

      recfp(&h);

      /*find_paths(&h);*/
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

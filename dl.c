#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "dl.h"
/*#include "tagger.h"*/

static size_t write_mem(void* data, size_t sz, size_t mems, void* ptr){
      struct web_page* w = (struct web_page*)ptr;
      size_t _sz = sz*mems;
      /*int bufsz = (_sz > w->bytes*2) ? _sz : w->bytes*2;*/
      if(_sz+w->bytes >= w->cap){
            w->cap = w->cap*2 + _sz + 1;
            w->data = realloc(w->data, w->cap);
      }
      memcpy(w->data+w->bytes+1, data, _sz);
      (w->data+w->bytes+1)[_sz] = 0;
      w->bytes += _sz;
      return _sz;
}

/*data needs to be expanded on, bytes incremented*/
#if 0
static size_t brite_mem(void* data, size_t sz, size_t mems, void* ptr){
      size_t _sz = sz*mems;
      printf("psize: %zu\n", _sz);
      struct web_page* web = (struct web_page*)ptr;

      web->data = malloc(_sz+1);
      memcpy(web->data, data, _sz);
      web->bytes = _sz;
      web->data[_sz] = 0;
      return _sz;
}
#endif

void init_wp(struct web_page* w){
      w->bytes = 0;
      w->cap = 500;
      w->data = calloc(1, w->cap);
}

/* TODO: this should possibly also tag pages */
/*
 *honestly i think that a bunch of segments are being dl'd
 *when summed they're about the size of the proper page
 */
void* dl_page_pth(void* dla_v){
      struct dl_arg* dla = (struct dl_arg*)dla_v;
      struct web_page w;
      init_wp(&w);
      CURL* c = curl_easy_init();

      int tries = 0;
      while(1){
            curl_easy_setopt(c, CURLOPT_URL, dla->url);
            curl_easy_setopt(c, CURLOPT_USERAGENT, "libcurl-agent/1.0");
            curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_mem);
            curl_easy_setopt(c, CURLOPT_WRITEDATA, (void*)&w);

            CURLcode ret = curl_easy_perform(c);
            if(ret != CURLE_OK){
                  if(dla->retries > tries++){
                        ++dla->h->retries_used;
                        puts("contining'");
                        continue;
                  }
                  dla->h->entries = NULL;
                  dla->h->nbux = -1;
                  /* TODO: handle this */
                  return NULL;
            }
            break;
      }

      printf("data: \"%s\" @ %zu, %zu", w.data, w.bytes, w.cap);
      puts(w.data);
      taggem(dla->h, &w, 1, 1);

      return NULL;
}

struct shash* dl_pages(char** urls, int npages, int retries){
      struct dl_arg dla[npages];
      struct shash* ret = malloc(sizeof(struct shash)*npages);
      pthread_t pth[npages];
      for(int i = 0; i < npages; ++i){
            dla[i].url = urls[i];
            dla[i].h = ret+i;
            dla[i].retries = retries;
            init_shash(dla[i].h);
            pthread_create(pth+i, NULL, dl_page_pth, (void*)(dla+i));
      }
      
      #ifdef _GNU_SOURCE
      /* if possible, might as well not dwell on one long download/parse */
      /* this will make more of a difference when downloads will be retried 
       * until succesfully parsed
       */
      int i = 0;
      _Bool joined[npages], ex = 1;
      memset(joined, 0, npages);
      while(1){
            if(i == npages){
                  if(ex)return ret;
                  i = 0;
                  ex = 1;
            }
            if(!joined[i]){
                  if(!pthread_tryjoin_np(pth[i], NULL)){
                        /*printf("joined thread %i\n", i);*/
                        joined[i] = 1;
                  }
                  else ex = 0;
            }
            /*usleep(1000);*/
            ++i;
      }
      #else
      for(int i = 0; i < npages; ++i)
            pthread_join(pth[i], NULL);
      #endif

      return ret;
}

#if 0
struct web_page* dl_page(char** urls, int npages){
      struct web_page* ret = malloc(sizeof(struct web_page)*npages);
      /* TODO: put on heap to accomodate very large npages */
      struct dl_arg dla[npages];
      pthread_t pth[npages];
      for(int i = 0; i < npages; ++i){
            dla[i].url = urls[i];
            dla[i].page = ret+i;
            pthread_create(pth+i, NULL, dl_page_pth, (void*)(dla+i));
      }

      for(int i = 0; i < npages; ++i)
            pthread_join(pth[i], NULL);
      return ret;
}
#endif

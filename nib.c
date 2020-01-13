#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct web_page{
      char* data;
      size_t bytes;
};

struct dl_arg{
      char* url;
      /* dl_page() writes to here */
      struct web_page* page;
};

static size_t write_mem(void* data, size_t sz, size_t mems, void* ptr){
      size_t _sz = sz*mems;
      struct web_page* web = (struct web_page*)ptr;

      web->data = malloc(_sz+1);
      memcpy(web->data, data, _sz);
      web->bytes = _sz;
      web->data[_sz] = 0;
      return _sz;
}

void* dl_page(void* dla_v){
      struct dl_arg* dla = (struct dl_arg*)dla_v;
      CURL* c = curl_easy_init();
      curl_easy_setopt(c, CURLOPT_URL, dla->url);
      curl_easy_setopt(c, CURLOPT_USERAGENT, "libcurl-agent/1.0");
      curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_mem);
      curl_easy_setopt(c, CURLOPT_WRITEDATA, (void*)dla->page);

      CURLcode ret = curl_easy_perform(c);
      if(ret != CURLE_OK){
            puts("UHOH");
            /* TODO: handle this */
            return NULL;
      }
      return NULL;
}

int main(){
      curl_global_init(CURL_GLOBAL_ALL);
      pthread_t pth;
      struct web_page page;
      struct dl_arg dla;
      dla.page = &page;
      pthread_create(&pth, NULL, dl_page, (void*)&dla);
      pthread_join(pth, NULL);
      printf("read %lu b\n", dla.page->bytes);
      puts(dla.page->data);
      curl_global_cleanup();
}

#if 0
our function that returns many web_page objects will
spawn many threads that all write to different sections
of the same buffer
struct web_page pages[nurls];
char* urls[nurls] = {"", ""};
for(int i = 0; i < nurls; ++i){
      // get_page(mem, url) spawns a pthread
      // that writes a struct mem_page to mem
      get_page(pages+i, urls[i])
}
for(int i = 0; i < nurls; ++i){
      pthread_join(...)
}
#endif

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
      /* dl_page_pth() writes to here */
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

void* dl_page_pth(void* dla_v){
      struct dl_arg* dla = (struct dl_arg*)dla_v;
      CURL* c = curl_easy_init();
      curl_easy_setopt(c, CURLOPT_URL, dla->url);
      curl_easy_setopt(c, CURLOPT_USERAGENT, "libcurl-agent/1.0");
      curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_mem);
      curl_easy_setopt(c, CURLOPT_WRITEDATA, (void*)dla->page);

      CURLcode ret = curl_easy_perform(c);
      if(ret != CURLE_OK){
            dla->page->data = NULL;
            dla->page->bytes = 0;
            /* TODO: handle this */
            return NULL;
      }
      return NULL;
}

struct web_page* dl_pages(char** urls, int npages){
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

/* TODO:
 * write process_web_page(struct web_page* w, void* k)
 */
int main(int a, char** b){
      if(a < 2)return 1;
      curl_global_init(CURL_GLOBAL_ALL);
      #if 0
      char* urls[] = {"https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com",
                      "https://www.example.com", "https://www.google.com", "https://www.reddit.com"};
      #endif                
      struct web_page* w = dl_pages(b+1, a-1);
      for(int i = 0; i < a-1; ++i){
            printf("%li bytes\n", w[i].bytes);
            printf("%s\n", w[i].data);
      }
      curl_global_cleanup();
}

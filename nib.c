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
      struct shash h;
      init_shash(&h);
      tag_page(&h, w);
      curl_global_cleanup();
}

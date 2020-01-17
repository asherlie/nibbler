#ifndef _DL
#define _DL
#include <stdint.h>

#include "tagger.h"

struct web_page{
      char* data;
      size_t bytes;
};

struct dl_arg{
      char* url;
      /* dl_page_pth() writes to here */
      struct web_page* page;
};

struct web_page* dl_pages(char** urls, int npages);
#endif

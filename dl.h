#ifndef _DL
#define _DL
#include <stdint.h>

#include "tagger.h"

struct web_page{
      char* data;
      size_t bytes, cap;
};

struct dl_arg{
      int retries;
      char* url;
      struct shash* h;
};

struct shash* dl_pages(char** urls, int npages, int retries);
#endif

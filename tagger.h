#ifndef _TAGGER
#define _TAGGER

#include "dl.h"
#include "strhash.h"
struct web_page;

/* returns 1 on success */
_Bool tag_page(struct shash* h, struct web_page* w);
#endif

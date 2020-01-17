#ifndef _TAGGER
#define _TAGGER

#include "dl.h"
#include "strhash.h"
struct web_page;

void tag_page(struct shash* h, struct web_page* w);
#endif

#ifndef _TAGGER
#define _TAGGER

#include "dl.h"
#include "strhash.h"
struct web_page;

/* should return 1 on success */
void taggem(struct shash* h, struct web_page* w, _Bool strip_tags);
#endif

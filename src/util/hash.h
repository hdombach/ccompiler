#pragma once

typedef unsigned long hash_t;

// http://www.cse.yorku.ca/~oz/hash.html
hash_t hashStr(const unsigned char *str);

hash_t hashStrp(const unsigned char **str);

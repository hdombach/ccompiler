#include "hash.h"

hash_t hashStr(const unsigned char *str) {
	hash_t hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

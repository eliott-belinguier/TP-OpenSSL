#ifndef _EX02_SEARCH_H_
#define _EX02_SEARCH_H_

#include <stddef.h>
#include <stdint.h>

ssize_t dict_search(const char *file_path, uint8_t *hash,
    size_t hash_len, int (*hash_func)(void *data, size_t len, uint8_t *hash));

#endif // _EX02_SEARCH_H_

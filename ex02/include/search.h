#ifndef _EX02_SEARCH_H_
#define _EX02_SEARCH_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t hash_len;
    uint8_t *hash;
    int (*hash_func)(void *data, size_t len, uint8_t *hash);
} search_option_t;

ssize_t dict_search(const char *file_path, search_option_t *option, char **line);

#endif // _EX02_SEARCH_H_

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void close_file(FILE **file)
{
    if (*file)
        fclose(*file);
}

ssize_t __tp_dict_search(const char *file_path, uint8_t *hash,
    size_t hash_len, int (*hash_func)(void *data, size_t len, uint8_t *hash))
{
    uint8_t file_hash[hash_len];
    FILE *file __attribute__ ((cleanup (close_file))) = 0;
    char *line = 0;
    size_t n = 0;
    ssize_t line_len;
    size_t line_count = 0;

    if (!file_path || !hash || !hash_func)
        return -1;
    file = fopen(file_path, "r");
    if (!file)
        return -1;
    while ((line_len = getline(&line, &n, file)) > 0) {
        ++line_count;
        if (line_len < 2)
            continue;
        if (hash_func(line, line_len - 1, file_hash) == -1)
            return -1;
        if (memcmp(hash, file_hash, hash_len) == 0)
            return line_count;
    }
    return -2;
}


ssize_t dict_search(const char *file_path, uint8_t *hash,
    size_t hash_len, int (*hash_func)(void *data, size_t len, uint8_t *hash))
    __attribute__ ((weak, alias ("__tp_dict_search")));

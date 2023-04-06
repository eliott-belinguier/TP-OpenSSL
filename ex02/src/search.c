#include <alloca.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "search.h"

static void close_file(FILE **file)
{
    if (*file)
        fclose(*file);
}

ssize_t __tp_dict_search(const char *path, search_option_t *opt, char **line)
{
    uint8_t *file_hash;
    FILE *file __attribute__ ((cleanup (close_file))) = 0;
    char *read_line = 0;
    size_t n = 0;
    ssize_t line_len;
    size_t line_count = 0;

    if (!path || !opt || opt->hash_len < 1 || !opt->hash || !opt->hash_func)
        return -1;
    file_hash = alloca(opt->hash_len);
    if (!file_hash)
        return -1;
    file = fopen(path, "r");
    if (!file)
        return -1;
    while ((line_len = getline(&read_line, &n, file)) > 0) {
        ++line_count;
        if (line_len < 2)
            continue;
        if (opt->hash_func(read_line, line_len - 1, file_hash) == -1)
            return -1;
        if (memcmp(opt->hash, file_hash, opt->hash_len) == 0) {
            *line = read_line;
            return line_count;
        }
    }
    return -2;
}


ssize_t dict_search(const char *file_path, search_option_t *option, char **line)
    __attribute__ ((weak, alias ("__tp_dict_search")));

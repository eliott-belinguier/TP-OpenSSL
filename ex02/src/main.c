#include <alloca.h>
#include <errno.h>
#include <getopt.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "search.h"
#include "sha.h"

static int parse_option(int argc, char **argv, uint8_t *hash, char **file_path)
{
    int opt_char;
    static struct option opt[] = {
        {"hash", required_argument, 0, 'h'},
        {"pass", required_argument, 0, 'p'},
        {"exercise", no_argument, 0, 'e'},
        {"dict", required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    while ((opt_char = getopt_long(argc, argv, "h:p:ed:", opt, 0)) != -1) {
        switch (opt_char) {
        case 'h':
            if (hash_sha256_from_str(optarg, hash) == -1)
                return -1;
            break;
        case 'p':
            if (!SHA256((uint8_t *) optarg, strlen(optarg), hash))
                return -1;
            break;
        case 'd':
            *file_path = strdup(optarg);
            break;
        default:
            errno = EINVAL;
            return -1;
        }
    }
    return 0;
}

static int hash_password(void *data, size_t len, uint8_t *hash)
{
    return SHA256(data, len, hash) ? 0 : -1;
}

int main(int argc, char **argv)
{
    search_option_t opt = {
        .hash_len = SHA256_DIGEST_LENGTH,
        .hash = alloca(SHA256_DIGEST_LENGTH),
        .hash_func = hash_password
    };
    char *file_path = 0;
    char *line = 0;
    ssize_t line_nb;

    if (!opt.hash || parse_option(argc, argv, opt.hash, &file_path) == -1)
        return errno;
    if (!file_path) {
        dprintf(2, "Dict file required.\n");
        return EINVAL;
    }
    line_nb = dict_search(file_path, &opt, &line);
    free(file_path);
    if (line_nb == -1)
        return errno;
    if (line_nb == -2) {
        printf("Password not found !\n");
        return 0;
    }
    printf("Password found at line %lu is %s", line_nb, line);
    free(line);
    return 0;
}

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
    int option_index;
    static struct option options[] = {
        {"hash", required_argument, 0, 'h'},
        {"pass", required_argument, 0, 'p'},
        {"exercise", no_argument, 0, 'e'},
        {"dict", required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };
    while (getopt_long(argc, argv, "hpe:", options, &option_index) != -1) {
        switch (option_index) {
        case 0:
            if (hash_sha256_from_str(optarg, hash) == -1)
                return -1;
            break;
        case 1:
            if (!SHA256((uint8_t *) optarg, strlen(optarg), hash))
                return -1;
            break;
        case 3:
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
    uint8_t hash[SHA256_DIGEST_LENGTH];
    char *file_path = 0;
    ssize_t line;

    if (parse_option(argc, argv, hash, &file_path) == -1)
        return errno;
    if (!file_path) {
        dprintf(2, "Dict file required.\n");
        return EINVAL;
    }
    line = dict_search(file_path, hash, SHA256_DIGEST_LENGTH, hash_password);
    free(file_path);
    if (line == -1)
        return errno;
    if (line == -2) {
        printf("Password not found !\n");
        return 0;
    }
    printf("Password found at line: %lu\n", line);
    return 0;
}

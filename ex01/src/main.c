#include <errno.h>
#include <unistd.h>
#include <openssl/sha.h>

#include "sha_print.h"
#include "sha_file.h"

int main(int argc, char **argv)
{
    uint8_t hash[SHA_DIGEST_LENGTH];
    char hash_str[SHA_DIGEST_LENGTH * 2];

    if (argc != 2)
        return EINVAL;
    if (hash_sha1_file(argv[1], hash) == -1)
        return errno;
    hash_printable(hash, SHA_DIGEST_LENGTH, hash_str);
    write(1, hash_str, sizeof(hash_str));
    write(1, "\n", 1);
    return 0;
}

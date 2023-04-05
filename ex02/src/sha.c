#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <openssl/sha.h>

#define HEX "0123456789abcdef"
#define HEX_LEN (sizeof(HEX) - 1)

#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) - 32 : (c))

static int convert_str_to_byte(char const *str, uint8_t *hash)
{
    char buffer[2] = {TO_LOWER(*str), 0};
    size_t first_index;
    size_t second_index;

    first_index = strcspn(HEX, buffer);
    if (first_index > 15)
        return -1;
    buffer[0] = TO_LOWER(str[1]);
    second_index = strcspn(HEX, buffer);
    if (second_index > 15)
        return -1;
    *hash = (((uint8_t) first_index) * HEX_LEN) + ((uint8_t) second_index);
    return 0;
}

int __tp_hash_sha256_from_str(char const *str, uint8_t *hash)
{
    if (!str || (strlen(str) / 2) != SHA256_DIGEST_LENGTH) {
        errno = EINVAL;
        return -1;
    }
    for (size_t i = 0; str[i]; i++) {
        if (convert_str_to_byte(str + i * 2, hash + i))
            return -1;
    }
    return 0;
}

int hash_sha256_from_str(char const *str, uint8_t *hash)
    __attribute__ ((weak, alias ("__tp_hash_sha256_from_str")));

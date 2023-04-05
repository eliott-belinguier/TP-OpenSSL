#include <stddef.h>
#include <stdint.h>

#define HEX "0123456789abcdef"
#define HEX_LEN (sizeof(HEX) - 1)

void __tp_hash_printable(const uint8_t *hash, size_t len, char *str)
{
    uint8_t current;
    size_t str_index;

    for (size_t i = 0; i < len; i++) {
        current = hash[i];
        str_index = i * 2;
        str[str_index] = HEX[(current / HEX_LEN) % HEX_LEN];
        str[str_index + 1] = HEX[current % HEX_LEN];
    }
}

void hash_printable(const uint8_t *hash, size_t len, char *str)
    __attribute__ ((weak, alias ("__tp_hash_printable")));

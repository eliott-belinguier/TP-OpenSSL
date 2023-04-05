#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 1024

static void _close_fd(int *fd)
{
    if (*fd != -1)
        close(*fd);
}

int __tp_hash_sha1_fd(const int fd, uint8_t *hash_buffer)
{
    SHA_CTX context;
    uint8_t buffer[BUFFER_SIZE];
    ssize_t read_count;

    if (fd == -1)
        return -1;
    if (SHA1_Init(&context) == 0)
        return -1;
    while ((read_count = read(fd, buffer, BUFFER_SIZE)) > 0) {
        if (SHA1_Update(&context, buffer, read_count) == 0)
            return -1;
    }
    if (read_count == -1)
        return -1;
    if (SHA1_Final(hash_buffer, &context) == 0)
        return -1;
    return 0;
}

int hash_sha1_fd(const int fd, uint8_t *hash_buffer)
    __attribute__ ((weak, alias ("__tp_hash_sha1_fd")));

int __tp_hash_sha1_file(const char *file_path, uint8_t *hash_buffer)
{
    int fd __attribute__ ((cleanup (_close_fd))) = -1;

    if (!file_path || !hash_buffer) {
        errno = EINVAL;
        return -1;
    }
    fd = open(file_path, O_RDONLY);
    if (fd == -1)
        return -1;
    return hash_sha1_fd(fd, hash_buffer);
}

int hash_sha1_file(const char *file_path, uint8_t *hash_buffer)
    __attribute__ ((weak, alias ("__tp_hash_sha1_file")));

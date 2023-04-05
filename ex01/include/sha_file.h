#ifndef _EX01_SHA_FILE_H_
#define _EX01_SHA_FILE_H_

#include <stdint.h>

int hash_sha1_fd(const int fd, uint8_t *hash_buffer);
int hash_sha1_file(const char *file_path, uint8_t *hash_buffer);

#endif // _EX01_SHA_FILE_H_

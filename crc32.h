#ifndef CRC32_H_INCLUDED
#define CRC32_H_INCLUDED
#include <stdlib.h>
#include <stdint.h>

uint32_t crc32(const void* data, size_t n_bytes);
#endif // CRC32_H_INCLUDED

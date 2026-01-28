#ifndef __MURMUR_H
#define __MURMUR_H
#include <stddef.h>
#include <stdint.h>

uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed);

#endif

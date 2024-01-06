#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include <stdint.h>

uint32_t speckhash(uint64_t* salt, char* key);

#ifdef __cplusplus
}
#endif
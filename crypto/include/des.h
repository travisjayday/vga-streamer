#ifndef DES_H
#define DES_H

#include <stdio.h>
#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#include <assert.h>
#include <x86intrin.h>
#include <inttypes.h>
#include <time.h>

#include "bitperm.h"

uint64_t encrypt_des_blk(uint64_t block, uint64_t key);

#endif

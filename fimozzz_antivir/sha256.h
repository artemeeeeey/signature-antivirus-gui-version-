#ifndef SHA256_H
#define SHA256_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

#define SHA256_SIZE_BYTES (32)
#define FN_ static inline __attribute__((const))
#define __CPROVER_assume(...) do {} while(0)

class sha256_cl {
public:
    typedef struct {
        uint8_t  buf[64];
        uint32_t hash[8];
        uint32_t bits[2];
        uint32_t len;
        uint32_t rfu__;
        uint32_t W[64];
    } sha256_context;

    static void sha256_init(sha256_context*);
    static void sha256_hash(sha256_context*, const void*, size_t);
    static void sha256_done(sha256_context*, uint8_t*);
    static void sha256(const void*, size_t, uint8_t*);

    static void _hash(sha256_context*);
    static void _addbits(sha256_context*, uint32_t);



};

#endif

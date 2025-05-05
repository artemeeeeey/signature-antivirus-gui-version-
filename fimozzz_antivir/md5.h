#ifndef MD5_H
#define MD5_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

#define F(X, Y, Z) ((X & Y) | (~X & Z))
#define G(X, Y, Z) ((X & Z) | (Y & ~Z))
#define H(X, Y, Z) (X ^ Y ^ Z)
#define I(X, Y, Z) (Y ^ (X | ~Z))

class md5 {
public:
    typedef struct {
        uint64_t size;        // Size of input in bytes
        uint32_t buffer[4];   // Current accumulation of hash
        uint8_t input[64];    // Input to be used in the next step
        uint8_t digest[16];   // Result of algorithm
    } MD5Context;

    static void md5Step(uint32_t*, uint32_t*);
    static void md5Init(MD5Context*);
    static void md5Update(MD5Context*, uint8_t *, size_t);
    static void md5Finalize(MD5Context*);
};

#endif

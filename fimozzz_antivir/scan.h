#ifndef SCAN_H
#define SCAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

#define MAX_PATTERNS 102000
#define MAX_PATTERN_LEN 1024

class scan {
public:
    typedef struct {
        uint8_t data[MAX_PATTERN_LEN];
        uint8_t mask[MAX_PATTERN_LEN];
        size_t length;
        char original[512];
    } BytePattern;

    static int check_polymorphic(const uint8_t*, size_t);
    static void hexdump(char*, int);
    static uint8_t rot13_left(uint8_t);
    static uint8_t rot13_right(uint8_t);
    static char* ya_gomoseksualist_md5(uint8_t*);
    static char* ya_gomoseksualist_sha256(uint8_t*);
    static int parse_pattern(const char*, BytePattern*);
    static int load_patterns(const char*, BytePattern *, int);
    static int find_pattern(const uint8_t*, size_t, const BytePattern*, int);
};

#endif

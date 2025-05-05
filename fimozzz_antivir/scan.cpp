#include "scan.h"

int scan::check_polymorphic(const uint8_t* code, size_t len) {
    const uint8_t xor_pattern[] = {0x67, 0x80, 0x30, 0x00};
    for (size_t i = 0; i < len - 3; i++) {
        if ((code[i] == xor_pattern[0] && code[i+1] == xor_pattern[1] && code[i+2] == xor_pattern[2])) {
            return 1;
        }
    }
    return 0;
}

void scan::hexdump(char *buf, int len) {
    int pos;
    char line[80];
    while (len > 0) {
        int cnt = (len > 16) ? 16 : len;
        pos = 0;

        for (int i = 0; i < cnt; i++) {
            pos += snprintf(&line[pos], sizeof(line) - pos, "%02x", (unsigned char)buf[i]);
        }
        //printf("%s\n", line);

        buf += cnt;
        len -= cnt;
    }
}

uint8_t scan::rot13_left(uint8_t a){
    //printf("%02x ", (a-0xD+0x100) % 0x100);
    return (a-0xD+0x100) % 0x100;
}

uint8_t scan::rot13_right(uint8_t a){
    //printf("%02x ", (a+0xD+0x100) % 0x100);
    return (a+0xD+0x100) % 0x100;
}

char* scan::ya_gomoseksualist_md5(uint8_t *hash) {
    int pos=0;
    char *result = (char*)malloc(33);
    for (int i = 0; i < 32; i++) {
        pos += snprintf(&result[pos], 33 - pos, "%02x", hash[i]);
    }
    result[32] = '\0';
    return result;
}

char* scan::ya_gomoseksualist_sha256(uint8_t *hash) {
    int pos=0;
    char *result = (char*)malloc(65);
    for (int i = 0; i < 64; i++) {
        pos += snprintf(&result[pos], 65 - pos, "%02x", hash[i]);
    }
    result[64] = '\0';
    return result;
}

int scan::parse_pattern(const char *str, scan::BytePattern *pattern) {
    size_t len = 0;
    size_t i = 0;
    memset(pattern, 0, sizeof(scan::BytePattern));
    strncpy(pattern->original, str, sizeof(pattern->original));

    while (str[i] && len < MAX_PATTERN_LEN) {
        if (isspace(str[i])) {
            i++;
            continue;
        }

        if (str[i] == '?' && str[i+1] == '?') {
            pattern->mask[len] = 0;
            i += 2;
            len++;
        }
        else if (isxdigit(str[i]) && isxdigit(str[i+1])) {
            char byte_str[3] = {str[i], str[i+1], '\0'};
            if (sscanf(byte_str, "%2hhx", &pattern->data[len]) != 1) {
                return 0;
            }
            pattern->mask[len] = 1;
            i += 2;
            len++;
        }
        else {
            return 0;
        }
    }
    pattern->length = len;
    return len > 0;
}

int scan::load_patterns(const char *filename, scan::BytePattern *patterns, int num_patterns) {
    FILE *fp = fopen(filename, "r");

    char line[512];
    while (fgets(line, sizeof(line), fp) && num_patterns < MAX_PATTERNS) {
        line[strcspn(line, "\r\n")] = 0;
        if (parse_pattern(line, &patterns[num_patterns])) {
            num_patterns++;
        }
    }
    fclose(fp);
    return 1;
}


int scan::find_pattern(const uint8_t *buffer, size_t buf_len, const scan::BytePattern *pattern, int score) {
    if (pattern->length == 0 || buf_len < pattern->length) return 0;
    const size_t limit = buf_len - pattern->length;
    for (size_t i = 0; i <= limit; i++) {
        int match = 1;
        for (size_t j = 0; j < pattern->length; j++) {
            if (pattern->mask[j] && (buffer[i + j] != pattern->data[j])) {
                match = 0;
                break;
            }
        }
        if (match) {
            score = 5;
            printf("match at address: 0x%08zx\n", i);
            printf("found pattern: %s\n", pattern->original);
            return 1;
        }
    }
    return 0;
}

#ifndef ZAD2_BITS_H
#define ZAD2_BITS_H

#include <inttypes.h>
#include <md5.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static inline void reverseWords(uint32_t words[const], size_t sourceLen) {
  for (size_t i = 0; i < sourceLen; i++) {
    uint32_t w = words[i];
    uint8_t bytes[4];
    memcpy(bytes, &w, sizeof(bytes));
    uint8_t t = bytes[0];
    bytes[0] = bytes[3];
    bytes[3] = t;
    t = bytes[1];
    bytes[1] = bytes[2];
    bytes[2] = t;
    memcpy(&w, bytes, sizeof(bytes));
    words[i] = w;
  }
}

static inline void bitsLittleEndianPutWord(uint8_t bytes[static 4],
                                           uint32_t word) {
  register const uint32_t mask = 0xFF;
  for (size_t i = 0; i < sizeof(word); i++) {
    uint8_t byte = word & mask;
    bytes[i] = byte;
    // assume 8-bit byte
    word >>= sizeof(byte) * 8;
  }
}

static inline void bitsCpyWordsLittle(uint8_t dest[restrict const],
                                      const uint32_t source[restrict const],
                                      size_t sourceLen) {
  for (size_t i = 0; i < sourceLen; i++) {
    uint8_t temp[4];
    const uint32_t w = source[i];
    bitsLittleEndianPutWord(temp, w);
    memcpy(&dest[i * sizeof(w)], temp, sizeof(temp));
  }

  //   for (size_t i = 0; i < sourceLen * sizeof(source[0]); i++) {
  //     // TODO: delete this
  //     printf("%" PRIx8, dest[i]);
  //     if (i % sizeof(source[0]) == 3) {
  //       printf(" ");
  //     }
  //   }
  //   puts("");
}

static inline void bitsPrintBytes(uint8_t bytes[], size_t len) {
  for (size_t i = 0; i < len; i++) {
    // TODO: delete this
    printf("%" PRIx8, bytes[i]);
    if (i % 4 == 3) {
      printf(" ");
    }
  }
  puts("");
}

static inline bool checkCollision(uint8_t m1[], uint8_t m2[], uint8_t m3[],
                                  uint8_t m4[], size_t len) {

  uint8_t hash1[16];
  uint8_t hash2[16];

  MD5_CTX ctx;
  md5_init(&ctx);
  md5_update(&ctx, m1, len);
  md5_update(&ctx, m2, len);
  md5_final(&ctx, hash1);

  md5_init(&ctx);
  md5_update(&ctx, m3, len);
  md5_update(&ctx, m4, len);
  md5_final(&ctx, hash2);

  return memcmp(hash1, hash2, sizeof(hash1)) == 0;
}

#endif // ZAD2_BITS_H

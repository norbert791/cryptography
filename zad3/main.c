#include <bits.h>
#include <check.h>
#include <limits.h>
#include <md5.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void randBytes(uint8_t arr[], size_t size) {
  for (size_t i = 0; i < size; i++) {
    arr[i] = rand() % UINT8_MAX;
  }
}

int main(void) {
  uint32_t words1[16] = {0x02dd31d1U, 0xc4eee6c5U, 0x069a3d69U, 0x5cf9af98U,
                         0x87b5ca2fU, 0xab7e4612U, 0x3e580440U, 0x897ffbb8U,
                         0x0634ad55U, 0x02b3f409U, 0x8388e483U, 0x5a417125U,
                         0xe8255108U, 0x9fc9cdf7U, 0xf2bd1dd9U, 0x5b3c3780U};

  uint32_t words2[16] = {0xd11d0b96U, 0x9c7b41dcU, 0xf497d8e4U, 0xd555655aU,
                         0xc79a7335U, 0x0cfdebf0U, 0x66f12930U, 0x8fb109d1U,
                         0x797f2775U, 0xeb5cd530U, 0xbaade822U, 0x5c15cc79U,
                         0xddcb74edU, 0x6dd3c55fU, 0xd80a9bb1U, 0xe3a7cc35U};

  uint32_t words3[16] = {0x02dd31d1U, 0xc4eee6c5U, 0x069a3d69U, 0x5cf9af98U,
                         0x07b5ca2fU, 0xab7e4612U, 0x3e580440U, 0x897ffbb8U,
                         0x0634ad55U, 0x02b3f409U, 0x8388e483U, 0x5a41f125U,
                         0xe8255108U, 0x9fc9cdf7U, 0x72bd1dd9U, 0x5b3c3780U};

  uint32_t words4[16] = {0xd11d0b96U, 0x9c7b41dcU, 0xf497d8e4U, 0xd555655aU,
                         0x479a7335U, 0x0cfdebf0U, 0x66f12930U, 0x8fb109d1U,
                         0x797f2775U, 0xeb5cd530U, 0xbaade822U, 0x5c154c79U,
                         0xddcb74edU, 0x6dd3c55fU, 0x580a9bb1U, 0xe3a7cc35U};

  uint8_t bytes1[16 * sizeof(uint32_t)];
  uint8_t bytes2[16 * sizeof(uint32_t)];
  uint8_t bytes3[16 * sizeof(uint32_t)];
  uint8_t bytes4[16 * sizeof(uint32_t)];

  bitsCpyWordsLittle(bytes1, words1, 16);
  bitsCpyWordsLittle(bytes2, words2, 16);
  bitsCpyWordsLittle(bytes3, words3, 16);
  bitsCpyWordsLittle(bytes4, words4, 16);

  while (true) {
#pragma omp parallel
    for (size_t i = 0; i < 1000; i++) {
      uint8_t bytes1[16 * sizeof(uint32_t)];
      uint8_t bytes2[16 * sizeof(uint32_t)];

      randBytes(bytes1, sizeof(bytes1[0]) * 16);
      memcpy(bytes2, bytes1, sizeof(bytes2));

      MD5_CTX ctx1;
      md5_init(&ctx1);
      md5_update(&ctx1, bytes1, 16 * sizeof(bytes1[0]));

      MD5_CTX ctx2;
      md5_init(&ctx2);
      md5_update(&ctx2, bytes2, 16 * sizeof(bytes2[0]));

      if (checkCollision(words1, bytes1, words2, bytes2, sizeof(bytes1))) {
        puts("collision found");
        return 0;
      }
    }
  }
}

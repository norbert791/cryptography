#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <md5.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bits.h>
#include <check.h>

int main(void) {
  messageSetVerbose(true);

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

  int exitCode = EXIT_SUCCESS;
  Message* m1 = messageNew(sizeof(bytes1), bytes1);
  if (m1 == NULL) {
    return EXIT_FAILURE;
  }
  Message* m2 = messageNew(sizeof(bytes2), bytes2);
  if (m2 == NULL) {
    exitCode = EXIT_FAILURE;
    goto delete_m1;
  }
  Message* m3 = messageNew(sizeof(bytes3), bytes3);
  if (m3 == NULL) {
    exitCode = 2;
    goto delete_m2;
  }
  Message* m4 = messageNew(sizeof(bytes4), bytes4);
  if (m4 == NULL) {
    exitCode = EXIT_FAILURE;
    goto delete_m3;
  }

  errno = 0;
  const bool result = messageCheckCollision(m1, m2, m3, m4);
  if (errno != 0) {
    perror("collision check failure");
  } else if (result) {
    puts("collision found");
  } else {
    puts("collision not found");
  }

  messageDelete(m4);
delete_m3:
  messageDelete(m3);
delete_m2:
  messageDelete(m2);
delete_m1:
  messageDelete(m1);

  if (exitCode == EXIT_FAILURE) {
    exit(EXIT_FAILURE);
  }

  uint32_t words5[16] = {0x02dd31d1U, 0xc4eee6c5U, 0x069a3d69U, 0x5cf9af98U,
                         0x87b5ca2fU, 0xab7e4612U, 0x3e580440U, 0x897ffbb8U,
                         0x0634ad55U, 0x02b3f409U, 0x8388e483U, 0x5a417125U,
                         0xe8255108U, 0x9fc9cdf7U, 0xf2bd1dd9U, 0x5b3c3780U};

  uint32_t words6[16] = {0x313e82d8U, 0x5b8f3456U, 0xd4ac6daeU, 0xc619c936U,
                         0xb4e253ddU, 0xfd03da87U, 0x6633902U,  0xa0cd48d2U,
                         0x42339fe9U, 0xe87e570fU, 0x70b654ceU, 0x1e0da880U,
                         0xbc2198c6U, 0x9383a8b6U, 0x2b65f996U, 0x702af76fU};

  uint32_t words7[16] = {0x02dd31d1U, 0xc4eee6c5U, 0x069a3d69U, 0x5cf9af98U,
                         0x07b5ca2fU, 0xab7e4612U, 0x3e580440U, 0x897ffbb8U,
                         0x0634ad55U, 0x02b3f409U, 0x8388e483U, 0x5a41f125U,
                         0xe8255108U, 0x9fc9cdf7U, 0x72bd1dd9U, 0x5b3c3780U};

  uint32_t words8[16] = {0x313e82d8U, 0x5b8f3456U, 0xd4ac6daeU, 0xc619c936U,
                         0x34e253ddU, 0xfd03da87U, 0x6633902U,  0xa0cd48d2U,
                         0x42339fe9U, 0xe87e570fU, 0x70b654ceU, 0x1e0d2880U,
                         0xbc2198c6U, 0x9383a8b6U, 0xab65f996U, 0x702af76fU};

  bitsCpyWordsLittle(bytes1, words5, 16);
  bitsCpyWordsLittle(bytes2, words6, 16);
  bitsCpyWordsLittle(bytes3, words7, 16);
  bitsCpyWordsLittle(bytes4, words8, 16);

  exitCode = EXIT_SUCCESS;
  Message* m5 = messageNew(sizeof(bytes1), bytes1);
  if (m5 == NULL) {
    return EXIT_FAILURE;
  }
  Message* m6 = messageNew(sizeof(bytes2), bytes2);
  if (m6 == NULL) {
    exitCode = EXIT_FAILURE;
    goto delete_m5;
  }
  Message* m7 = messageNew(sizeof(bytes3), bytes3);
  if (m7 == NULL) {
    exitCode = 2;
    goto delete_m6;
  }
  Message* m8 = messageNew(sizeof(bytes4), bytes4);
  if (m8 == NULL) {
    exitCode = EXIT_FAILURE;
    goto delete_m7;
  }

  errno = 0;
  const bool result2 = messageCheckCollision(m5, m6, m7, m8);
  if (errno != 0) {
    perror("collision check failure");
  } else if (result2) {
    puts("collision found");
  } else {
    puts("collision not found");
  }

  messageDelete(m8);
delete_m7:
  messageDelete(m7);
delete_m6:
  messageDelete(m6);
delete_m5:
  messageDelete(m5);

  return exitCode;
}

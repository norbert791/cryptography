#include <bits.h>
#include <check.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <md5.h>

static bool VERBOSE = false;

static void printHash(const uint8_t bytes[16]);

typedef struct Message {
  size_t size;
  uint8_t data[];
} Message;

Message* messageNew(const size_t size,
                    const uint8_t data[const restrict static size]) {
  Message* const result =
      calloc(sizeof(*result) + sizeof(result->data[0]) * size, 1);
  if (result == NULL) {
    return NULL;
  }

  result->size = size;
  memcpy(result->data, data, size);

  return result;
}

void messageDelete(Message* const msg) { free(msg); }

size_t getSize(Message* msg) {
  if (msg == NULL) {
    return 0;
  }

  return msg->size;
}

uint8_t* getData(Message* msg) { return msg->data; }

bool messageCheckCollision(Message* restrict m1, Message* restrict m2,
                           Message* restrict m3, Message* restrict m4) {
  errno = 0;
  bool result = false;

  uint8_t hash1[16];
  uint8_t hash2[16];

  MD5_CTX ctx;
  md5_init(&ctx);
  md5_update(&ctx, m1->data, m1->size);
  md5_update(&ctx, m2->data, m2->size);
  md5_final(&ctx, hash1);

  md5_init(&ctx);
  md5_update(&ctx, m3->data, m3->size);
  md5_update(&ctx, m4->data, m4->size);
  md5_final(&ctx, hash2);

  result = memcmp(hash1, hash2, sizeof(hash1)) == 0;

  if (VERBOSE) {
    printf("hash1: ");
    printHash(hash1);
    printf("\nhash2: ");
    printHash(hash2);
    puts("");
  }

  return result;
}

static void reverseWord(uint32_t* w) {
  uint8_t data[4];
  memcpy(data, w, sizeof(*w));
  uint8_t temp = data[3];
  data[3] = data[0];
  data[0] = temp;
  temp = data[2];
  data[2] = data[1];
  data[1] = temp;
  memcpy(w, data, sizeof(*w));
}

void messageSetVerbose(bool isVerbose) { VERBOSE = isVerbose; }

static void printHash(const uint8_t bytes[const 16]) {
  for (size_t i = 0; i < 16; i += 4) {
    uint32_t word = 0;
    memcpy(&word, &bytes[i], sizeof(word));
    reverseWord(&word);
    printf("%" SCNx32 " ", word);
  }
}

#include <check.h>
#include <errno.h>
#include <inttypes.h>
#include <mbedtls/md5.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool VERBOSE = false;

static Message* mergeMessages(Message* restrict m1, Message* restrict m2);
static void reverseWords32(Message* m);
static int messageMD5(const Message* m, uint8_t result[16]);
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

  Message* bytes1 = mergeMessages(m1, m2);
  if (bytes1 == NULL) {
    errno = MESSAGE_ERRNO_MEM;
    return false;
  }
  Message* bytes2 = mergeMessages(m3, m4);
  if (bytes2 == NULL) {
    errno = MESSAGE_ERRNO_MEM;
    goto delete_bytes1;
  }

  reverseWords32(bytes1);
  reverseWords32(bytes2);

  uint8_t hash1[16];
  uint8_t hash2[16];

  int res1 = messageMD5(bytes1, hash1);
  if (res1 != 0) {
    errno = MESSAGE_ERROR_HASH;
    goto delete_bytes2;
  }
  int res2 = messageMD5(bytes2, hash2);
  if (res2 != 0) {
    errno = MESSAGE_ERROR_HASH;
    goto delete_bytes2;
  }

  result = memcmp(hash1, hash2, sizeof(hash1)) == 0;

  if (VERBOSE) {
    printf("hash1: ");
    printHash(hash1);
    printf("\nhash2: ");
    printHash(hash2);
    puts("");
  }

delete_bytes2:
  messageDelete(bytes2);
delete_bytes1:
  messageDelete(bytes1);

  return result;
}

static Message* mergeMessages(Message* restrict m1, Message* restrict m2) {
  Message* const result = calloc(
      sizeof(*result) + (m1->size + m2->size) * sizeof(result->data[0]), 1);
  // Note: if callaloc fails -> return NULL
  memcpy(result->data, m1->data, m1->size);
  memcpy(&result->data[sizeof(m2->size)], m2->data, m2->size);
  result->size = m1->size + m2->size;

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

static void reverseWords32(Message* const m) {
  // "panic"
  if (m->size % 4 != 0) {
    perror("message size is not multiplicity of 4bytes");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i + 4 < m->size; i += 4) {
    uint8_t temp = m->data[i];
    m->data[i] = m->data[i + 3];
    m->data[i + 3] = temp;
    temp = m->data[i + 1];
    m->data[i + 1] = m->data[i + 2];
    m->data[i + 2] = temp;
  }
}

static int messageMD5(const Message* const m, uint8_t result[const 16]) {
  return mbedtls_md5(m->data, m->size, result);
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

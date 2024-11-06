#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <check.h>

static size_t sscanfArray(const char str[], uint8_t bytes[16]);

int main(void) {
  const char str1[] =
      "2dd31d1 c4eee6c5 69a3d69 5cf9af98 87b5ca2f ab7e4612 3e580440 897ffbb8 "
      "634ad55 2b3f409 8388e483 5a417125 e8255108 9fc9cdf7 f2bd1dd9 "
      "5b3c3780";
  const char str2[] =
      "d11d0b96 9c7b41dc f497d8e4 d555655a c79a7335 cfdebf0 66f12930 "
      "8fb109d1 797f2775 eb5cd530 baade822 5c15cc79 ddcb74ed 6dd3c55f "
      "d80a9bb1 e3a7cc35";
  const char str3[] = "2dd31d1 c4eee6c5 69a3d69 5cf9af98 7b5ca2f ab7e4612 "
                      "3e580440 897ffbb8 634ad55 2b3f409 8388e483 "
                      "5a41f125 e8255108 9fc9cdf7 72bd1dd9 5b3c3780 ";
  const char str4[] =
      "d11d0b96 9c7b41dc f497d8e4 d555655a 479a7335 cfdebf0 66f12930 8fb109d1 "
      "797f2775 eb5cd530 baade822 5c154c79 ddcb74ed 6dd3c55f 580a9bb1 e3a7cc35";

  uint8_t bytes1[16];
  uint8_t bytes2[16];
  uint8_t bytes3[16];
  uint8_t bytes4[16];

  if (sscanfArray(str1, bytes1) != 16) {
    return EXIT_FAILURE;
  }
  if (sscanfArray(str2, bytes2) != 16) {
    return EXIT_FAILURE;
  }
  if (sscanfArray(str3, bytes3) != 16) {
    return EXIT_FAILURE;
  }
  if (sscanfArray(str4, bytes4) != 16) {
    return EXIT_FAILURE;
  }

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
  if (result) {
    puts("collision found");
  } else if (errno != 0) {
    perror("collision check failure");
  }

  messageDelete(m4);
delete_m3:
  messageDelete(m3);
delete_m2:
  messageDelete(m2);
delete_m1:
  messageDelete(m1);

  return exitCode;
}

static size_t sscanfArray(const char str[], uint8_t bytes[16]) {
  size_t count = 0;
  char* endptr;
  while (*str != '\0' && count < 16) {
    uint64_t value = strtoull(str, &endptr, 16); // Parse as unsigned long long
    if (str == endptr) {
      break; // No more numbers to parse
    }
    bytes[count++] = value;
    str = endptr;
  }

  return count;
}

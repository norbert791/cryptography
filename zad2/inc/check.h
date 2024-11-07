#ifndef ZAD2_CHECK_H
#define ZAD2_CHECK_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

enum MessageError { MESSAGE_ERROR_HASH, MESSAGE_ERRNO_MEM };

typedef struct Message Message;

Message* messageNew(size_t size, const uint8_t data[restrict static size]);
void messageDelete(Message* msg);
size_t getSize(Message* msg);
uint8_t* getData(Message* msg);

bool messageCheckCollision(Message* restrict m1, Message* restrict m2,
                           Message* restrict m3, Message* restrict m4);
void messageSetVerbose(bool isVerbose);

#endif // ZAD2_CHECK_H

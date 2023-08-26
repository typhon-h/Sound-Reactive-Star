#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <stdint.h>
typedef struct
{
    int16_t size;
    int16_t windex;
    int16_t rindex;
    int16_t *data;
} CircBuf_t;

int16_t *initCircBuf(CircBuf_t *buff, int16_t size);

void writeCircBuf(CircBuf_t *buff, int16_t value);

int16_t readCircBuf(CircBuf_t *buff);

void freeCircBuf(CircBuf_t buff);

#endif // CIRCBUF_H

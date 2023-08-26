#include "circularBuffer.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int16_t *initCircBuf(CircBuf_t *buff, int16_t size)
{
    buff->size = size;
    buff->windex = 0;
    buff->rindex = 0;
    buff->data =
        (int16_t *)calloc(size, sizeof(int16_t));
    return buff->data;
}

void writeCircBuf(CircBuf_t *buff, int16_t value)
{
    buff->data[buff->windex++] = value;

    if (buff->windex >= buff->size)
        buff->windex = 0;
}

int16_t readCircBuf(CircBuf_t *buff)
{
    int16_t val = buff->data[buff->rindex++];

    if (buff->rindex >= buff->size)
        buff->rindex = 0;
    return val;
}

void freeCircBuf(CircBuf_t *buff)
{
    buff->windex = 0;
    buff->rindex = 0;
    buff->size = 0;

    free(buff->data);
    buff->data = NULL;
}

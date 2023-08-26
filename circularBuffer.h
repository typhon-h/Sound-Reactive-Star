#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <stdint.h>

typedef struct
{
	int16_t size;
	int16_t windex;
	int16_t rindex;
	int16_t *data;
} circBuf_t;

int16_t *initCircBuf(circBuf_t *buff, int16_t size);

void writeCircBuf(circBuf_t *buff, int16_t value);

int16_t readCircBuf(circBuf_t *buff);

void freeCircBuf(circBuf_t *buff);

#endif // CIRCBUF_H

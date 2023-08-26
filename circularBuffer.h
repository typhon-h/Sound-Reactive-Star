#ifndef CIRCBUF_H_
#define CIRCBUF_H_

#include <stdint.h>

typedef struct
{
	uint16_t size;
	uint16_t windex;
	uint16_t rindex;
	uint16_t *data;
} circBuf_t;

uint16_t *initCircBuf(circBuf_t *buff, uint16_t size);

void writeCircBuf(circBuf_t *buff, uint16_t value);

uint16_t readCircBuf(circBuf_t *buff);

void freeCircBuf(circBuf_t *buff);

#endif /*CIRCBUF_H_*/

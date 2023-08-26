
#include <stdint.h>

#include "circularBuffer.h"

uint16_t *initCircBuf(circBuf_t *buff, uint16_t size)
{
	buff->size = size;
	buff->windex = 0;
	buff->rindex = 0;
	buff->data =
		(uint16_t *)calloc(size, sizeof(uint16_t));
	return buff->data;
}

void writeCircBuf(circBuf_t *buff, uint16_t entry)
{
	buff->data[buff->windex++] = entry;

	if (buff->windex >= buff->size)
		buff->windex = 0;
}

uint16_t readCircBuf(circBuf_t *buff)
{
	uint16_t val = buff->data[buff->rindex++];

	if (buff->rindex >= buff->size)
		buff->rindex = 0;
	return val;
}

void freeCircBuf(circBuf_t *buff)
{
	buff->windex = 0;
	buff->rindex = 0;
	buff->size = 0;

	free(buff->data);
	buff->data = NULL;
}

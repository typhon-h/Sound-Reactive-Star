
#include <stdint.h>

#include "circularBuffer.h"

int16_t *initCircBuf(circBuf_t *buff, int16_t size)
{
	buff->size = size;
	buff->windex = 0;
	buff->rindex = 0;
	buff->data =
		(int16_t *)calloc(size, sizeof(int16_t));
	return buff->data;
}

void writeCircBuf(circBuf_t *buff, int16_t value)
{
	buff->data[buff->windex++] = value;

	if (buff->windex >= buff->size)
		buff->windex = 0;
}

int16_t readCircBuf(circBuf_t *buff)
{
	int16_t val = buff->data[buff->rindex++];

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

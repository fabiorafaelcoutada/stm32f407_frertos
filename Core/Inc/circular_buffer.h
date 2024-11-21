#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stddef.h>

/* Define buffer size */
#define BUFFER_SIZE 10U

/* Circular buffer structure */
typedef struct {
    float data[BUFFER_SIZE]; /* Data storage */
    uint8_t head;              /* Index for the next write */
    uint8_t tail;              /* Index for the next read */
    uint8_t count;             /* Number of elements in the buffer */
} CircularBuffer_t;

/* Function Prototypes */
void CircularBuffer_Init(CircularBuffer_t *buffer);
void CircularBuffer_Add(CircularBuffer_t *buffer, float value);

#endif /* CIRCULAR_BUFFER_H */

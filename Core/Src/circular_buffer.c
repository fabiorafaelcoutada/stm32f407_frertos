#include "circular_buffer.h"

/**
 * @brief Initializes the circular buffer.
 *
 * This function sets the head, tail, and count of the circular buffer to zero,
 * effectively clearing any existing data in the buffer.
 *
 * @param buffer Pointer to the CircularBuffer structure to be initialized.
 */
void CircularBuffer_Init(CircularBuffer_t *buffer) {
    if (buffer != NULL) {
        buffer->head = 0U;
        buffer->tail = 0U;
        buffer->count = 0U;
    }
}

/**
 * @brief Adds a value to the circular buffer.
 *
 * This function adds a new temperature reading to the circular buffer.
 * If the buffer is full, it overwrites the oldest value.
 *
 * @param buffer Pointer to the CircularBuffer structure.
 * @param value The temperature value to be added to the buffer.
 */
void CircularBuffer_Add(CircularBuffer_t *buffer, float value) {
	  buffer->data[buffer->head] = value; // Add value to buffer
	  buffer->head = (buffer->head + 1) % BUFFER_SIZE; // Move head to next position
	  if (buffer->count < BUFFER_SIZE) {
	    buffer->count++; // Increment count if buffer is not full
	  }
}

/**
 * @brief Prints the contents of the circular buffer.
 *
 * This function iterates through the circular buffer and prints each stored
 * temperature value to the console. The buffer is printed in the order of
 * oldest to newest entry.
 *
 * @param buffer Pointer to the CircularBuffer structure.
 */
void CircularBuffer_Print(CircularBuffer_t *buffer) {
  printf("Buffer: ");
  for (int i = 0; i < buffer->count; i++) {
    int index = (buffer->head - buffer->count + i + BUFFER_SIZE) % BUFFER_SIZE;
    printf("%.2f ", buffer->data[index]);
  }
  printf("\r\n");
}

/* Check if the buffer is empty */
uint8_t CircularBuffer_IsEmpty(const CircularBuffer_t *buffer) {
    if (buffer == NULL) {
        return 1U; /* Null buffer treated as empty */
    }

    return (buffer->count == 0U) ? 1U : 0U;
}

/* Check if the buffer is full */
uint8_t CircularBuffer_IsFull(const CircularBuffer_t *buffer) {
    if (buffer == NULL) {
        return 0U; /* Null buffer cannot be full */
    }

    return (buffer->count == BUFFER_SIZE) ? 1U : 0U;
}

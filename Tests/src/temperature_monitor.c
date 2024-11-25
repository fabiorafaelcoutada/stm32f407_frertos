#include "temperature_monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void circular_buffer_init(CircularBuffer_t* const buffer) {
    if (NULL == buffer) {
        return;
    }

    buffer->write_index = 0U;
    buffer->read_index = 0U;
    buffer->count = 0U;
    pthread_mutex_init(&buffer->mutex, NULL);
}

bool circular_buffer_write(CircularBuffer_t* const buffer, const float value) {
    bool result = false;

    if (NULL == buffer) {
        return false;
    }

    pthread_mutex_lock(&buffer->mutex);

    if (buffer->count < BUFFER_SIZE) {
        buffer->temperatures[buffer->write_index] = value;
        buffer->write_index = (buffer->write_index + 1U) % BUFFER_SIZE;
        buffer->count++;
        result = true;
    }

    pthread_mutex_unlock(&buffer->mutex);
    return result;
}

bool circular_buffer_read(CircularBuffer_t* const buffer, float* const value) {
    bool result = false;

    if ((NULL == buffer) || (NULL == value)) {
        return false;
    }

    pthread_mutex_lock(&buffer->mutex);

    if (buffer->count > 0U) {
        *value = buffer->temperatures[buffer->read_index];
        buffer->read_index = (buffer->read_index + 1U) % BUFFER_SIZE;
        buffer->count--;
        result = true;
    }

    pthread_mutex_unlock(&buffer->mutex);
    return result;
}

float simulate_temperature_reading(void) {
    /* Simulate temperature between 20.0 and 30.0 degrees Celsius */
    return 20.0F + ((float)rand() / (float)RAND_MAX) * 10.0F;
}

void* sensor_thread(void* arg) {
    ThreadArgs_t* thread_args = (ThreadArgs_t*)arg;
    struct timespec sleep_time = {0, SENSOR_READ_INTERVAL_MS * 1000000L};

    while (*(thread_args->running)) {
        float temperature = simulate_temperature_reading();
        circular_buffer_write(thread_args->buffer, temperature);
        nanosleep(&sleep_time, NULL);
    }

    return NULL;
}

void* display_thread(void* arg) {
    ThreadArgs_t* thread_args = (ThreadArgs_t*)arg;
    struct timespec sleep_time = {0, DISPLAY_INTERVAL_MS * 1000000L};
    float temperature;

    while (*(thread_args->running)) {
        printf("\nTemperature readings:\n");
        while (circular_buffer_read(thread_args->buffer, &temperature)) {
            printf("%.2f°C\n", temperature);
        }
        nanosleep(&sleep_time, NULL);
    }

    return NULL;
}
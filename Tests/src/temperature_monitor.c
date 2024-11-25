// temperature_monitor.c
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
    if (buffer == NULL) {
        return false;
    }

    pthread_mutex_lock(&buffer->mutex);

    // Always write, overwriting oldest data if full
    buffer->temperatures[buffer->write_index] = value;
    buffer->write_index = (buffer->write_index + 1U) % BUFFER_SIZE;

    // Limit count to buffer size
    if (buffer->count < BUFFER_SIZE) {
        buffer->count++;
    }

    pthread_mutex_unlock(&buffer->mutex);
    return true;
}

bool circular_buffer_read(CircularBuffer_t* const buffer, float* const value) {
    bool result = false;
    
    if ((buffer == NULL) || (value == NULL)) {
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

void display_buffer_contents(CircularBuffer_t* const buffer) {
    uint32_t i, display_count;

    if (NULL == buffer) {
        return;
    }

    pthread_mutex_lock(&buffer->mutex);

    printf("\nCurrent Buffer Contents:\n");
    printf("Total Readings: %u\n", buffer->count);

    display_count = buffer->count;
    for (i = 0U; i < display_count; i++) {
        float temperature = buffer->temperatures[
            (buffer->read_index + i) % BUFFER_SIZE
        ];

        printf("Reading %u: %.2f°C\n", i + 1, temperature);
    }

    pthread_mutex_unlock(&buffer->mutex);
}

float simulate_temperature_reading(void) {
    return 20.0F + ((float)rand() / (float)RAND_MAX) * 10.0F;
}

void sleep_until_next_period(struct timespec* next_period, time_t interval_sec) {
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, next_period, NULL);
    next_period->tv_sec += interval_sec;
}

void* sensor_thread(void* arg) {
    ThreadArgs_t* thread_args = (ThreadArgs_t*)arg;
    struct timespec next_period;
    time_t current_time;
    struct tm* time_info;
    
    clock_gettime(CLOCK_MONOTONIC, &next_period);
    
    while (*(thread_args->running)) {
        float temperature = simulate_temperature_reading();
        
        if (circular_buffer_write(thread_args->buffer, temperature)) {
            current_time = time(NULL);
            time_info = localtime(&current_time);
            printf("[%02d:%02d:%02d] Temperature logged: %.2f°C\n",
                   time_info->tm_hour,
                   time_info->tm_min,
                   time_info->tm_sec,
                   temperature);
        }
        
        sleep_until_next_period(&next_period, SENSOR_READ_INTERVAL_SEC);
    }
    
    return NULL;
}

void* display_thread(void* arg) {
    ThreadArgs_t* thread_args = (ThreadArgs_t*)arg;
    struct timespec next_period;
    
    clock_gettime(CLOCK_MONOTONIC, &next_period);
    
    while (*(thread_args->running)) {
        display_buffer_contents(thread_args->buffer);
        
        sleep_until_next_period(&next_period, DISPLAY_INTERVAL_SEC);
    }
    
    return NULL;
}
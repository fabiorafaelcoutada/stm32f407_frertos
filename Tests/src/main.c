#include "temperature_monitor.h"
#include <signal.h>
#include <stdlib.h>

static volatile bool running = true;

void signal_handler(int signum) {
    (void)signum;
    running = false;
}

int main(void) {
    CircularBuffer_t buffer;
    pthread_t sensor_thread_id, display_thread_id;
    ThreadArgs_t thread_args;

    /* Initialize */
    signal(SIGINT, signal_handler);
    srand((unsigned int)time(NULL));
    circular_buffer_init(&buffer);

    /* Set up thread arguments */
    thread_args.buffer = &buffer;
    thread_args.running = &running;

    /* Create threads */
    pthread_create(&sensor_thread_id, NULL, sensor_thread, &thread_args);
    pthread_create(&display_thread_id, NULL, display_thread, &thread_args);

    /* Wait for threads to complete */
    pthread_join(sensor_thread_id, NULL);
    pthread_join(display_thread_id, NULL);

    /* Cleanup */
    pthread_mutex_destroy(&buffer.mutex);

    return 0;
}
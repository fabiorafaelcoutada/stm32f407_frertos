#include "unity.h"
#include "temperature_monitor.h"

CircularBuffer_t test_buffer;

void setUp(void) {
    circular_buffer_init(&test_buffer);
}

void tearDown(void) {
    pthread_mutex_destroy(&test_buffer.mutex);
}

void test_circular_buffer_write_read(void) {
    float write_value = 25.5F;
    float read_value = 0.0F;

    TEST_ASSERT_TRUE(circular_buffer_write(&test_buffer, write_value));
    TEST_ASSERT_TRUE(circular_buffer_read(&test_buffer, &read_value));
    TEST_ASSERT_EQUAL_FLOAT(write_value, read_value);
}

void test_circular_buffer_overflow(void) {
    float value = 25.5F;
    uint32_t i;

    /* Fill buffer */
    for (i = 0U; i < BUFFER_SIZE; i++) {
        TEST_ASSERT_TRUE(circular_buffer_write(&test_buffer, value));
    }

    /* Try to write one more value */
    TEST_ASSERT_FALSE(circular_buffer_write(&test_buffer, value));
}

void test_circular_buffer_underflow(void) {
    float value;

    TEST_ASSERT_FALSE(circular_buffer_read(&test_buffer, &value));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_circular_buffer_write_read);
    RUN_TEST(test_circular_buffer_overflow);
    RUN_TEST(test_circular_buffer_underflow);
    return UNITY_END();
}
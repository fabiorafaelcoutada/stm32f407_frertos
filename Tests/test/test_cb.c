#include "unity.h"
#include "circular_buffer.h"

void setUp(void) {}
void tearDown(void) {}

void test_CircularBuffer_Init(void) {
    CircularBuffer cb;
    CircularBuffer_Init(&cb);

    TEST_ASSERT_TRUE(CircularBuffer_IsEmpty(&cb));
    TEST_ASSERT_FALSE(CircularBuffer_IsFull(&cb));
}

void test_CircularBuffer_PushPop(void) {
    CircularBuffer cb;
    CircularBuffer_Init(&cb);

    TEST_ASSERT_TRUE(CircularBuffer_Push(&cb, 42));
    TEST_ASSERT_FALSE(CircularBuffer_IsEmpty(&cb));
    TEST_ASSERT_FALSE(CircularBuffer_IsFull(&cb));

    int32_t value;
    TEST_ASSERT_TRUE(CircularBuffer_Pop(&cb, &value));
    TEST_ASSERT_EQUAL(42, value);
    TEST_ASSERT_TRUE(CircularBuffer_IsEmpty(&cb));
}

void test_CircularBuffer_Overflow(void) {
    CircularBuffer cb;
    CircularBuffer_Init(&cb);

    for (int i = 0; i < BUFFER_SIZE; i++) {
        TEST_ASSERT_TRUE(CircularBuffer_Push(&cb, i));
    }
    TEST_ASSERT_TRUE(CircularBuffer_IsFull(&cb));

    TEST_ASSERT_FALSE(CircularBuffer_Push(&cb, 99));
}

void test_CircularBuffer_Underflow(void) {
    CircularBuffer cb;
    CircularBuffer_Init(&cb);

    int32_t value;
    TEST_ASSERT_FALSE(CircularBuffer_Pop(&cb, &value));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_CircularBuffer_Init);
    RUN_TEST(test_CircularBuffer_PushPop);
    RUN_TEST(test_CircularBuffer_Overflow);
    RUN_TEST(test_CircularBuffer_Underflow);
    return UNITY_END();
}

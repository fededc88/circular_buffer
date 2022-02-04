#include "unity.h"

#include "TestBuffer.h"
#include "buffer.h"

/* generic test struct declaration */
struct test 
{
    int integer;
    float floating;
    char character;
};

circular_buffer cb;

/* circular_buffer_handler declaration - funtion as a pointer */
circular_buffer_handler cbh;

struct test array[ARRAY_LEN];
struct test sData[ARRAY_LEN];

void setUp(void)
{
    int j;
    char a;
    float f;

   /* initialize the sData buffer with values to test the library */
    for(int i = 0, a = '0', j = 0, f = 0.0; i < ARRAY_LEN; i++, a++, j++, f + 0.5)
    {
        if(a > 'z')
            a = '0';

        sData[i].character = a;
        sData[i].integer = j;
        sData[i].floating = f;
    }
}

void tearDown(void)
{
}

void test_circular_buffer_initialization(void)
{
    TEST_ASSERT_NOT_NULL_MESSAGE((cbh = circular_buffer_init((void*)array,
                                  sizeof(struct test), ARRAY_LEN)),"cbh == NULL");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(ARRAY_LEN, circular_buffer_size(cbh), "Wrong buffer size");
}

void test_load_entire_buffer(void)
{
    struct test aux[ARRAY_LEN];

    circular_buffer_reset(cbh);

    for(int i = 0; i<ARRAY_LEN-1 ; i++)
        TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_put(cbh, (void*) &sData[i]),"No more space");

    TEST_ASSERT_EQUAL_UINT16_MESSAGE(ARRAY_LEN-1, circular_buffer_count(cbh), "Wrong length");

    for(int i = 0; i<ARRAY_LEN-1 ; i++)
       TEST_ASSERT_EQUAL_MEMORY(&sData[i], &array[i], sizeof(struct test));

    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_drop(cbh, (void*)&aux, ARRAY_LEN-1), "couldent dump");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, circular_buffer_count(cbh), "Wrong buffer size");

    for(int i = 0; i<ARRAY_LEN-1 ; i++)
       TEST_ASSERT_EQUAL_MEMORY(&sData[i], &aux[i], sizeof(struct test));
}

void test_load_partial_buffer(void)
{
    struct test aux[ARRAY_LEN];

    circular_buffer_reset(cbh);

    for (int a = 1; a < ARRAY_LEN; a++)
    {
	for(int i = 0; i < a ; i++)
        TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_put(cbh,(void *) &sData[i]), "No more space");

    TEST_ASSERT_EQUAL_UINT16_MESSAGE(a, circular_buffer_count(cbh), "Wrong length");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_drop(cbh, (void *) &aux, a), "couldnt dump");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, circular_buffer_count(cbh), "Wrong buffer size");

    for(int i = 0; i < a ; i++)
        TEST_ASSERT_EQUAL_MEMORY(&sData[i], &aux[i], sizeof(struct test));
    }
}

void test_push_buffer(void)
{
    struct test aux[ARRAY_LEN];

    /* Load the entire buffer and check it is full */
    for(;0 == circular_buffer_put(cbh, &sData[1]););
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(ARRAY_LEN-1, circular_buffer_count(cbh), "Wrong buffer size");

    /* Push data and re-load the entire buffer and check it is full */
    for(int i = 0; i < ARRAY_LEN-1; i++)
        circular_buffer_push(cbh, (void *) &sData[i]);

    /* Dump the entire circular buffer in aux[] */
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_drop(cbh, (void *) &aux, ARRAY_LEN-1), "couldent dump");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, circular_buffer_count(cbh), "Wrong buffer size");

    for(int i = 0; i < ARRAY_LEN-1; i++)
        TEST_ASSERT_EQUAL_MEMORY(&sData[i], &aux[i], sizeof(struct test));
}

void test_put_pop(void)
{
    struct test aux;

    circular_buffer_reset(cbh);
    for(int a = 0; a < 100 ; a++)
    {
        for(int i = 0; i < ARRAY_LEN; i++)
        {
            TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_put(cbh,(void *) &sData[i]), "put failed");
            TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_pop(cbh, (void *) &aux), "pop failed");
            TEST_ASSERT_EQUAL_MEMORY(&sData[i], &aux, sizeof(struct test));
        }
    }
}

void test_circular_buffer_free(void)
{
    TEST_ASSERT_NULL_MESSAGE(circular_buffer_free(&cbh), "cbh != NULL" );
}

int main(void)
{

    UNITY_BEGIN();

    RUN_TEST(test_circular_buffer_initialization);

//    for(int i = 0; i<1000; i++)
//    {

        RUN_TEST(test_push_buffer);
        RUN_TEST(test_put_pop);

        RUN_TEST(test_load_entire_buffer);
        RUN_TEST(test_load_partial_buffer);
//    }

    RUN_TEST(test_circular_buffer_free);

    cbh = &cb;
    circular_buffer_instance_init(cbh, (void*)array, sizeof(struct test), ARRAY_LEN);

//    for(int i = 0; i<1000; i++)
//    {

        RUN_TEST(test_push_buffer);
        RUN_TEST(test_put_pop);

        RUN_TEST(test_load_entire_buffer);
        RUN_TEST(test_load_partial_buffer);
//    }

    //RUN_TEST(test_circular_buffer_free);


    return UNITY_END();
}

// 
// End of file.
//

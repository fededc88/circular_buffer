#include "unity.h"

#include "TestBuffer.h"
#include "buffer.h"

char array[ARRAY_LEN];
circular_buffer_handler cbh;

char texto[ARRAY_LEN];

void setUp(void)
{
    char a = '0';
    for(int i = 0; i < ARRAY_LEN; i++){
	a++;
	if(a > 'z')
	    a = '0';
	
	texto[i] = a;
    }

    //printf(texto);
}

void tearDown(void)
{
}

void test_circular_buffer_initialization(void)
{

TEST_ASSERT_NOT_NULL_MESSAGE((cbh = circular_buffer_init((uint8_t *)array, ARRAY_LEN)),"cbh == NULL" );
TEST_ASSERT_EQUAL_UINT16_MESSAGE(ARRAY_LEN, circular_buffer_size(cbh), "Wrong buffer size");
}

void test_load_entire_buffer(void)
{
    char aux[ARRAY_LEN];

    for(int i = 0; i<ARRAY_LEN-1 ; i++)
	TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_put(cbh, texto[i]),"No more space");

    TEST_ASSERT_EQUAL_UINT16_MESSAGE(ARRAY_LEN-1, circular_buffer_count(cbh), "Wrong length");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_drop(cbh, aux, ARRAY_LEN-1), "couldent dump");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, circular_buffer_count(cbh), "Wrong buffer size");
    TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(0, texto, aux, ARRAY_LEN-1, aux);

}

void test_load_partial_buffer(void)
{
    char aux[ARRAY_LEN];

    for (int a = 1; a < ARRAY_LEN; a++)
    {
	for(int i = 0; i < a ; i++)
	    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_put(cbh, texto[i]),"No more space");

	TEST_ASSERT_EQUAL_UINT16_MESSAGE(a, circular_buffer_count(cbh), "Wrong length");
	TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_drop(cbh, aux, a), "couldent dump");
	TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, circular_buffer_count(cbh), "Wrong buffer size");
	TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(0, texto, aux, a, aux);
    }

}

void test_push_buffer(void)
{
    char aux[ARRAY_LEN];

    while(0 == circular_buffer_put(cbh, 'a'))
    {;}

    for(int i = 0; i < ARRAY_LEN-1; i++)
	circular_buffer_push(cbh, texto[i]);


    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_drop(cbh, aux, ARRAY_LEN -1 ), "couldent dump");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, circular_buffer_count(cbh), "Wrong buffer size");
    TEST_ASSERT_UINT8_ARRAY_WITHIN_MESSAGE(0, texto, aux, ARRAY_LEN-1, aux);
}

void test_put_pop(void)
{
    char aux;

    circular_buffer_reset(cbh);
    for(int a = 0; a < 100 ; a++)
    {
	for(int i = 0; i < ARRAY_LEN; i++)
	{
	    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_put(cbh, texto[i]),"No more space");
	    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, circular_buffer_pop(cbh, (uint8_t *) &aux), "pop");
	    TEST_ASSERT_EQUAL_CHAR_MESSAGE(texto[i],aux,"asd");

	}
    }
}

void test_circular_buffer_free(void)
{
TEST_ASSERT_NULL_MESSAGE(circular_buffer_free(&cbh), "cbh != NULL" );
}


/*
void test_AverageThreeBytes_should_AverageHighValues(void)
{
TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(70, 80, 90));
TEST_ASSERT_EQUAL_HEX8(127, AverageThreeBytes(127, 127, 127));
TEST_ASSERT_EQUAL_HEX8(84, AverageThreeBytes(0, 126, 126));
}
*/


int main(void)
{

UNITY_BEGIN();

RUN_TEST(test_circular_buffer_initialization);
for(int i = 0; i<1000; i++)
{
RUN_TEST(test_put_pop);

RUN_TEST(test_load_entire_buffer);
RUN_TEST(test_load_partial_buffer);
RUN_TEST(test_push_buffer);
}
RUN_TEST(test_circular_buffer_free);
//RUN_TEST(test_AverageThreeBytes_should_AverageHighValues);
return UNITY_END();
}

#include "../unity/src/unity.h"
#include "../src/include/calc.h"

void setUp()
{

}

void tearDown()
{

}

void test_add()
{
    int output = add(10, 20);
    TEST_ASSERT_EQUAL_MESSAGE(30, output, "this is an output");
}

void test_sub()
{
    int out = sub(30, 10);
    TEST_ASSERT_EQUAL(20, out);
    TEST_ASSERT_MESSAGE(out == 32, "something is not firght?:<");
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_add); 
    RUN_TEST(test_sub);

    UNITY_END();

    return 0;
}

#include "unity.h"
#include "../include/socketHandler.h"


void setUp() 
{

}

void tearDown()
{

}


void testSocketCreation() {
    int descript = bind_local_socket(1234);
    TEST_ASSERT_TRUE(descript >= 0);

    TEST_ASSERT_TRUE(socket_start_listen(descript, 0) < 0);

    int listen = socket_start_listen(descript, 2);
    TEST_ASSERT_TRUE(listen >= 0);

    TEST_ASSERT_TRUE(bind_local_socket(1234) < 0); // "created second socket should not work"

    TEST_ASSERT_TRUE(socket_start_listen(23103, 3) < 0);

    UnityPrint("testSocketCreation passed!");
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(testSocketCreation);
    return UNITY_END();
}

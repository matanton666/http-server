#include "../unity/src/unity.h"
#include "../src/include/socketHandler.h"

void setUp()
{

}

void tearDown()
{

}

void testSocketCreation()
{
    int descript = bind_local_socket(1234);
    TEST_ASSERT(descript >= 0);

    int listen = socket_start_listen(descript, 2);
    TEST_ASSERT(listen >= 0);

    TEST_ASSERT_MESSAGE(bind_local_socket(1234) < 0, "created second socket should not work");

}

int main(void)
{
    UNITY_BEGIN(); 

    RUN_TEST(testSocketCreation);


    UNITY_END();

    return 0;
}
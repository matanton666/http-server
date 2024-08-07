#include "../unity/src/unity.h"
#include "../src/include/requestParser.h"
#include <stdio.h>

void setUp()
{

}

void tearDown()
{

}

void testGetHttpVersion()
{
    char st[] = "GET / HTTP/1.1\nHost: 127.0.0.1:1234\nConnection: keep-alive";
    TEST_ASSERT(get_req_http_version(st) == V_ONE);
    st[13] = '2';
    TEST_ASSERT(get_req_http_version(st) == V_TWO);
    st[13] = '4';
    TEST_ASSERT(get_req_http_version(st) == V_INVALID);
}

 
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testGetHttpVersion);

    UNITY_END();

    return 0;
}
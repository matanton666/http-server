#include "../unity/src/unity.h"
#include "../src/include/requestParser.h"
#include <stdio.h>

char st[] = "DELETE /item/902 HTTP/1.1\nHost: 127.0.0.1:1234\nConnection: keep-alive";


void setUp()
{

}

void tearDown()
{

}

void testGetHttpVersion()
{
    char st[] = "DELETE /item/902 HTTP/1.1\nHost: 127.0.0.1:1234\nConnection: keep-alive";
    TEST_ASSERT(get_req_http_version(st) == V_ONE);
    st[24] = '2';
    TEST_ASSERT(get_req_http_version(st) == V_TWO);
    st[24] = '4';
    TEST_ASSERT(get_req_http_version(st) == V_INVALID);
    TEST_ASSERT(get_req_http_version("asd asdf asdf asdfasdfffdf") == V_INVALID);
    TEST_ASSERT(get_req_http_version("asd asdf asdf\n asdfasdfffdf") == V_INVALID);
}


void testRequestType()
{
    TEST_ASSERT(get_req_type("DELETE ASDF") == REQ_DELETE);
    TEST_ASSERT(get_req_type("GET ASDF") == REQ_GET);
    TEST_ASSERT(get_req_type("POST ASDF") == REQ_POST);
    TEST_ASSERT(get_req_type("PUT ASDF") == REQ_PUT);
    TEST_ASSERT(get_req_type("PUS asdf") == REQ_INVALID);
    TEST_ASSERT(get_req_type("a") == REQ_INVALID);
    TEST_ASSERT(get_req_type("") == REQ_INVALID);
}

 
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testGetHttpVersion);

    UNITY_END();

    return 0;
}
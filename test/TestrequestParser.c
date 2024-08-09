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

        // Test case with NULL input
    TEST_ASSERT(get_req_http_version(NULL) == V_INVALID);

    // Test case with empty string
    TEST_ASSERT(get_req_http_version("") == V_INVALID);

    // Test case with only spaces
    TEST_ASSERT(get_req_http_version("   ") == V_INVALID);

    // Test case with HTTP version at the beginning
    TEST_ASSERT(get_req_http_version("HTTP/1.1 GET /path") == V_ONE);

    // Test case with HTTP version at the end
    TEST_ASSERT(get_req_http_version("GET /path HTTP/1.1") == V_ONE);

    // Test case with multiple HTTP versions
    TEST_ASSERT(get_req_http_version("HTTP/1.1 GET /path HTTP/1.0") == V_ONE);

    // Test case with negative HTTP version
    TEST_ASSERT(get_req_http_version("HTTP/-1.1") == V_INVALID);

    // Test case with floating-point HTTP version
    TEST_ASSERT(get_req_http_version("HTTP/1.5") == V_INVALID);

}


void testRequestType() {
    TEST_ASSERT(get_req_type("DELETE ASDF") == REQ_DELETE);
    TEST_ASSERT(get_req_type("GET ASDF") == REQ_GET);
    TEST_ASSERT(get_req_type("POST ASDF") == REQ_POST);
    TEST_ASSERT(get_req_type("PUT ASDF") == REQ_PUT);
    TEST_ASSERT(get_req_type("PUS asdf") == REQ_INVALID);
    TEST_ASSERT(get_req_type("a") == REQ_INVALID);
    TEST_ASSERT(get_req_type("") == REQ_INVALID);

    // Test case with mixed case
    TEST_ASSERT(get_req_type("DELETE asdf") == REQ_DELETE);
    TEST_ASSERT(get_req_type("get ASDF") == REQ_INVALID);
    TEST_ASSERT(get_req_type("POST asdf") == REQ_POST);

    // Test case with leading/trailing spaces
    TEST_ASSERT(get_req_type("  DELETE ASDF") == REQ_DELETE);
    TEST_ASSERT(get_req_type("GET ASDF  ") == REQ_GET);

    // Test case with invalid request type
    TEST_ASSERT(get_req_type("PATCH ASDF") == REQ_INVALID);
    TEST_ASSERT(get_req_type("HEAD ASDF") == REQ_INVALID);
    TEST_ASSERT(get_req_type("OPTIONS ASDF") == REQ_INVALID);

    // Test case with NULL input
    TEST_ASSERT(get_req_type(NULL) == REQ_INVALID);

    // Test case with only spaces
    TEST_ASSERT(get_req_type("   ") == REQ_INVALID);

    // Test case with request type at the beginning
    TEST_ASSERT(get_req_type("GET /path") == REQ_GET);

    // Test case with request type at the end
    TEST_ASSERT(get_req_type("/path GET") == REQ_INVALID);

    // Test case with multiple request types
    TEST_ASSERT(get_req_type("GET /path POST /another") == REQ_GET);

    // Test case with request type and HTTP version
    TEST_ASSERT(get_req_type("GET /path HTTP/1.1") == REQ_GET);
}


 
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testGetHttpVersion);
    RUN_TEST(testRequestType);


    UNITY_END();

    return 0;
}
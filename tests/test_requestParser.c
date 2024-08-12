#include "unity.h"
#include "../include/requestParser.h"
#include "../include/hashTable.h"

void setUp() 
{

}

void tearDown()
{

}


void testGetHttpVersion() {
    char st[] = "DELETE /item/902 HTTP/1.1\nHost: 127.0.0.1:1234\nConnection: keep-alive";
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_ONE, get_req_http_version(st), "Failed to get HTTP version 1.1");
    st[24] = '2';
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_TWO, get_req_http_version(st), "Failed to get HTTP version 1.2");
    st[24] = '4';
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_INVALID, get_req_http_version(st), "Failed to get invalid HTTP version");
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_INVALID, get_req_http_version("asd asdf asdf asdfasdfffdf"), "Failed to get invalid HTTP version");
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_INVALID, get_req_http_version("asd asdf asdf\n asdfasdfffdf"), "Failed to get invalid HTTP version");

    TEST_ASSERT_EQUAL_INT_MESSAGE(V_INVALID, get_req_http_version(NULL), "Failed to get invalid HTTP version for NULL input");
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_INVALID, get_req_http_version(""), "Failed to get invalid HTTP version for empty input");
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_INVALID, get_req_http_version("   "), "Failed to get invalid HTTP version for whitespace input");
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_ONE, get_req_http_version("HTTP/1.1 GET /path"), "Failed to get HTTP version 1.1");
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_ONE, get_req_http_version("GET /path HTTP/1.1"), "Failed to get HTTP version 1.1");
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_ONE, get_req_http_version("HTTP/1.1 GET /path HTTP/1.0"), "Failed to get HTTP version 1.1");
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_INVALID, get_req_http_version("HTTP/-1.1"), "Failed to get invalid HTTP version");
    TEST_ASSERT_EQUAL_INT_MESSAGE(V_INVALID, get_req_http_version("HTTP/1.5"), "Failed to get invalid HTTP version");
}

void testRequestType() {
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_DELETE, get_req_type("DELETE ASDF"), "Failed to get DELETE request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_GET, get_req_type("GET ASDF"), "Failed to get GET request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_POST, get_req_type("POST ASDF"), "Failed to get POST request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_PUT, get_req_type("PUT ASDF"), "Failed to get PUT request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type("PUS asdf"), "Failed to get invalid request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type("a"), "Failed to get invalid request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type(""), "Failed to get invalid request type");

    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_DELETE, get_req_type("DELETE asdf"), "Failed to get DELETE request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type("get ASDF"), "Failed to get invalid request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_POST, get_req_type("POST asdf"), "Failed to get POST request type");

    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_DELETE, get_req_type("  DELETE ASDF"), "Failed to get DELETE request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_GET, get_req_type("GET ASDF  "), "Failed to get GET request type");

    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type("PATCH ASDF"), "Failed to get invalid request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type("HEAD ASDF"), "Failed to get invalid request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type("OPTIONS ASDF"), "Failed to get invalid request type");

    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type(NULL), "Failed to get invalid request type for NULL input");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type("   "), "Failed to get invalid request type for whitespace input");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_GET, get_req_type("GET /path"), "Failed to get GET request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_INVALID, get_req_type("/path GET"), "Failed to get invalid request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_GET, get_req_type("GET /path POST /another"), "Failed to get GET request type");
    TEST_ASSERT_EQUAL_INT_MESSAGE(REQ_GET, get_req_type("GET /path HTTP/1.1"), "Failed to get GET request type");
}

void testHeadersParser() {
    char req[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n{\n  'name': 'John Doe',\n  'email': 'john.doe@example.com'\n}";
    HashTable* tbl = parse_req_headers(req);

    TEST_ASSERT_EQUAL_STRING_MESSAGE("example.com", search(tbl, "Host"), "Failed to get Host header value");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("application/json", search(tbl, "Content-Type"), "Failed to get Content-Type header value");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("49", search(tbl, "Content-Length"), "Failed to get Content-Length header value");
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(testGetHttpVersion);
    RUN_TEST(testRequestType);
    // RUN_TEST(testHeadersParser);
    //todo: fix function (there is a segfault probably null deref)
    return UNITY_END();
}

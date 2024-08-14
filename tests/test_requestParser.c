#include "unity.h"
#include "../include/requestParser.h"
#include "../include/hashTable.h"
#include "unity_internals.h"
#include <time.h>

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
    free_table(tbl);


    // Test case with empty headers
    char req1[] = "GET / HTTP/1.1\n\n";
    HashTable* tbl1 = parse_req_headers(req1);
    TEST_ASSERT_NULL_MESSAGE(search(tbl1, "Host"), "Expected NULL for empty headers");
    free_table(tbl1);

    // Test case with missing newline after headers
    char req2[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49{\n  'name': 'John Doe',\n  'email': 'john.doe@example.com'\n";
    HashTable* tbl2 = parse_req_headers(req2);
    TEST_ASSERT_EQUAL_STRING(search(tbl2, "Host"), "example.com");
    TEST_ASSERT_EQUAL_STRING(search(tbl2, "  'name'"), "'John Doe',");
    free_table(tbl2);

    // Test case with duplicate headers
    char req3[] = "GET / HTTP/1.1\nHost: example.com\nHost: example.org\n\n";
    HashTable* tbl3 = parse_req_headers(req3);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("example.org", search(tbl3, "Host"), "Expected last duplicate header value");
    free_table(tbl3);

    // Test case with mixed casing for header names
    char req4[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-type: application/json\nContent-Length: 49\n\n{\n  'name': 'John Doe',\n  'email': 'john.doe@example.com'\n}";
    HashTable* tbl4 = parse_req_headers(req4);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("application/json", search(tbl4, "Content-type"), "Failed to get Content-Type header value");
    free_table(tbl4);
}


void test_request_validation() 
{
    // Existing test cases
    char req[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n{\n  'name': 'John Doe}\n  this is some example of possible data";
    TEST_ASSERT_TRUE(validate_req_syntax(req));

    char req3[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49";
    TEST_ASSERT_FALSE(validate_req_syntax(req3));

    char req1[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n";
    TEST_ASSERT_TRUE(validate_req_syntax(req1));

    char req2[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n";
    TEST_ASSERT_TRUE(validate_req_syntax(req2));

    // Additional test cases
    // 1. GET request with no body
    char req4[] = "GET /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 0\n\n";
    TEST_ASSERT_TRUE(validate_req_syntax(req4));

    // 3. Missing HTTP version
    char req6[] = "POST /api/users\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n";
    TEST_ASSERT_FALSE(validate_req_syntax(req6)); // Invalid because HTTP version is missing

    // 4. Invalid HTTP version
    char req7[] = "POST /api/users HTTP/2.0\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n";
    TEST_ASSERT_FALSE(validate_req_syntax(req7)); // Invalid because it's HTTP/2.0, not HTTP/1.x

    // 5. Malformed request line (no method)
    char req8[] = "/api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n";
    TEST_ASSERT_FALSE(validate_req_syntax(req8)); // Invalid because method is missing

    // 6. Malformed request line (no resource)
    char req9[] = "POST HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n";
    TEST_ASSERT_FALSE(validate_req_syntax(req9)); // Invalid because resource is missing

    // 7. Malformed request line (extra spaces)
    char req10[] = "POST  /api/users  HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n";
    TEST_ASSERT_FALSE(validate_req_syntax(req10)); // Invalid because of extra spaces in the request line

    // 11. Valid GET request with query parameters
    char req14[] = "GET /api/users?id=123&name=John HTTP/1.1\nHost: example.com\n\n";
    TEST_ASSERT_TRUE(validate_req_syntax(req14)); // Valid GET request with query parameters

    // 12. Invalid request method (unsupported method)
    char req15[] = "ASDF /api/users HTTP/1.1\nHost: example.com\n\n";
    TEST_ASSERT_FALSE(validate_req_syntax(req15)); // Invalid because TRACE is not typically supported

    // 13. Case sensitivity check in method and HTTP version
    char req16[] = "get /api/users http/1.1\nHost: example.com\n\n";
    TEST_ASSERT_FALSE(validate_req_syntax(req16)); // Invalid because method and HTTP version should be case-sensitive (GET and HTTP/1.1)

    // 16. Valid request with chunked transfer encoding
    char req19[] = "POST /api/users HTTP/1.1\nHost: example.com\nTransfer-Encoding: chunked\n\n4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n";
    TEST_ASSERT_TRUE(validate_req_syntax(req19)); // Valid request with chunked transfer encoding

    // 18. Valid request with custom headers
    char req21[] = "GET /api/users HTTP/1.1\nHost: example.com\nX-Custom-Header: custom-value\n\n";
    TEST_ASSERT_TRUE(validate_req_syntax(req21)); // Valid request with a custom header

    // 20. Valid request with an empty line after the headers but before the body
    char req23[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n\n{\"name\":\"John\"}";
    TEST_ASSERT_TRUE(validate_req_syntax(req23)); // Valid despite the extra newline between headers and body
    
}


void test_url_parser()
{
    char req[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n{\n  'name': 'John Doe}\n  this is some example of possible data";
    HashTable* tbl = parse_req_headers(req);
    url_t* url = parse_req_url(req, tbl);
    TEST_ASSERT_EQUAL_STRING("example.com/api/users", url->domain);
    TEST_ASSERT_EQUAL_STRING("/api/users", url->path);
    TEST_ASSERT_EQUAL_STRING("", url->query);

    TEST_ASSERT_EQUAL_INT(11, url->domain_len);
    TEST_ASSERT_EQUAL_INT(10, url->path_len);
    free_table(tbl);
    free(url->domain);
    free(url);

    char req2[] = "GET / HTTP/1.1\nHost: example.com\nHost: example.org\n\n";
    tbl = parse_req_headers(req2);
    url = parse_req_url(req2, tbl);

    TEST_ASSERT_EQUAL_STRING("example.org/", url->domain);
    TEST_ASSERT_EQUAL_STRING("/", url->path);
    TEST_ASSERT_EQUAL_STRING("", url->query);

    TEST_ASSERT_EQUAL_INT(11, url->domain_len);
    free_table(tbl);
    free(url->domain);
    free(url);   

    char req3[] = "GET /path?query=value HTTP/1.1\nHost: example.com\n\n";
    tbl = parse_req_headers(req3);
    url = parse_req_url(req3, tbl);
    TEST_ASSERT_EQUAL_STRING("example.com/path?query=value", url->domain);
    TEST_ASSERT_EQUAL_STRING("/path?query=value", url->path);
    TEST_ASSERT_EQUAL_STRING("query=value", url->query);
    TEST_ASSERT_EQUAL_INT(11, url->domain_len);
    TEST_ASSERT_EQUAL_INT(5, url->path_len);
    TEST_ASSERT_EQUAL_INT(11, strlen(url->query));
    free_table(tbl);
    free(url->domain);
    free(url);


    char req5[] = "GET /path HTTP/1.1\nHost:\n\n";
    tbl = parse_req_headers(req5);
    url = parse_req_url(req5, tbl);
    TEST_ASSERT_NULL(url);


}


void test_all()
{
    char req[] = "GET /path?query=value HTTP/1.1\nHost: example.com\nContent-type: application/json\n\n";
    TEST_ASSERT_TRUE(validate_req_syntax(req));
    HashTable* tbl = parse_req_headers(req);
    TEST_ASSERT_NOT_NULL(tbl);
    url_t* url = parse_req_url(req, tbl);
    TEST_ASSERT_NOT_NULL(url);

    TEST_ASSERT_EQUAL_STRING("query=value",url->query);
   // Test case for an empty request
    char req_empty[] = "";
    TEST_ASSERT_FALSE(validate_req_syntax(req_empty));
    tbl = parse_req_headers(req_empty);
    TEST_ASSERT_NULL(tbl);
    url = parse_req_url(req_empty, tbl);
    TEST_ASSERT_NULL(url);

    // Test case for a request with multiple query parameters
    char req_multiple_queries[] = "GET /path?query1=value1&query2=value2 HTTP/1.1\nHost: example.com\n\n";
    TEST_ASSERT_TRUE(validate_req_syntax(req_multiple_queries));
    tbl = parse_req_headers(req_multiple_queries);
    TEST_ASSERT_NOT_NULL(tbl);
    url = parse_req_url(req_multiple_queries, tbl);
    TEST_ASSERT_NOT_NULL(url);
    TEST_ASSERT_EQUAL_STRING("query1=value1&query2=value2", url->query);
    
}

 

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(testGetHttpVersion);
    RUN_TEST(testRequestType);
    RUN_TEST(testHeadersParser);
    RUN_TEST(test_request_validation);
    RUN_TEST(test_url_parser);
    RUN_TEST(test_all);
    return UNITY_END();
}

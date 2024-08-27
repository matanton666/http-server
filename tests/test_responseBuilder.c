#include "unity.h"
#include "../include/responseBuilder.h"
#include "unity_internals.h"
#include <string.h>

void setUp(void) {}

void tearDown(void) {}

void test_build_response_should_create_valid_response_for_200_status_code(void)
{
    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");
    char* body = "Hello, World!";

    response_t* resp = build_response(200, headers, body);

    TEST_ASSERT_EQUAL_STRING("200", resp->status_code);
    TEST_ASSERT_EQUAL_STRING("OK", resp->reason_phrase);
    TEST_ASSERT_EQUAL_PTR(headers, resp->headers);
    TEST_ASSERT_EQUAL_STRING(body, resp->body);

    free_response(resp);
}

void test_build_response_should_create_valid_response_for_404_status_code(void)
{
    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");

    response_t* resp = build_response(404, headers, NULL);

    TEST_ASSERT_EQUAL_STRING("404", resp->status_code);
    TEST_ASSERT_EQUAL_STRING("Not Found", resp->reason_phrase);
    TEST_ASSERT_EQUAL_PTR(headers, resp->headers);
    TEST_ASSERT_NULL(resp->body);

    free_response(resp);
}

void test_response_to_str_should_convert_response_to_string(void)
{
    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");
    insert(headers, "Content-Length", "12");
    char* body = "Hello, World";

    response_t* resp = build_response(200, headers, body);

    char* expected_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello, World";
    char* actual_response = response_to_str(resp);

    TEST_ASSERT_EQUAL_STRING(expected_response, actual_response);

    free(actual_response);
    free_response(resp);
}


void test_build_response_should_handle_invalid_status_code(void)
{
    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");

    response_t* resp = build_response(999, headers, NULL);

    TEST_ASSERT_EQUAL_STRING("500", resp->status_code);
    TEST_ASSERT_EQUAL_STRING("Internal Server Error", resp->reason_phrase);
    TEST_ASSERT_EQUAL_PTR(headers, resp->headers);
    TEST_ASSERT_NULL(resp->body);

    free_response(resp);
}

void test_build_response_should_handle_null_headers(void)
{
    response_t* resp = build_response(200, NULL, "Hello, World!");

    TEST_ASSERT_EQUAL_STRING("200", resp->status_code);
    TEST_ASSERT_EQUAL_STRING("OK", resp->reason_phrase);
    TEST_ASSERT_NULL(resp->headers);
    TEST_ASSERT_EQUAL_STRING("Hello, World!", resp->body);

    free_response(resp);
}

void test_response_to_str_should_handle_null_body(void)
{
    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");

    response_t* resp = build_response(404, headers, NULL);

    char* expected_response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
    char* actual_response = response_to_str(resp);

    TEST_ASSERT_EQUAL_STRING(expected_response, actual_response);

    free(actual_response);
    free_response(resp);
}

void test_response_to_str_should_handle_null_headers(void)
{
    response_t* resp = build_response(200, NULL, "Hello, World!");

    char* expected_response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    char* actual_response = response_to_str(resp);

    TEST_ASSERT_EQUAL_STRING(expected_response, actual_response);

    free(actual_response);
    free_response(resp);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_build_response_should_create_valid_response_for_200_status_code);
    RUN_TEST(test_build_response_should_create_valid_response_for_404_status_code);
    RUN_TEST(test_response_to_str_should_convert_response_to_string);
    RUN_TEST(test_response_to_str_should_handle_null_body);
    RUN_TEST(test_response_to_str_should_handle_null_headers);
    // RUN_TEST(test_build_response_should_handle_null_headers);

    return UNITY_END();
}

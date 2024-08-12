#include "../include/requestParser.h"
#include "../include/hashTable.h"
#include <assert.h>
#include <stdio.h>


void testGetHttpVersion() {
    char st[] = "DELETE /item/902 HTTP/1.1\nHost: 127.0.0.1:1234\nConnection: keep-alive";
    printf("got here****************************************");
    assert(get_req_http_version(st) == V_ONE);
    st[24] = '2';
    assert(get_req_http_version(st) == V_TWO);
    st[24] = '4';
    assert(get_req_http_version(st) == V_INVALID);
    assert(get_req_http_version("asd asdf asdf asdfasdfffdf") == V_INVALID);
    assert(get_req_http_version("asd asdf asdf\n asdfasdfffdf") == V_INVALID);

    assert(get_req_http_version(NULL) == V_INVALID);
    assert(get_req_http_version("") == V_INVALID);
    assert(get_req_http_version("   ") == V_INVALID);
    assert(get_req_http_version("HTTP/1.1 GET /path") == V_ONE);
    assert(get_req_http_version("GET /path HTTP/1.1") == V_ONE);
    assert(get_req_http_version("HTTP/1.1 GET /path HTTP/1.0") == V_ONE);
    assert(get_req_http_version("HTTP/-1.1") == V_INVALID);
    assert(get_req_http_version("HTTP/1.5") == V_INVALID);

    printf("testGetHttpVersion passed!\n");
}

void testRequestType() {
    assert(get_req_type("DELETE ASDF") == REQ_DELETE);
    assert(get_req_type("GET ASDF") == REQ_GET);
    assert(get_req_type("POST ASDF") == REQ_POST);
    assert(get_req_type("PUT ASDF") == REQ_PUT);
    assert(get_req_type("PUS asdf") == REQ_INVALID);
    assert(get_req_type("a") == REQ_INVALID);
    assert(get_req_type("") == REQ_INVALID);

    assert(get_req_type("DELETE asdf") == REQ_DELETE);
    assert(get_req_type("get ASDF") == REQ_INVALID);
    assert(get_req_type("POST asdf") == REQ_POST);

    assert(get_req_type("  DELETE ASDF") == REQ_DELETE);
    assert(get_req_type("GET ASDF  ") == REQ_GET);

    assert(get_req_type("PATCH ASDF") == REQ_INVALID);
    assert(get_req_type("HEAD ASDF") == REQ_INVALID);
    assert(get_req_type("OPTIONS ASDF") == REQ_INVALID);

    assert(get_req_type(NULL) == REQ_INVALID);
    assert(get_req_type("   ") == REQ_INVALID);
    assert(get_req_type("GET /path") == REQ_GET);
    assert(get_req_type("/path GET") == REQ_INVALID);
    assert(get_req_type("GET /path POST /another") == REQ_GET);
    assert(get_req_type("GET /path HTTP/1.1") == REQ_GET);

    printf("testRequestType passed!\n");
}

void testHeadersParser() {
    char req[] = "POST /api/users HTTP/1.1\nHost: example.com\nContent-Type: application/json\nContent-Length: 49\n\n{\n  'name': 'John Doe',\n  'email': 'john.doe@example.com'\n}";
    HashTable* tbl = parse_req_headers(req);

    assert(strcmp(search(tbl, "Host"), "example.com") == 0);
    assert(strcmp(search(tbl, "Content-Type"), "application/json") == 0);
    assert(strcmp(search(tbl, "Content-Length"), "49") == 0);
}

int main(void) {
    testGetHttpVersion();
    testRequestType();
    // testHeadersParser();

    return 0;
}

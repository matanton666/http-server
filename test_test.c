#include "../include/socketHandler.h"
#include <assert.h>

void testSocketCreation() {
    int descript = bind_local_socket(1234);
    assert(descript >= 0);

    assert(socket_start_listen(descript, 0) < 0);

    int listen = socket_start_listen(descript, 2);
    assert(listen >= 0);

    assert(bind_local_socket(1234) < 0); // "created second socket should not work"

    assert(socket_start_listen(23103, 3) < 0);
    

    printf("testSocketCreation passed!\n");
}

int main(void) {
    testSocketCreation();

    return 0;
}


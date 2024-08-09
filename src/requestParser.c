
#include "include/requestParser.h"
#include <string.h>

http_version get_req_http_version(char *req)
{
    char search[] = "HTTP/1.";
    int curr = 0;

    while (curr <= strlen(req) && req[curr] != '\n') {
        if (req[curr] == search[0]){
            // validate that there is http/1.
            if (strncmp(search, req + curr, strlen(search)) == 0) {
                curr += strlen(search);
                switch (*(req + curr)) {
                    case '0':
                        return V_ZERO;
                    case '1':
                        return V_ONE;
                    case '2':
                        return V_TWO;
                    default:
                        return V_INVALID;
                }
            }
        }
        curr++;
    }
    return V_INVALID;
}



#include "../../include/helpers.h"


clock_t timer_start() {
    return clock();
}


void print_timer_end(const char* msg, clock_t start) {
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n%s: %f\n", msg, cpu_time_used);
}

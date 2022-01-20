#include <stdio.h>

#define LAPS 100

void neumann(int n, int laps) {
    for (int i = 0; i < LAPS; i++) {
        printf("i=%d; %04d\n", i, n);
        n = ((n * n) / 100) % 10000;
    }
}

int main() {

    printf("###########1###########\n");
    neumann(1234, 100); // converges towards 0, reaches it at i=56

    printf("###########2###########\n");
    neumann(4100, 100); // is stuck in a short period at i%4==0 (4100, 8100, 6100, 2100, ...)

    return 0;
}

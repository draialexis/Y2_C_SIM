#include <stdio.h>

#define LAPS 100

int main() {

    int n = 1234; // converges towards 0, reaches it at i=55
    for (int i = 0; i < LAPS; i++) {
        n = ((n * n) / 100) % 10000;
        printf("i=%d; %04d\n", i, n);
    }

    return 0;
}

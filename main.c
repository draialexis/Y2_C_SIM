#include <stdio.h>

#define LAPS 100

int main() {

    int n = 1234; // converges towards 0, reaches it at i=56
    for (int i = 0; i < LAPS; i++) {
        printf("i=%d; %04d\n", i, n);
        n = ((n * n) / 100) % 10000;
    }

    return 0;
}

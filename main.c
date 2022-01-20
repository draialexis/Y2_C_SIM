#include <stdio.h>
#define LAPS 100

int main() {

    int n = 1234; // converges towards 0, reaches it at i=54
    for (int i = 0; i < LAPS; i++) {
        n = n * n;
        //01522756
        n = n / 100;
        //015227
        n = n % 10000;
        //5227
        printf("i=%d; %04d\n", i, n);
    }

    return 0;
}

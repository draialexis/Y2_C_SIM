#include <stdio.h>
#include <stdlib.h>

#define LAPS 100

void neumann(int n, int laps) {
    printf("init Neumann\n");
    for (int i = 0; i < laps; i++) {
        printf("i=%d; %04d\n", i, n);
        n = ((n * n) / 100) % 10000;
    }
}

int main() {

    printf("###########1###########\n");
    neumann(1234, LAPS); // converges towards 0, reaches it at i=56

    printf("###########2###########\n");
    neumann(4100, LAPS); // is stuck in a short period at i%4==0 (4100, 8100, 6100, 2100, ...)
    neumann(1324, LAPS); // reaches 4100 at i=92, and gets stuck in the above-mentioned period then
    neumann(1301, LAPS); // reaches 6100 at i=10, and gets stuck in the above-mentioned period then
    neumann(3141, LAPS); // reaches 100 at i=7 and just stays there

    printf("###########3###########\n");
    /* as a "pseudo-random number generator", rand() doesn't seem appropriate for scientific uses.
     * basically, srand() will always give the same number -- given the same seed.
     * good for reproducibility => good for testing -- but not good if you need randomness
     * also, some OSs and some C implementations have a problem with lower-order bits being not "as random"
     */

    printf("###########4###########\n");


    return 0;
}

//
// Created by aldrai on 27/01/2022.
//

#include <stdio.h>
#include "../matsumoto/mt19937ar.h"

int main() {
    unsigned long init[4] = {0x123, 0x234, 0x345, 0x456};
    int length = 4;
    init_by_array(init, length);

    /*
    printf("1000 outputs of genrand_int32()\n");
    for (int i = 0; i < 1000; i++) {
        printf("%10lu ", genrand_int32());
        if (i % 5 == 4) printf("\n");
    }
    printf("\n1000 outputs of genrand_real2()\n");
    for (int i = 0; i < 1000; i++) {
        printf("%10.8f ", genrand_real2());
        if (i % 5 == 4) printf("\n");
    }
    */

    return 0;
}
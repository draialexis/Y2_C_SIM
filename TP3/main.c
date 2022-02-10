//
// Created by draia on 10/02/2022.
//

#include <math.h>
#include <stdio.h>
#include "mt.h"

#define DEBUG printf("file %s; line %d\n", __FILE__, __LINE__);
#define FAIL_OUT DEBUG exit(EXIT_FAILURE);
#define MALLOC_FAIL printf("!_malloc failed_!\n"); FAIL_OUT

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   ANNEXES     /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   BUSINESS    /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

double simPi(int inPoints)
{
    double   m, x, y;
    for (int i = 0; i < inPoints; i++)
    {
        x = genrand_real1();
        y = genrand_real1();
        if (pow(x, 2) + pow(y, 2) < 1)
        {
            m++;
        }
    }
    return (m / inPoints) * 4;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   TESTS       /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    // init by M.M.
    int           i, j, k;
    unsigned long init[4] = {0x123, 0x234, 0x345, 0x456};
    int           length  = 4;

    init_by_array(init, length);

    printf("########### 1 ###########\n");

    printf("1000 outputs of genrand_int32()\n");
    for (i = 0; i < 1000; i++)
    {
        printf("%10lu ", genrand_int32());
        if (i % 5 == 4) printf("\n");
    }
    printf("\n1000 outputs of genrand_real2()\n");
    for (i = 0; i < 1000; i++)
    {
        printf("%10.8f ", genrand_real2());
        if (i % 5 == 4) printf("\n");
    }
    // making sure that it checks out compared to ../matsumoto/mt19937ar.out

    printf("########### 2 ###########\n");
    double res1;
    for (i = 1000; i <= 1000000000; i *= 1000)
    {
        res1 = simPi(i);
        printf("Pi approx, at i = %d: %10.8f\n", i, res1);
    }

    return 0;
}
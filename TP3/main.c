//
// Created by draia on 10/02/2022.
//

#include <math.h>
#include <stdio.h>
#include "mt.h"

#define DEBUG printf("file %s; line %d\n", __FILE__, __LINE__);
#define FAIL_OUT DEBUG exit(EXIT_FAILURE);
#define MALLOC_FAIL printf("!_malloc failed_!\n"); FAIL_OUT

/*-------------------------------------------------------------------------------*/
/*-----------------------------    ANNEXES    -----------------------------------*/
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
/*-----------------------------   BUSINESS    -----------------------------------*/
/*-------------------------------------------------------------------------------*/

/**
 * simPi
 * uses a Monte Carlo simulation to approximate pi
 * <br><em>effective, but not so efficient</em>
 * -- gets quite slow past 10^7 points (i.e. 3-decimal precision)
 * @param inPoints number of random points used to approximate pi
 * @return said approximation of pi
 */
double simPi(int inPoints)
{
    double m = 0, x, y;
    int    i;
    for (i = 0; i < inPoints; i++)
    {
        x = genrand_real1();
        y = genrand_real1();
        if (pow(x, 2) + pow(y, 2) < 1)
        {
            m++;
        }
    }
    return (m / (double) inPoints) * 4;
}

/*-------------------------------------------------------------------------------*/
/*-----------------------------     TESTS     -----------------------------------*/
/*-------------------------------------------------------------------------------*/

int main(void)
{
    // init by M.M.
    int           i;
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

    printf("M-C Pi approx., with %d points: %10.8f\n", 1000, simPi(1000));
    printf("M-C Pi approx., with %d points: %10.8f\n", 1000000, simPi(1000000));
    printf("M-C Pi approx., with %d points: %10.8f\n", 1000000000, simPi(1000000000));

    return 0;
}
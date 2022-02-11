//
// Created by draia on 10/02/2022.
//

#include <math.h>
#include <stdio.h>
#include "mt.h"

#define DEBUG printf("file %s; line %d\n", __FILE__, __LINE__);
#define FAIL_OUT DEBUG exit(EXIT_FAILURE);
#define MALLOC_FAIL printf("!_malloc failed_!\n"); FAIL_OUT

#define MAX_POINTS 1000000000
#define SIZE_2 30

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
double simPi(unsigned long long inPoints)
{
    double             x, y, m = 0;
    unsigned long long i_l     = 0;
    while (i_l++ < inPoints)
    {
        x = genrand_real1();
        y = genrand_real1();
        if ((x * x) + (y * y) < 1)
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

    printf("###################### 0 ######################\n");

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

    printf("\n###################### 1 ######################\n");
    printf("\nHi there, person correcting this. You're gonna be there a while, simPi() is slow.\n"
           "For your convenience, the pre-processor constant MAX_POINTS can be decreased (on line 13).\n"
           "Thank you for your time!\n\n");
    unsigned long long i_l;
    i_l = 1000;
    while (i_l <= MAX_POINTS)
    {
        printf("M-C Pi approx., with %llu points: %10.8f\n", i_l, simPi(i_l));
        i_l *= 10;
    }

    printf("\n###################### 2 ######################\n");

    double res2[SIZE_2];
    double mean2, tmp, err2, rel_err2;
    i_l = 1000;
    while (i_l <= MAX_POINTS)
    {
        mean2  = 0.0;
        for (i = 0; i < SIZE_2; i++)
        {
            tmp = simPi(i_l);
            res2[i] = tmp;
            mean2 += tmp;
        }
        printf("\nM-C Pi approx., with %llu points, sample size = %d\n", i_l, SIZE_2);
        for (i = 0; i < SIZE_2; i++)
        {
            printf("%10.8f, ", res2[i]);
        }

        mean2 /= SIZE_2;
        printf("mean = %10.8f\n", mean2);
        err2 = fabs(mean2 - M_PI);
        printf("absolute error = %10.8f\n", err2);
        rel_err2 = err2 / M_PI;
        printf("relative error = %10.8f\n", rel_err2);

        i_l *= 1000;
    }

    return 0;
}
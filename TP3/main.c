//
// Created by draia on 10/02/2022.
//

#include <math.h>
#include <stdio.h>
#include "mt.h"

#define MAX_POINTS 1000000000
//#define MAX_POINTS 10000000 //faster
#define MIL 1000000
#define SIZE_2 30
#define SIZE_2_MAX 30
#define SIZE_3_MAX 30

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
    int           i, j;
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

    printf("\nHi there, person correcting this. You might be here a while, simPi() is pretty slow.\n"
           "To speed up execution, you may comment line 9 and uncomment line 10.\n"
           "Thank you for your time!\n\n");

    printf("\nTesting simPi() with varying # of points, trying to reach values close to pi\n");

    unsigned long long i_l;
    i_l = 1000;
    while (i_l <= MAX_POINTS)
    {
        printf("%llu points: %10.8f\n", i_l, simPi(i_l));
        i_l *= 10;
        // going power of 10 by power of 10
        // to look for intervals where decimal precision improves past certain thresholds
    }

    printf("\n###################### 2 ######################\n");

    double res2[SIZE_2];
    double mean2, tmp, err2, rel_err2;
    i_l = 1000;

    printf("\nTesting with varying # of points, introducing a constant sample size > 1 "
           "to obtain a mean value closer to pi\n");
    while (i_l <= MAX_POINTS)
    {
        mean2 = 0.0;

        for (i = 0; i < SIZE_2; i++) // running SIZE_2 replicates, instead of 1
        {
            tmp = simPi(i_l);
            res2[i] = tmp;
            mean2 += tmp;
        }
        printf("\n%llu points, sample size = %d\n", i_l, SIZE_2);
        for (i = 0; i < SIZE_2; i++)
        {
            printf("%10.8f, ", res2[i]);
        }

        mean2 /= SIZE_2;
        printf("\nmean = %10.8f\n", mean2);
        err2 = fabs(mean2 - M_PI);
        printf("absolute error = %10.8f\n", err2);
        rel_err2 = err2 / M_PI;
        printf("relative error = %10.8f\n", rel_err2);

        i_l *= 1000;
    }

    printf("\nNow testing the impact of sample size on mean and error, keeping # of points constant\n");
    for (i = 10; i <= SIZE_2_MAX; i += 10)
    {
        mean2 = 0.0;

        for (j = 0; j < i; j++)
        {
            tmp = simPi(MIL);
            mean2 += tmp;
        }
        printf("\n%d points, sample size = %d\n", MIL, i);
        mean2 /= i;
        printf("mean = %10.8f\n", mean2);
        err2 = fabs(mean2 - M_PI);
        printf("absolute error = %10.8f\n", err2);
        rel_err2 = err2 / M_PI;
        printf("relative error = %10.8f\n", rel_err2);
    }

    printf("\n###################### 3 ######################\n");

    // See whether the number of replicates improves your results and decreases the confidence radius (comparison with M_PI).

    printf("\nNow testing the impact of sample size on confidence radius, keeping # of points constant\n");

    double t_vals[30] =
                   {
                           12.706, 4.303, 3.182, 2.776, 2.571, 2.447, 2.365, 2.308, 2.262, 2.228,
                           2.201, 2.179, 2.160, 2.145, 2.131, 2.120, 2.110, 2.101, 2.093, 2.086,
                           2.080, 2.074, 2.069, 2.064, 2.060, 2.056, 2.052, 2.048, 2.045, 2.042
                   };

    double res3[SIZE_3_MAX];
    double conf_rads3[SIZE_3_MAX];
    double mean3, est_var3, std_err3, std_dev3;

    for (i = 2; i <= SIZE_3_MAX; i++)
    {
        mean3    = 0.0;
        est_var3 = 0.0;

        for (j = 0; j < i; j++)
        {
            tmp = simPi(MIL);
            res3[j] = tmp;
            mean3 += tmp;
        }
        printf("\n%d points, sample size = %d\n", MIL, i);
        mean3 /= i;
        printf("mean = %10.8f\n", mean3);

        for (j = 0; j < i; j++)
        {
            est_var3 += pow(res3[j] - mean3, 2);
        }
        est_var3 /= i - 1;
        printf("estimated variance = %10.8f\n", est_var3);
        std_dev3 = sqrt(est_var3);
        printf("standard deviation = %10.8f\n", std_dev3);
        std_err3 = std_dev3 / sqrt(i);
        printf("standard error = %10.8f\n", std_err3);
        conf_rads3[i - 2] = std_err3 * t_vals[i - 1];
        printf("confidence radius = %10.8f\n", conf_rads3[i - 2]);
        printf("confidence interval = [%10.8f, %10.8f]\n",
               mean3 - conf_rads3[i - 2],
               mean3 + conf_rads3[i - 2]);
    }

    for (i = 0; i < SIZE_3_MAX - 1; i++)
    {
        printf("%10.8f, ", conf_rads3[i]);
    }
    printf("\n");

    return 0;
}
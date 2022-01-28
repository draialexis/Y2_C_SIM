/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.
   Copyright (C) 2005, Mutsuo Saito,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#include <stdio.h>
#include <stdlib.h>

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int           mti = N + 1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
    mt[0] = s & 0xffffffffUL;
    for (mti = 1; mti < N; mti++)
    {
        mt[mti] =
                (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i = 1;
    j = 0;
    k = (N > key_length ? N : key_length);
    for (; k; k--)
    {
        mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525UL))
                + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        j++;
        if (i >= N)
        {
            mt[0] = mt[N - 1];
            i = 1;
        }
        if (j >= key_length) j = 0;
    }
    for (k = N - 1; k; k--)
    {
        mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941UL))
                - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i >= N)
        {
            mt[0] = mt[N - 1];
            i = 1;
        }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long        y;
    static unsigned long mag01[2] = {0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N)
    { /* generate N words at one time */
        int kk;

        if (mti == N + 1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk = 0; kk < N - M; kk++)
        {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (; kk < N - 1; kk++)
        {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
        mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }

    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void)
{
    return (long) (genrand_int32() >> 1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
    return genrand_int32() * (1.0 / 4294967295.0);
    /* divided by 2^32-1 */
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
    return genrand_int32() * (1.0 / 4294967296.0);
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
    return (((double) genrand_int32()) + 0.5) * (1.0 / 4294967296.0);
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void)
{
    unsigned long a = genrand_int32() >> 5, b = genrand_int32() >> 6;
    return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

/* These real versions are due to Isaku Wada, 2002/01/09 added */
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Created by draia on 27/01/2022.
//

#include <math.h>

// let me have this!
#define TAU (2*M_PI)

/**
 * uniform
 * gives a random real number within an interval
 * @param a lower bound
 * @param b upper bound
 * @return said random real number
 */
double uniform(double a, double b)
{
    return a + genrand_real1() * (b - a);
}

/**
 * initArr
 * initializes an array of doubles to 0.0 s
 * @param arr said array
 * @param n size of said array
 */
void initArr(double *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = 0.0;
    }
}

/**
 * mkArr
 * creates an array of doubles
 * @param n size of said array
 * @return said array
 */
double *mkArr(int n)
{
    double *res = NULL;

    res = malloc(sizeof(double) * n);
    if (res == NULL)
    {
        printf("MALLOC FAILURE");
        exit(EXIT_FAILURE);
    }
    return res;
}

/**
 * printArrMsg_f
 * prints to console a given string, followed by the contents of an array of doubles
 * @param msg said string
 * @param arr said array
 * @param n size of said array
 */
void printArrMsg_f(char *msg, double *arr, int n)
{
    printf("%s\n", msg);
    for (int i = 0; i < n; i++)
    {
        printf("%10.8f ", arr[i]);
        if ((i % 5 == 4) || (i == n - 1)) printf("\n");
    }
}

/**
 * printArrMsg
 * prints to console a given string, followed by the contents of an array of ints
 * @param msg said string
 * @param arr said array
 * @param n size of said array
 */
void printArrMsg(char *msg, int *arr, int n)
{
    printf("%s\n", msg);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
        if ((i % 5 == 4) || (i == n - 1)) printf("\n");
    }
}

/**
 * cdf
 * finds the cumulative distribution function from basic observation data
 * @param n size of observation data array, strictly positive
 * @param obs said array, of ints, non null
 * @return an array of doubles, CDF
 */
double *cdf(int n, int *obs)
{
    if ((n < 1) || (obs == NULL))
    {
        printf("cdf: please provide a non-null array, and a strictly positive size for it\n");
        exit(EXIT_FAILURE);
    }

    int    i, j;
    int    ttl  = 0;
    double *pdf = mkArr(n);

    // finding sample size
    for (i = 0; i < n; i++)
    {
        ttl += obs[i];
    }

    // 3.b.a producing probability distribution function
    for (i = 0; i < n; i++)
    {
        pdf[i] = (double) obs[i] / ttl;
    }
    printArrMsg_f("PDF:", pdf, 6);

    int    sum  = 0;
    double *cdf = mkArr(n);

    // 3.b.b producing cumulative distribution function
    for (i = 0; i < n; i++)
    {

        for (j = 0; j <= i; j++)
        {
            sum += obs[j];
        }
        cdf[i] = (double) sum / ttl;
        sum = 0;
    }

    return cdf;
}

/**
 * negExp
 * implements the negative exponential function: an exponential continuous distribution
 * @param m a desired mean
 * @return result of the formula: -m * ln(1 - rdm) (rdm being a double in [0, 1[ so as not to ask ln(0))
 */
double negExp(double m)
{
    return (-m * log(1 - genrand_real2()));
}

/**
 * d6
 * gives probability for x to be the outcome of a 6-sided-die throw
 * @param x outcome tested for
 * @return probability of said outcome, in [0, 1]
 */
double d6(double x)
{
    if ((x >= 1) && (x <= 6)) return (1.0 / 6);
    else return 0.0;
}

/**
 * rejection algorithm to generate a random number according to a PDF
 * @param xMax upper bound of said random number
 * @param xMin lower bound of said random number
 * @param size sample size
 * @param fun a PDF (probability distribution function)
 * @return a random number according to a PDF
 */
double rejection(int xMax, int xMin, int size, double (*pdf)(double))
{
    double yMax = xMax * size;
    while (1)
    {
        double xRdm = genrand_real1();
        double yRdm = genrand_real1();
        double x    = xMin + xRdm * (xMax - xMin);
        double y    = yMax * yRdm;
        if (y <= (*pdf)(x)) return x;
    }
}

/**
 * boxMuller
 * procedure to generate two random numbers uniformly distributed around a given mean according to a given sigma
 * @param x1 pointer to a double, will house first random number
 * @param x2 pointer to a double, will house second random number
 * @param mean said mean
 * @param sigma said sigma (standard deviation)
 */
void boxMuller(double *x1, double *x2, double mean, double sigma)
{
    //with some help from https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform

    double r1  = genrand_real3();
    double r2  = genrand_real3();
    double mag = sigma * sqrt((-2 * log(r1)));
    *x1 = mag * cos(TAU * r2) + mean;
    *x2 = mag * sin(TAU * r2) + mean;
}

int main(void)
{
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
    // make sure that it checks out compared to ../matsumoto/mt19937ar.out

    printf("########### 2 ###########\n");

    printf("1000 outputs of uniform(); expected mean = -16.25\n");
    double mean2 = 0, uniformRet;
    for (i = 0; i < 1000; i++)
    {
        uniformRet = uniform(-89.2, 56.7);
        mean2 += uniformRet;
        printf("%10.8f ", uniformRet);
        if (i % 5 == 4) printf("\n");
    }
    mean2 /= 1000;
    printf("approximate mean with uniform(): %10.8f\n", mean2);

    printf("########### 3 ###########\n");
    printf("##### 3.a #####\n");

    int    a, b, c;
    double rand3a;
    printf("expecting around: A = 50 pct; B = 10 pct; C = 40 pct\n");
    for (i = 1000; i <= 1000000; i *= 1000)
    {
        a = b = c = 0;
        printf("sample size = %d\n", i);
        for (j = 0; j < i; j++)
        {
            rand3a = genrand_real1();
            if (rand3a < 0.5) a++;
            else if (rand3a < 0.6) b++;
            else c++;
        }
        printf("individuals in class A = %10f pct\n", ((double) a / i) * 100.0);
        printf("individuals in class B = %10f pct\n", ((double) b / i) * 100.0);
        printf("individuals in class C = %10f pct\n", ((double) c / i) * 100.0);
    }

    printf("##### 3.b #####\n");
    int    obs3b[] = {100, 400, 600, 400, 100, 200};
    double *cdf3b  = cdf(6, obs3b);
    double rand3b;
    double *res3 = mkArr(6);
    initArr(res3, 6);
    printArrMsg_f("CDF:", cdf3b, 6);
    for (i = 1000; i <= 1000000; i *= 1000)
    {
        // simulating each item of sample, while resetting cumulative sum to 0
        for (j = 0; j < i; j++)
        {
            rand3b = genrand_real1();
            {
                // putting away each item of sample in our array
                for (k = 0; k < 6; k++)
                {
                    if (rand3b < cdf3b[k])
                    {
                        res3[k] += 1;
                        break;
                    }
                }
            }
        }
        // presenting the results as percentages
        for (j = 0; j < 6; j++) res3[j] = (double) res3[j] * 100.0 / i;
        printf("sample size = %d: ", i);
        printArrMsg_f("DED_%:", res3, 6);
    }
    printf("########### 4 ###########\n");

    double cuml4;
    printf("expecting around: average = 10.0\n");
    for (i = 1000; i <= 1000000; i *= 1000)
    {
        cuml4 = 0;

        for (j = 0; j < i; j++)
        {
            cuml4 += negExp(10.0);
        }
        printf("sample size = %d: average = %10f\n", i, cuml4 / i);
    }

    int testBins4[21] = {0};
    int negExpRet;
    for (i = 1000; i <= 1000000; i *= 1000)
    {
        printf("test bins, sample size = %d\n", i);
        for (j = 0; j < i; j++)
        {
            negExpRet = (int) negExp(10.0);
            if (negExpRet < 20) testBins4[negExpRet] += 1;
            else testBins4[20] += 1;
        }
        for (j = 0; j < 21; j++)
        {
            printf("in box %d: %d\n", j, testBins4[j]);
        }
    }
    printf("(see report for scatter plots of these tests)\n");

    printf("########### 5 & EXTRA ###########\n");

    // the 'many' variable below can be increased, but the relevant functions get really slow (10sec+) in the 10^6 range
    int    many  = 100000, throws = 20;
    double mean4 = 0, sigma4 = 0;
    printf
            (
                    "rejection function with 20d6\n"
                    "expecting around: mean = 3.5; sigma = %10f\n",
                    sqrt(throws * (17.5 / 6))
            );
    printf("'many' = %d\n", many);

    for (i = 0; i < many; i++) mean4 += rejection(6, 1, throws, &d6);

    mean4 /= many;
    printf("using our rejection function\napproximate mean for d6 throws: %10f\n", mean4);

    //sum of squares of distances
    for (i = 0; i < many; i++) sigma4 += pow((rejection(6, 1, throws, &d6) - mean4), 2);

    sigma4 = sqrt((throws * sigma4) / many); //population standard deviation
    printf("approximate population standard deviation for 20 d6 throws: %10f\n", sigma4);

    printf("Box-Muller function:\n");
    int    testBins5[20] = {0};
    double x[2];
    double mean5, sigma5;
    for (i = 1000; i <= 1000000; i *= 1000)
    {
        mean5 = sigma5 = 0;
        printf("expecting around: mean = 0; sigma = 1 (sample size = %d)\n", i);
        for (j = 0; j < i; j++)
        {
            boxMuller(&x[0], &x[1], 0, 1);
            for (k = 0; k < 2; k++) // checking both numbers from box&muller
            {
                mean5 += x[k];
                if ((x[k] >= -1) && (x[k] <= 1))
                {
                    testBins5[(int) (x[k] * 10 + 10)] += 1; //[-1, 1] --> [0, 20]
                }
            }
        }
        mean5 /= 2 * i;
        printf("approximate mean: %10f\n", mean5);
        for (j = 0; j < i; j++)
        {
            boxMuller(&x[0], &x[1], 0, 1);
            for (k = 0; k < 2; k++) // still checking both numbers from box&muller
            {
                sigma5 += pow((x[k] - mean5), 2);
            }
        }
        sigma5 = sqrt(sigma5 / (2 * i));
        printf("approximate standard deviation: %10f\n", sigma5);
        for (j = 0; j < 20; j++)
        {
            printf("in [%f, %f[: %d\n", (j - 10.0) / 10, (j - 10.0) / 10 + 0.1, testBins5[j]);
        }
    }

    printf("And now for mean = 10, sigma = 3 (sample size = 2000000)\n");
    mean5  = sigma5 = 0;
    for (i = 0; i < 1000000; i++)
    {
        boxMuller(&x[0], &x[1], 10, 3);
        for (j = 0; j < 2; j++)
        {
            mean5 += x[j];
            if ((x[j] >= 0) && (x[j] <= 20))
            {
                testBins5[(int) x[j]] += 1;
            }
        }
    }
    mean5 /= 2000000;
    printf("approximate mean: %10f\n", mean5);
    for (i = 0; i < 1000000; i++)
    {
        boxMuller(&x[0], &x[1], 10, 3);
        for (j = 0; j < 2; j++)
        {
            sigma5 += pow((x[j] - mean5), 2);
        }
    }
    sigma5 = sqrt(sigma5 / 2000000);
    printf("approximate standard deviation: %10f\n", sigma5);
    for (j = 0; j < 20; j++)
    {
        printf("in [%d, %d[: %d\n", j, j + 1, testBins5[j]);
    }
    printf("(see report for scatter plots of these tests)\n");

    printf("########### 6 ###########\n");
    printf("(see comments or report)\n");
    /*
     * C/C++ libs
     * uniform:             https://people.sc.fsu.edu/~jburkardt/c_src/uniform/uniform.html
     * discrete empirical:  https://www.nsnam.org/doxygen/classns3_1_1_empirical_random_variable.html
     * continuous:          https://people.sc.fsu.edu/~jburkardt/c_src/ranlib/ranlib.html
     * normal:              https://people.sc.fsu.edu/~jburkardt/c_src/normal/normal.html ;
     *                      https://people.sc.fsu.edu/~jburkardt/c_src/ziggurat/ziggurat.html
     * Java libs
     * uniform:             https://commons.apache.org/proper/commons-math/javadocs/api-3.5/org/apache/commons/math3/distribution/UniformRealDistribution.html
     * discrete empirical:  https://commons.apache.org/proper/commons-math/javadocs/api-3.5/org/apache/commons/math3/random/class-use/RandomGenerator.html
     * continuous:          https://commons.apache.org/proper/commons-math/javadocs/api-3.4/org/apache/commons/math3/distribution/ExponentialDistribution.html ;
     *                      etc.
     * normal:              https://commons.apache.org/proper/commons-math/javadocs/api-3.6/org/apache/commons/math3/distribution/NormalDistribution.html
     *
     */

    return 0;
}
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

/* These real versions are due to Isaku Wada, 2002/01/09 added */
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Created by draia on 27/01/2022.
//

#include <math.h>

// let me have this!
#define TAU (2*M_PI)
#define DEBUG printf("file %s; line %d\n", __FILE__, __LINE__);
#define FAIL_OUT DEBUG exit(EXIT_FAILURE);
#define MALLOC_FAIL printf("!_malloc failed_!\n"); FAIL_OUT

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   ANNEXES   ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

/**
 * mkArr
 * creates an array of doubles, initialized at 0.0 s
 * @param n size of said array
 * @return said array
 */
double *mkArr(int n)
{
    double *res = NULL;

    res = calloc(n, sizeof(double));
    if (res == NULL)
    {
        MALLOC_FAIL
    }
    return res;
}

/**
 * mkArr_int
 * creates an array of ints, initialized at 0 s
 * @param n size of said array
 * @return said array
 */
int *mkArr_int(int n)
{
    int *res = NULL;
    res = calloc(n, sizeof(int));
    if (res == NULL)
    {
        MALLOC_FAIL
    }
    return res;
}

/**
 * free2d
 * frees a 2d array of doubles
 * @param arr said array
 * @param n outer size of said array
 */
void free2d(double **arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] != NULL)
        {
            free(arr[i]);
        }
    }
    free(arr);
}

/**
 * mk2tuplesArr
 * makes an array of 2-tuples of real numbers
 * @param n outer size of desired array
 * @return said array
 */
double **mk2tuplesArr(int n)
{
    int    hasFailed = 0;
    double **res     = NULL;
    res = (double **) malloc(sizeof(double *) * n);
    if (res == NULL)
    {
        MALLOC_FAIL
    }
    for (int i = 0; i < n; i++)
    {
        res[i] = NULL;
        res[i] = (double *) malloc(sizeof(double) * 2);
        if (res[i] == NULL)
        {
            hasFailed = 1;
        }
    }
    if (hasFailed)
    {
        free2d(res, n);
        MALLOC_FAIL
    } else
    {
        return res;
    }
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
        if ((i % 5 == 4) || (i == n - 1))
        {
            printf("\n");
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   BUSINESS   //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

/**
 * uniform
 * gives a random real number within an interval
 * @param inLo lower bound
 * @param inHi upper bound
 * @return said random real number
 */
double uniform(double inLo, double inHi)
{
    return inLo + genrand_real1() * (inHi - inLo);
}

/**
 * cdf
 * finds the cumulative distribution function from basic observation data
 * @param inSize size of observation data array, strictly positive
 * @param inObs said array, of ints, non null
 * @return an array of doubles, CDF
 */
double *cdf(int inSize, const int *inObs)
{
    if ((inSize < 1) || (inObs == NULL))
    {
        printf("cdf: please provide a non-null array, and a strictly positive size for it\n");
        FAIL_OUT
    }

    int    i, j;
    int    ttl  = 0;
    double *pdf = mkArr(inSize);

    // finding sample size
    for (i = 0; i < inSize; i++)
    {
        ttl += inObs[i];
    }

    // 3.b.a producing probability distribution function
    for (i = 0; i < inSize; i++)
    {
        pdf[i] = (double) inObs[i] / ttl;
    }
    printArrMsg_f("PDF:", pdf, 6);

    double *cdf = mkArr(inSize);

    // 3.b.b producing cumulative distribution function
    for (i = 0; i < inSize; i++)
    {
        for (j = 0; j <= i; j++)
        {
            cdf[i] += pdf[j];
        }
    }
    free(pdf);
    return cdf;
}

/**
 * negExp
 * implements the negative exponential function: an exponential continuous distribution
 * @param inMean a desired mean
 * @return result of the formula: -m * ln(1 - rdm) (rdm being a double in [0, 1[ so as not to ask ln(0))
 */
double negExp(double inMean)
{
    return (-inMean * log(1 - genrand_real2()));
}

/**
 * ndn
 * throws a given number of dice with a given number of sides
 * @param inThrows number of throws
 * @param inSides number of sides
 * @return total result
 */
int ndn(int inThrows, int inSides)
{
    int res = 0;

    for (int i = 0; i < inThrows; i++)
    {
        res += (int) (1 + genrand_real1() * inSides);
    }
    return res;
}

/**
 * boxMuller
 * procedure to generate two random numbers uniformly normally distributed around a given mean according to a given sigma
 * @param inX1 pointer to a double, will house first random number
 * @param inX2 pointer to a double, will house second random number
 * @param inMean said mean
 * @param inSigma said sigma (standard deviation)
 */
void boxMuller(double *inX1, double *inX2, double inMean, double inSigma)
{
    //with some help from https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform

    double r1  = genrand_real2();
    double r2  = genrand_real2();
    double mag = inSigma * sqrt((-2 * log(r1)));
    *inX1 = mag * cos(TAU * r2) + inMean;
    *inX2 = mag * sin(TAU * r2) + inMean;
}

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////   TESTS   //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    // init and ### 1 ### by M.M.
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
    for (i                   = 0; i < 1000; i++)
    {
        printf("%10.8f ", genrand_real2());
        if (i % 5 == 4) printf("\n");
    }
    // making sure that it checks out compared to ../matsumoto/mt19937ar.out

    printf("########### 2 ###########\n");

    printf("after 1000000 outputs of uniform(); expected mean = -16.25\n");
    double uniformRet, mean2 = 0;
    int    idx, size2        = 1000000;
    double testBins2[145]    = {0.0};
    for (i = 0; i < size2; i++)
    {
        uniformRet = uniform(-89.2, 56.7);
        idx        = (int) uniformRet + 88;

        mean2 += uniformRet;
        // making sure not to conflate -0.boxMulRet and +0.boxMulRet
        if (uniformRet < 0)
        {
            testBins2[idx] += 1;
        } else
        {
            testBins2[idx + 1] += 1;
        }
    }
    mean2 /= size2;
    printf("approximate mean with uniform(): %10.8f\n", mean2);
    // presenting the results as percentages
    for (i = 0; i < 145; i++)
    {
        testBins2[i] = (double) testBins2[i] * 100.0 / size2;
    }
    printf("sample size = %d: ", i);
    printArrMsg_f("uniform_%:", testBins2, 145);
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
            else if (rand3a < 0.65) b++;
            else c++;
        }
        printf("individuals in class A = %10f pct\n"
               "individuals in class B = %10f pct\n"
               "individuals in class C = %10f pct\n",
               ((double) a / i) * 100.0, ((double) b / i) * 100.0, ((double) c / i) * 100.0);
    }

    printf("##### 3.b #####\n");
    int    obs3b[]      = {100, 400, 600, 400, 100, 200};
    double *cdf3b       = cdf(6, obs3b);
    double rand3b;
    double testBins3[6] = {0.0};
    printArrMsg_f("CDF:", cdf3b, 6);

    for (i = 1000; i <= 1000000; i *= 1000)
    {
        for (j = 0; j < i; j++)
        {
            rand3b = genrand_real1();
            {
                for (k = 0; k < 6; k++)
                {
                    if (rand3b < cdf3b[k])
                    {
                        testBins3[k] += 1;
                        break;
                    }
                }
            }
        }
        for (j = 0; j < 6; j++)
        {
            testBins3[j] = (double) testBins3[j] * 100.0 / i;
        }
        printf("sample size = %d: ", i);
        printArrMsg_f("DED_%:", testBins3, 6);
    }
    free(cdf3b);
    printf("########### 4 ###########\n");

    double cuml4;
    double mean4 = 11.0;
    printf("expecting around: average = %10.8f\n", mean4);
    for (i = 1000; i <= 1000000; i *= 1000)
    {
        cuml4 = 0;

        for (j = 0; j < i; j++)
        {
            cuml4 += negExp(mean4);
        }
        printf("sample size = %d: average = %10f\n", i, cuml4 / i);
    }

    int testBins4[22] = {0};
    int negExpRet;
    for (i = 1000; i <= 1000000; i *= 1000)
    {
        printf("test bins, sample size = %d\n", i);
        for (j = 0; j < i; j++)
        {
            negExpRet = (int) negExp(11.0);
            if (negExpRet < 21)
            {
                testBins4[negExpRet] += 1;
            } else
            {
                testBins4[21] += 1;
            }
        }
        for (j = 0; j < 22; j++)
        {
            printf("in box %d: %d\n", j, testBins4[j]);
        }
    }
    printf("(see report for scatter plots of these tests)\n");

    printf("########### 5 ###########\n");

    int    testBins5a[150] = {0};
    int    many            = 1000000, throws = 30, tmp;
    double mean5a          = 0, sigma5a = 0;
    int    *ndnRet         = mkArr_int(many);
    printf
            (
                    "sample size = %d throws of 20 d6s\n"
                    "expecting around: mean = %10.8f ; sigma = %10.8f\n",
                    many, (throws * 21.0) / 6, sqrt(throws * (35.0 / 12))
            );

    for (i = 0; i < many; i++)
    {
        tmp       = ndn(throws, 6);
        testBins5a[tmp - 30] += 1; //[30, 180] --> [0, 150]
        ndnRet[i] = tmp;
        mean5a += tmp;
    }

    mean5a /= many;
    printf("approximate mean for 20 d6 throws: %10f\n", mean5a);

    //sum of squares of distances
    for (i  = 0; i < many; i++)
    {
        sigma5a += (pow((ndnRet[i] - mean5a), 2));
    }
    sigma5a = sqrt(sigma5a / many); // standard deviation
    printf("approximate population standard deviation for 20 d6 throws: %10.8f\n", sigma5a);

    for (j = 0; j < 150; j++)
    {
        printf("in [%d, %d[: %d\n", j + 30, j + 31, testBins5a[j]);
    }

    printf("Box-Muller function:\n");
    int    testBins5b[20] = {0};
    double mean5b, sigma5b, tmp_f, boundLow, boundHigh;
    double **boxMulRet;
    for (i = 1000; i <= 1000000; i *= 1000)
    {
        boxMulRet = mk2tuplesArr(i);
        mean5b    = sigma5b = 0;
        printf("expecting around: mean = 0; sigma = 1 (sample size = %d)\n", i * 2);
        for (j = 0; j < i; j++)
        {
            boxMuller(&boxMulRet[j][0], &boxMulRet[j][1], 0, 1);
            for (k = 0; k < 2; k++) // checking both numbers from box&muller
            {
                tmp_f = boxMulRet[j][k];
                mean5b += tmp_f;
                if ((tmp_f >= -1) && (tmp_f <= 1))
                {
                    testBins5b[(int) (tmp_f * 10 + 10)] += 1; //[-1, 1] --> [0, 20]
                }
            }
        }
        mean5b /= 2 * i;
        printf("approximate mean: %10f\n", mean5b);
        for (j  = 0; j < i; j++)
        {
            for (k = 0; k < 2; k++)
            {
                sigma5b += pow((boxMulRet[j][k] - mean5b), 2);
            }
        }
        sigma5b = sqrt(sigma5b / (2 * i));
        printf("approximate standard deviation: %10f\n", sigma5b);
        for (j = 0; j < 20; j++)
        {
            boundLow  = (j - 10.0) / 10;
            boundHigh = boundLow + 0.1;
            printf("in [%f, %f[: %d\n", boundLow, boundHigh, testBins5b[j]);
        }
        free2d(boxMulRet, i);
    }

    printf("And now for mean = 12.0, sigma = 3.0 (sample size = 2000000)\n");
    int testBins5b_12[24] = {0};
    mean5b = sigma5b = 0;
    int    sample5b       = 1000000;
    double **boxMulRet_12 = mk2tuplesArr(sample5b);
    for (i = 0; i < sample5b; i++)
    {
        boxMuller(&boxMulRet_12[i][0], &boxMulRet_12[i][1], 12, 3);
        for (j = 0; j < 2; j++)
        {
            tmp_f = boxMulRet_12[i][j];
            mean5b += tmp_f;
            if ((tmp_f >= 0) && (tmp_f <= 24))
            {
                testBins5b_12[(int) tmp_f] += 1;
            }
        }
    }
    mean5b /= sample5b * 2;
    printf("approximate mean: %10f\n", mean5b);
    for (i  = 0; i < sample5b; i++)
    {
        for (j = 0; j < 2; j++)
        {
            sigma5b += pow((boxMulRet_12[i][j] - mean5b), 2);
        }
    }
    sigma5b = sqrt(sigma5b / (sample5b * 2));
    printf("approximate standard deviation: %10f\n", sigma5b);
    for (j = 0; j < 24; j++)
    {
        printf("in [%d, %d[: %d\n", j, j + 1, testBins5b_12[j]);
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
     */
    return 0;
}
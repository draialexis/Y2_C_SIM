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


#include <stdlib.h>
#include <math.h>

#define EPSILON 0.00000000000000022204460492503131

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

double *cpArr(double *orig, int n)
{
    double *copy = mkArr(n);

    for (int i = 0; i < n; i++)
    {
        copy[i] = orig[i];
    }
    return copy;
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
        printf("%10f ", arr[i]);
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
 * ded_old (discrete empirical distribution)
 * populates a given array with a probability distribution
 * @param probs an array of distinct probabilities represented as reals in [0...1], whose cumulative sum is 1.0
 * @param arr the array to be populated, after having been initialized to 0.0 s
 * @param n size of said arrays, strictly positive
 * @param pop sample size, strictly positive
 */
void ded_old(const double *probs, double *arr, int n, int pop)
{
    int    i, j;
    double cuml = 0.0;
    for (i = 0; i < n; i++) cuml += probs[i];

    if ((fabs(fabs(cuml) - 1) > EPSILON) || (n <= 0) || (pop <= 0) || (arr == NULL))
    {
        printf("cdf: cumulated probabilities must amount to 1.0 (found %f)\n"
               "n and pop must be strictly positive (n=%d, pop=%d)\n"
               "arr must not be NULL\n", cuml, n, pop);
        if (n > 0) printArrMsg_f("cdf: arr:", arr, n);
        exit(EXIT_FAILURE);
    }

    initArr(arr, n);
    double rdm;

    // simulating each item of sample, while resetting cumulative sum to 0
    for (i = 0; i < pop; i++)
    {
        cuml = 0;
        rdm  = genrand_real1();
        {
            // putting away each item of sample in our array
            for (j = 0; j < n; j++)
            {
                cuml += probs[j];
                if (rdm < cuml)
                {
                    arr[j] += 1;
                    break;
                }
            }
        }
    }
    // presenting the results as percentages
    for (i = 0; i < n; i++) arr[i] *= 100.0 / pop;
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
 * @return result of the formula: -m * ln(1 - rdm)
 */
double negExp(double m)
{
    return (-m * log(1 - genrand_real1()));
}

int main(void)
{
    int           i, j, k;
    unsigned long init[4] = {0x123, 0x234, 0x345, 0x456};
    int           length  = 4;

    init_by_array(init, length);

    printf("########### 2 ###########\n");

    printf("1000 outputs of uniform()\n");
    for (i = 0; i < 1000; i++)
    {
        printf("%10f ", uniform(-89.2, 56.7));
        if (i % 5 == 4) printf("\n");
    }

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
    double *res    = mkArr(6);
    initArr(res, 6);
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
                        res[k] += 1;
                        break;
                    }
                }
            }
        }
        // presenting the results as percentages
        for (j = 0; j < 6; j++) res[j] = (double) res[j] * 100.0 / i;
        printf("sample size = %d: ", i);
        printArrMsg_f("DED_%:", res, 6);
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

    int testBins[21] = {0};
    int negExpRet;
    for (i = 1000; i <= 1000000; i *= 1000)
    {
        printf("test bins, sample size = %d\n", i);
        for (j = 0; j < i; j++)
        {
            negExpRet = (int) negExp(10.0);
            if (negExpRet < 20) testBins[negExpRet] += 1;
            else testBins[20] += 1;
        }
        for (j = 0; j < 21; j++)
        {
            printf("in box %d: %d\n", j, testBins[j]);
        }
    }
    printf("see report for scatter plots of these tests\n");

    printf("########### 5 ###########\n");


    return 0;
}
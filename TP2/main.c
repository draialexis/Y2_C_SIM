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
 * abc_rep
 * populates a given array with a probability distribution
 * @param probs an array of probabilities represented as reals in [0...1]
 * @param arr the array to be populated, after having been initialized to 0.0 s
 * @param size size of said arrays
 * @param reps number of repetitions (i.e. sample size)
 */
void abc_rep(const double *probs, double *arr, int size, int reps)
{
    int    i, j;
    double rdm, cuml = 0.0;

    for (i = 0; i < size; i++) cuml += probs[i];

    if (fabs(cuml) - 1 > EPSILON)
    {
        printf("abc_rep: total of probabilities only amount to %f, expected 1.0\n", cuml);
        exit(EXIT_FAILURE);
    }

    initArr(arr, size);

    for (i = 0; i < reps; i++)
    {
        cuml = 0;
        rdm  = genrand_real1();
        if (rdm < 0.0 || rdm > 1.0)
        {
            printf("abc_rep: genrand_real1() < 0 || > 1 ?..\n");
            exit(EXIT_FAILURE);
        } else
        {
            for (j = 0; j < size; j++)
            {
                cuml += probs[j];
                if (rdm <= cuml)
                {
                    arr[j] += 1;
                    break;
                }
            }
        }
    }
    for (i = 0; i < size; i++) arr[i] /= reps;
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
 * printArrMsg
 * prints to console a given string, followed by the contents of an array of doubles
 * @param msg said string
 * @param arr said array
 * @param n size of said array
 */
void printArrMsg(char *msg, double *arr, int n)
{
    printf("%s\n", msg);
    for (int i = 0; i < n; i++)
    {
        printf("%10f ", arr[i]);
        if ((i % 5 == 4) || (i == n - 1)) printf("\n");
    }
}

int main(void)
{
    int           i;
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
    int    size3     = 3;
    double probs3[3] = {0.5, 0.1, 0.4};
    double *arr3_1k  = mkArr(size3);

    abc_rep(probs3, arr3_1k, size3, 1000);
    printArrMsg("arr3_1k", arr3_1k, size3);

    double *arr3_1M = mkArr(size3);

    abc_rep(probs3, arr3_1M, size3, 1000000);
    printArrMsg("arr3_1M", arr3_1M, size3);

    return 0;
}
//
// Created by draia on 20/01/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

static int seed = 5;

void initArr(int *res, int n)
{
    for (int i = 0; i < n; i++) res[i] = 0;
}

int *mkArr(int n)
{
    int *res = NULL;
    res = malloc(sizeof(int) * n);
    if (res == NULL)
    {
        printf("MALLOC FAILURE");
        exit(EXIT_FAILURE);
    }
    initArr(res, n);
    return res;
}

float *mkArr_f(int n)
{
    float *res = NULL;
    res = malloc(sizeof(float) * n);
    if (res == NULL)
    {
        printf("MALLOC FAILURE");
        exit(EXIT_FAILURE);
    }
    return res;
}

void printArrMsg(char *msg, int *arr, int n)
{
    printf("%s\n", msg);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
        if ((i % 5 == 4) || (i == n - 1)) printf("\n");
    }
}

void printArrMsg_f(char *msg, double *arr, int n)
{
    printf("%s\n", msg);
    for (int i = 0; i < n; i++)
    {
        printf("%10f ", arr[i]);
        if ((i % 5 == 4) || (i == n - 1)) printf("\n");
    }
}

void neumann(int n, int laps, int *res)
{
    for (int i = 0; i < laps; i++)
    {
        res[i] = n;
        n = ((n * n) / 100) % 10000;
    }
}

// "sides" is length of res... let's consider that a coin is kinda like a 2-sided die
void diceThrow(int sides, int throws, int *res)
{
    for (int i = 0; i < throws; i++)
    {
        res[rand() % sides]++;
    }
}

int intRand()
{
    seed = (seed * 5 + 1) % 16;
    return seed;
}

float floatRand()
{
    seed = (seed * 5 + 1) % 16;
    return (float) seed / 16;
}

void LCG(int a, int c, int m, int n, int *res)
{
    for (int i = 0; i < n; i++)
    {
        seed = (seed * a + c) % m;
        res[i] = seed;
    }
}

int main()
{
    srand(time(NULL));
    int laps = 100;
    int *arr = mkArr(laps);
    int i;

    printf("########### 1 ###########\n");
    neumann(1234, laps, arr); // converges towards 0, reaches it at i=56
    printArrMsg("seed 1234", arr, laps);

    printf("########### 2 ###########\n");
    neumann(4100, laps, arr); // is stuck in a short period at i%4==0 (4100, 8100, 6100, 2100, ...)
    printArrMsg("seed 4100", arr, laps);
    neumann(1324, laps, arr); // reaches 4100 at i=92, and then gets stuck in the above-mentioned period
    printArrMsg("seed 1324", arr, laps);
    neumann(1301, laps, arr); // reaches 6100 at i=10, and then gets stuck in the above-mentioned period
    printArrMsg("seed 1301", arr, laps);
    neumann(3141, laps, arr); // reaches 100 at i=7 and just stays there (_100_ * 100 / 100 % 10000 = _100_)
    printArrMsg("seed 3141", arr, laps);

    printf("########### 3 ###########\n");
    /* 
     * as a "pseudo-random number generator", rand() doesn't seem appropriate for scientific uses.
     * regarding srand(): basically, srand() will always give the same number -- given the same seed.
     * good for reproducibility if we keep the seeds => good for testing -- but not good if we need true randomness
     * also, some OSs and some C implementations have a problem with lower-order bits being not "as random"
     */

    printf("########### 4 ###########\n");
    int sides    = 2;
    int *coinArr = mkArr(sides);
    for (i = 10; i <= 1000; i *= 10)
    {
        diceThrow(sides, i, coinArr);
        printArrMsg("coin flips", coinArr, sides);
        initArr(coinArr, sides);
    }

    // the results seem equidistributed -- no clear bias one way or another

    printf("########### 5 ###########\n");
    sides = 6;
    int *d6Arr = mkArr(sides);
    for (i = 10; i <= 1000; i *= 10)
    {
        diceThrow(sides, i, d6Arr);
        printArrMsg("d6 throws", d6Arr, sides);
        initArr(d6Arr, sides);
    }

    sides = 10;
    int *d10Arr = mkArr(sides);
    for (i = 10; i <= 1000; i *= 10)
    {
        diceThrow(sides, i, d10Arr);
        printArrMsg("d10 throws", d10Arr, sides);
        initArr(d10Arr, sides);
    }
    diceThrow(sides, 1000000, d10Arr);
    printArrMsg("d10 throws", d10Arr, sides);

    printf("########### 6 ###########\n");
    int n = 32;

    arr    = mkArr(n);
    for (i = 0; i < n; i++)
    {
        arr[i] = intRand();
    }
    printArrMsg("'x_i + 1 = (5 * x_i + 1) mod 16'", arr, n);

    printf("########### 7 ###########\n");
    float *arr_f = mkArr_f(n);
    for (i = 0; i < n; i++)
    {
        arr_f[i] = floatRand();
    }
    printArrMsg_f("'x_i + 1 = ((5 * x_i + 1) mod 16) / 16'", arr_f, n);

    printf("########### 8 ###########\n");
    initArr(arr, n);
    LCG(3, 6, 81, n, arr);
    printArrMsg("'x_i + 1 = (3 * x_i + 6) mod 81'", arr, n);
    // a=3, c=6, m=81 converges to 78 at i=4
    initArr(arr, n);
    LCG(2, 4, 14, n, arr);
    printArr("'x_i + 1 = (2 * x_i + 4) mod 14'", arr, n);
    // a=2, c=4, m=14 creates a very short period: 6, 2, 8, ...

    printf("########### 9 ###########\n");
    /* L'Ecuyer, 1999
     * https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwi6-_HcusD1AhVChRoKHRulC6YQFnoECAcQAQ&url=https%3A%2F%2Fwww.ams.org%2Fjournals%2Fmcom%2F1999-68-225%2FS0025-5718-99-00996-5%2FS0025-5718-99-00996-5.pdf&usg=AOvVaw0kDbsjp2F_xPvvY4rPEbcH
     *
     * choosing a=0 gives us a maximal period (m-1), but the generated sequences are obviously not even random-**looking**
     *
     * L'Ecuyer points out that choosing m=2^x and c=/=0, with c odd and a%8=5, gives us sequences with major limitations:
     * mostly, lower order bits then have a short period. With a large x and only using higher order bits of our results,
     * we can make it work, but we're only working with 32 bits here.
     *
     * We can now look at c=0, m=largest prime < 2³¹, and a such as there exists a y such as (a*y)%m=1
     * we can then use a or y, and get the same results
     * for example:
     * m=2039, a=393 or 799
     * m=32749, a=22661 or 23234
     * ...
     */

    printf("########### 10 ###########\n");
    /*
     * gsl_rng.h: https://www.gnu.org/software/gsl/doc/html/rng.html
     * sodium.h: https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c/39475626#39475626
     */

    printf("########### 11 ###########\n");
    /*
     * TestU01.h: http://simul.iro.umontreal.ca/testu01/tu01.html
     */

    printf("########### EXTRA ###########\n");

    // inspired by https://en.wikipedia.org/wiki/Linear-feedback_shift_register
    uint8_t first   = 6;
    uint8_t current = first;
    uint8_t bit;

    do
    {
        printf(BYTE_TO_BINARY_PATTERN"-->", BYTE_TO_BINARY(current));
        bit     = (current ^ (current >> 1)) & 1u;
        /*
         *               0 1 1 0
         *                      -xor-> 0 1 0 1
         * 0 1 1 0 -r1-> 0 0 1 1
         */
        current = (current >> 1) | (bit << 3); //(bit << (n-1)) with n number of bits considered
        /*
         * 0 1 1 0 -r1-> 0 0 1 1
         *                       -or-> 1 0 1 1
         * 0 1 0 1 -l3-> 1 0 0 0
         */
    } while (current != first);
    printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(current));


    return 0;
}

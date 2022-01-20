#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initArr(int *res, int n)
{
    for (int i = 0; i < n; i++)
    {
        res[i] = 0;
    }
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

void printArr(char *msg, int *arr, int n)
{
    printf("%s: ", msg);
    for (int i = 0; i < n; i++)
    {
        printf("%d", arr[i]);
        if (i == n - 1)
        {
            printf("\n");
        }
        else
        {
            printf(", ");
        }
    }
    printf("\n");
}

void neumann(int n, int laps, int *res)
{
    for (int i = 0; i < laps; i++)
    {
        res[i] = n;
        n = ((n * n) / 100) % 10000;
    }
}

void diceThrow(int sides, int throws, int *res)
{ // "sides" is length of res... let's consider that a coin is kinda like a 2-sided die
    for (int i = 0; i < throws; i++)
    {
        res[rand() % sides]++;
    }
}

void myLCG(int a, int c, int m, int *res, int n, int x_0)
{
    int x_prev;
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            x_prev = x_0;
        }
        else
        {
            x_prev = res[i - 1];
        }
        res[i] = (x_prev * a + c) % m;
    }
}

/*
int intRand() {
    return (_ * 5 + 1) % 16;
}
*/

void myLCG_float(int a, int c, int m, float *res, int n, float x_0)
{
    float x_prev;
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            x_prev = x_0;
        }
        else
        {
            x_prev = res[i - 1];
        }
        res[i] = (x_prev * a + c) % m;
    }
}

int main()
{
    srand(time(NULL));
    int laps = 100;
    int *arr = mkArr(laps);

    printf("########### 1 ###########\n");
    neumann(1234, laps, arr); // converges towards 0, reaches it at i=56
    printArr("seed 1234", arr, laps);

    printf("########### 2 ###########\n");
    neumann(4100, laps, arr); // is stuck in a short period at i%4==0 (4100, 8100, 6100, 2100, ...)
    printArr("seed 4100", arr, laps);
    neumann(1324, laps, arr); // reaches 4100 at i=92, and then gets stuck in the above-mentioned period
    printArr("seed 1324", arr, laps);
    neumann(1301, laps, arr); // reaches 6100 at i=10, and then gets stuck in the above-mentioned period
    printArr("seed 1301", arr, laps);
    neumann(3141, laps, arr); // reaches 100 at i=7 and just stays there (_100_ * 100 / 100 % 10000 = _100_)
    printArr("seed 3141", arr, laps);

    printf("########### 3 ###########\n");
    /* 
     * as a "pseudo-random number generator", rand() doesn't seem appropriate for scientific uses.
     * regarding srand(): basically, srand() will always give the same number -- given the same seed.
     * good for reproducibility if we keep the seeds => good for testing -- but not good if we need true randomness
     * also, some OSs and some C implementations have a problem with lower-order bits being not "as random"
     */

    printf("########### 4 ###########\n");
    int sides = 2;
    int *coinArr = mkArr(sides);
    for (int i = 10; i <= 1000; i *= 10)
    {
        diceThrow(sides, i, coinArr);
        printArr("coin flips", coinArr, sides);
        initArr(coinArr, sides);
    }

    // the results seem equidistributed -- no clear bias one way or another

    printf("########### 5 ###########\n");
    sides = 6;
    int *d6Arr = mkArr(sides);
    for (int i = 10; i <= 1000; i *= 10)
    {
        diceThrow(sides, i, d6Arr);
        printArr("d6 throws", d6Arr, sides);
        initArr(d6Arr, sides);
    }

    sides = 10;
    int *d10Arr = mkArr(sides);
    for (int i = 10; i <= 1000; i *= 10)
    {
        diceThrow(sides, i, d10Arr);
        printArr("d10 throws", d10Arr, sides);
        initArr(d10Arr, sides);
    }
    diceThrow(sides, 1000000, d10Arr);
    printArr("d10 throws", d10Arr, sides);

    printf("########### 6 ###########\n");
    int n = 32;
    arr = mkArr(n);
    myLCG(5, 1, 16, arr, n, 5);
    printArr("x_i+1 = (5 * x_i + 1) mod 16", arr, n);

    return 0;
}

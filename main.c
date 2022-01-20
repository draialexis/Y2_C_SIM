#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LAPS 100

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

void printArr(int *arr, int n)
{
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

int main()
{
    srand(time(NULL));

    int *arr = mkArr(LAPS);

    printf("########### 1 ###########\n");
    neumann(1234, LAPS, arr); // converges towards 0, reaches it at i=56
    printArr(arr, LAPS);

    printf("########### 2 ###########\n");
    neumann(4100, LAPS, arr); // is stuck in a short period at i%4==0 (4100, 8100, 6100, 2100, ...)
    printArr(arr, LAPS);
    neumann(1324, LAPS, arr); // reaches 4100 at i=92, and then gets stuck in the above-mentioned period
    printArr(arr, LAPS);
    neumann(1301, LAPS, arr); // reaches 6100 at i=10, and then gets stuck in the above-mentioned period
    printArr(arr, LAPS);
    neumann(3141, LAPS, arr); // reaches 100 at i=7 and just stays there (_100_ * 100 / 100 % 10000 = _100_)
    printArr(arr, LAPS);

    printf("########### 3 ###########\n");
    /* 
     * as a "pseudo-random number generator", rand() doesn't seem appropriate for scientific uses.
     * regarding srand(): basically, srand() will always give the same number -- given the same seed.
     * good for reproducibility if we keep the seeds => good for testing -- but not good if we need true randomness
     * also, some OSs and some C implementations have a problem with lower-order bits being not "as random"
     */

    printf("########### 4 ###########\n");
    int *coinArr = mkArr(2);
    for (int i = 10; i <= 1000; i *= 10)
    {
        diceThrow(2, i, coinArr);
        printArr(coinArr, 2);
        initArr(coinArr, 2);
    }

    // the results seem equidistributed -- no clear bias one way or another

    return 0;
}

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "utils.c"

#define TASK_SIZE 100

void partition(int *X, int n, int *tmp)
{
    int i = 0;
    int j = n / 2;
    int ti = 0;

    while (i < n / 2 && j < n)
    {
        if (X[i] < X[j])
        {
            tmp[ti] = X[i];
            ti++;
            i++;
        }
        else
        {
            tmp[ti] = X[j];
            ti++;
            j++;
        }
    }
    while (i < n / 2)
    { // lower half
        tmp[ti] = X[i];
        ti++;
        i++;
    }
    while (j < n)
    { // upper half
        tmp[ti] = X[j];
        ti++;
        j++;
    }
    memcpy(X, tmp, n * sizeof(int));
}

void mergesort(int *X, int n, int *tmp)
{
    if (n < 2)
        return;

#pragma omp task shared(X) if (n > TASK_SIZE)
    mergesort(X, n / 2, tmp);

#pragma omp task shared(X) if (n > TASK_SIZE)
    mergesort(X + (n / 2), n - (n / 2), tmp + n / 2);

#pragma omp taskwait
    partition(X, n, tmp);
}

void init(int *a, int size)
{
    for (int i = 0; i < size; i++)
        a[i] = 0;
}

int main(int argc, char *argv[])
{
    srand(123456);
    int N = (argc > 1) ? atoi(argv[1]) : 10;
    int print = (argc > 2) ? atoi(argv[2]) : 0;
    int numThreads = (argc > 3) ? atoi(argv[3]) : 2;
    int *X = malloc(N * sizeof(int));
    int *tmp = malloc(N * sizeof(int));

    omp_set_dynamic(0);              // Explicitly disable dynamic teams
    omp_set_num_threads(numThreads); // Use N threads for all parallel regions

    // Fail memory allocation
    if (!X || !tmp)
    {
        if (X)
            free(X);
        if (tmp)
            free(tmp);
        return (EXIT_FAILURE);
    }

    gen_list(X, N);
    if (print)
    {
        printf("Before\n");
        print_array(X, N);
    }

    double begin = omp_get_wtime();
#pragma omp parallel
    {
#pragma omp single
        mergesort(X, N, tmp);
    }
    double end = omp_get_wtime();
    if (print)
    {
        printf("After\n");
        print_array(X, N);
    }

    printf("Time: %f (s) \n", end - begin);

    assert(1 == is_sorted(X, N));

    free(X);
    free(tmp);
    return (EXIT_SUCCESS);
}

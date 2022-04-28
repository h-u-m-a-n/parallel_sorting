#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "utils.c"

#define TASK_SIZE 100

int partition(int *a, int p, int r)
{
    int lt[r - p];
    int gt[r - p];
    int i;
    int j;
    int key = a[r];
    int lt_n = 0;
    int gt_n = 0;

    for (i = p; i < r; i++)
    {
        if (a[i] < a[r])
        {
            lt[lt_n++] = a[i];
        }
        else
        {
            gt[gt_n++] = a[i];
        }
    }

    for (i = 0; i < lt_n; i++)
    {
        a[p + i] = lt[i];
    }

    a[p + lt_n] = key;

    for (j = 0; j < gt_n; j++)
    {
        a[p + lt_n + j + 1] = gt[j];
    }

    return p + lt_n;
}

void quicksort(int *a, int p, int r)
{
    int div;

    if (p < r)
    {
        div = partition(a, p, r);
#pragma omp task shared(a) if (r - p > TASK_SIZE)
        quicksort(a, p, div - 1);
#pragma omp task shared(a) if (r - p > TASK_SIZE)
        quicksort(a, div + 1, r);
    }
}

int main(int argc, char *argv[])
{
    srand(123456);
    int N = (argc > 1) ? atoi(argv[1]) : 10;
    int print = (argc > 2) ? atoi(argv[2]) : 0;
    int numThreads = (argc > 3) ? atoi(argv[3]) : 2;
    int *X = malloc(N * sizeof(int));

    omp_set_dynamic(0); // Explicitly disable dynamic teams
    omp_set_num_threads(numThreads);

    // Fail memory allocation
    if (!X)
    {
        if (X)
            free(X);
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
        quicksort(X, 0, N);
    }
    double end = omp_get_wtime();

    assert(1 == is_sorted(X, N));

    if (print)
    {
        printf("After\n");
        print_array(X, N);
    }

    printf("Time: %f (s) \n", end - begin);
    free(X);
    return (EXIT_SUCCESS);
}

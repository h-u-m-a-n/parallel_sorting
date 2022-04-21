// Utility functions for sorting algorithms

void gen_list(int *x, int n)
{
   int i, j, t;
   for (i = 0; i < n; i++)
      x[i] = i;
   for (i = 0; i < n; i++)
   {
      j = rand() % n;
      t = x[i];
      x[i] = x[j];
      x[j] = t;
   }
}

void print_array(int *x, int n)
{
   int i;
   for (i = 0; i < n; i++)
   {
      printf("%d ", x[i]);
   }
   printf("\n");
}

int is_sorted(int *a, int size)
{
   for (int i = 0; i < size - 1; i++)
      if (a[i] > a[i + 1])
         return 0;
   return 1;
}

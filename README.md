# Parallel sorting
Implementation of parallel sorting using openMP

# Compilation
`gcc -fopenmp -o quick_sort.out quick_sort.c`  
`gcc -fopenmp -o merge_sort.out merge_sort.c`

# Run
`./quick_sort.out <size> <print> <n>`  
`./merge_sort.out <size> <print> <n>`  
size: integer, size of array  
print: (0 or 1), show array  
n: integer, number of threads

# Generate report
`python report.py`

# Video
[Presentation video](https://www.youtube.com/watch?v=2-IGTNijc4w)  
[Explanation video](https://youtu.be/TmlpD2BFiLA)
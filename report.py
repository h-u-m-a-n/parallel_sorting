import os

for stype in ['merge_sort', 'quick_sort']:
    print(stype)
    for i in range(7):
        size = 10 ** i
        for n in [1, 4, 8]:
            os.system(f"./{stype}.out {size} 0 {n}")
        print()
    print()

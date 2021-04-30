cfs = [1.5748, -0.4681, -0.1873, 1.8556]

for i in range(4, 21): 
    bins = [bin(int(round(x * 2**i))) for x in cfs]
    print(bins)


import timeit
import itertools
import cProfile

import sys, os.path
root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
tools_dir = os.path.join(root_dir, 'src/tools')
sys.path.append(tools_dir)

import random
from despy import * 

def do_bench_des():
    num_keys = 2000
    samples_per_key = 1000
    keys = [random.randint(0, 2**64 -1) for _ in range(num_keys)]
    blocks = []
    for _ in range(num_keys):
        blocks += [random.randint(0, 2**64 -1) for _ in range(samples_per_key)]
    ssl_speed = bench_des_openssl(keys, blocks, samples_per_key)
    my_speed = bench_des(keys, blocks, samples_per_key)
    factor_faster = (1 / ssl_speed) / (1 / my_speed)
    print("SSL is", factor_faster, " faster than mine")

if __name__ == "__main__":
    print("Benchmarking des")
    do_bench_des()


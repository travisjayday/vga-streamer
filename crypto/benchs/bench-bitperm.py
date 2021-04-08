import timeit
import itertools
import cProfile

import sys, os.path
root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
tools_dir = os.path.join(root_dir, 'src/tools')
sys.path.append(tools_dir)

from permgen import * 
from permpy import *

def bench_nbit_perm(n, masks_n, samples_per_mask):

    # generate permutations to execute
    raw_masks = [(x, s, d) for x, s, d in generate_random_masks(n, masks_n)]

    # generate samples numbers to that will be fed into permutations
    total_samples = masks_n * samples_per_mask
    samples = [random.randint(0, 2**n - 1) for _ in range(total_samples)]
    samples_data = (ctypes.c_uint64 * total_samples)(*samples)

    print("Benchmarking Bitperms...")

    #########################
    # Benchmark Dumb Python #
    #########################

    t = time.time()
    i = 0
    for _, src, dst in raw_masks:
        for _ in range(samples_per_mask): 
            dumb_perm(samples[i], src, dst)
            i += 1
    duration = time.time() - t
    python_speed = duration / (masks_n * samples_per_mask)

    print("\nPython Results:")
    print("---------------")
    print("Total seconds:\t", duration)
    print("Seconds / Call:\t", python_speed)
    print("Sanity Check:\t", hex(dumb_perm(samples[-1], raw_masks[-1][1], raw_masks[-1][2])))
    print('')

    #########################
    # Benchmark Dumb Native #
    #########################

    srcs = [[ord(a) - ord('A') for a in s] for x, s, d in raw_masks]
    dsts = [[ord(a) - ord('A') for a in d] for x, s, d in raw_masks]
    srcs_data = [(ctypes.c_uint8 * len(x))(*x) for x in srcs]
    dsts_data = [(ctypes.c_uint8 * len(x))(*x) for x in dsts]
    srcs_ptrs = (ctypes.POINTER(ctypes.c_uint8) * masks_n)(*srcs_data)
    dsts_ptrs = (ctypes.POINTER(ctypes.c_uint8) * masks_n)(*dsts_data)
    native_dumb_speed = bench_perm_bitstring_dumb(
            n, srcs_ptrs, dsts_ptrs, masks_n, samples_data, samples_per_mask)

    #########################
    # Benchmark Fast Native #
    #########################

    # convert the list of lists to a list of c, uint64 arrays
    mask_data = [(ctypes.c_uint64 * len(x))(*x) for x, y, z in raw_masks]

    # create an array of Uint64 pointers from the data arrays
    mask_ptrs = (ctypes.POINTER(ctypes.c_uint64) * masks_n)(*mask_data)
    native_fast_speed = bench_perm_bitstring(
            n, mask_ptrs, masks_n, samples_data, samples_per_mask)

    time.sleep(1)

    # Print final results
    print("\nSpeed-up Factor Results")
    print("-----------------------")
    print("Dumb C vs Dumb Python\t", round(python_speed / native_dumb_speed, 2))
    print("Fast C vs Dumb C\t", round(native_dumb_speed / native_fast_speed, 2))
    print("Fast C vs Dumb Python\t", round(python_speed / native_fast_speed, 2))

if __name__ == "__main__":
    bench_nbit_perm(64, 1000, 10000)


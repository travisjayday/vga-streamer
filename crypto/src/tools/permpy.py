import os
import time
import random
import ctypes

# Load dynamic library 
libbitperm_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../build/libbitperm.so'))
libbitperm = ctypes.CDLL(libbitperm_path)

# Define fast native function
libbitperm.perm_bitstring.restype = ctypes.c_ulong
libbitperm.perm_bitstring.argtypes = (
        ctypes.c_uint64,                                    # uint64_t x
        ctypes.c_uint8,                                     # uint8_t n
        ctypes.POINTER(ctypes.c_uint64)                     # uint64_t* mask
)

# Define dumb native function
libbitperm.perm_bitstring_dumb.restype = ctypes.c_ulong
libbitperm.perm_bitstring_dumb.argtypes = (
        ctypes.c_uint64,                                    # uint64_t x
        ctypes.c_uint8,                                     # uint8_t n
        ctypes.POINTER(ctypes.c_uint8),                     # uint8_t* src
        ctypes.POINTER(ctypes.c_uint8)                      # uint8_t* dst
)

# Define the fast benchmark function
libbitperm.bench_perm_bitstring.restype = ctypes.c_double
libbitperm.bench_perm_bitstring.argtypes = (
        ctypes.c_uint8,                                     # uint8_t n 
        ctypes.POINTER(ctypes.POINTER(ctypes.c_uint64)),    # uint64_t** masks
        ctypes.c_int,                                       # int masks_n
        ctypes.POINTER(ctypes.c_uint64),                    # uint64_t* samples
        ctypes.c_int                                        # int samples_n
)

# Define the dumb benchmark function
libbitperm.bench_perm_bitstring_dumb.restype = ctypes.c_double
libbitperm.bench_perm_bitstring_dumb.argtypes = (
        ctypes.c_uint8,                                     # uint8_t n 
        ctypes.POINTER(ctypes.POINTER(ctypes.c_uint8)),     # uint8_t** srcs
        ctypes.POINTER(ctypes.POINTER(ctypes.c_uint8)),     # uint8_t** dsts
        ctypes.c_int,                                       # int perms_n 
        ctypes.POINTER(ctypes.c_uint64),                    # uint64_t* samples
        ctypes.c_int                                        # int samples_n
)

def perm_bitstring(x, n, masks):
    """
    Permutates a given bitstring using the permutation defined by the given masks. 
    @x: The integer to permute
    @n: The bit-width of x 
    @masks: A list of integer delta-transform masks
    """
    global libbitperm
    masks_arr = (ctypes.c_uint64 * len(masks))(*masks)
    return libbitperm.perm_bitstring(x, n, masks_arr)

def dumb_perm(x, src_perm, dst_perm):
    """
    Permutates a given bitstring using the permutation defined by src_perm 
    and dst_perm. Uses dumb python datastructures.
    @x: The integer to permute
    @start: array with elements. For example, ['A', 'B', 'C', 'D']
    @end:   array with permutated elemented. For example, ['D', 'C', 'B', 'A']
    """
    mapping = {x: y for x, y in zip(src_perm, list(format(x, f'0{len(src_perm)}b')))}
    return int(''.join([mapping[x] for x in dst_perm]), 2)

def dumb_perm_native(x, src_perm, dst_perm):
    srcs = [ord(a) - ord('A') for a in src_perm] 
    dsts = [ord(a) - ord('A') for a in dst_perm]
    srcs_data = (ctypes.c_uint8 * len(srcs))(*srcs)
    dsts_data = (ctypes.c_uint8 * len(dsts))(*dsts)
    return libbitperm.perm_bitstring_dumb(x, len(src_perm), srcs_data, dsts_data)

def bench_perm_bitstring_dumb(n, srcs_ptrs, dsts_ptrs, \
        masks_n, samples_data, samples_per_mask):
    return libbitperm.bench_perm_bitstring_dumb(
            n, srcs_ptrs, dsts_ptrs, masks_n, samples_data, samples_per_mask)

def bench_perm_bitstring(n, mask_ptrs, masks_n, samples_data, samples_per_mask):
    return libbitperm.bench_perm_bitstring(
            n, mask_ptrs, masks_n, samples_data, samples_per_mask)


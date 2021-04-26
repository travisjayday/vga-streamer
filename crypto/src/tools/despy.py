import os
import time
import random
import ctypes

# Load dynamic library 
libdes_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../build/libdes.so'))
libdes = ctypes.CDLL(libdes_path)

# Define fast native function   
libdes.encrypt_des_blk.restype = ctypes.c_uint64
libdes.encrypt_des_blk.argtypes = (
        ctypes.c_uint64,                                    # uint64_t block
        ctypes.c_uint64,                                    # uint8_t  key
)

# Define openssl DES function
libdes.bench_des_openssl.restype = ctypes.c_double
libdes.bench_des_openssl.argtypes = (
        ctypes.POINTER(ctypes.c_uint64),                    # uint64_t* blocks
        ctypes.c_int,                                       # int blocks_n
        ctypes.POINTER(ctypes.c_uint64),                    # uint64_t* keys
        ctypes.c_int,                                       # int keys_n
        ctypes.c_int                                        # int samples_per_key
)

# Define my DES function
libdes.bench_des.restype = ctypes.c_double
libdes.bench_des.argtypes = (
        ctypes.POINTER(ctypes.c_uint64),                    # uint64_t* blocks
        ctypes.c_int,                                       # int blocks_n
        ctypes.POINTER(ctypes.c_uint64),                    # uint64_t* keys
        ctypes.c_int,                                       # int keys_n
        ctypes.c_int                                        # int samples_per_key
)

def encrypt_des_blk(block, key):
    return libdes.encrypt_des_blk(block, key)

def bench_des_openssl(keys, blocks, samples_per_key): 
    blocks_data = (ctypes.c_uint64 * len(blocks))(*blocks)
    keys_data = (ctypes.c_uint64 * len(keys))(*keys)
    return libdes.bench_des_openssl(
            blocks_data, len(blocks), 
            keys_data, len(keys),
            samples_per_key)

def bench_des(keys, blocks, samples_per_key): 
    blocks_data = (ctypes.c_uint64 * len(blocks))(*blocks)
    keys_data = (ctypes.c_uint64 * len(keys))(*keys)
    return libdes.bench_des(
            blocks_data, len(blocks), 
            keys_data, len(keys),
            samples_per_key)


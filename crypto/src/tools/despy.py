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

def encrypt_des_blk(block, key):
    return libdes.encrypt_des_blk(block, key)

import random

import sys, os.path
root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
tools_dir = os.path.join(root_dir, 'src/tools')
sys.path.append(tools_dir)

from permgen import * 
from permpy import *

def test_perm(src_perm, dst_perm, test_dumb_c=True): 
    masks = render_mask(get_masks(src_perm, dst_perm))

    for _ in range(255): 
        x = random.randint(0, 2**len(src_perm) - 1)
        r1 = perm_bitstring(x, len(src_perm), masks)
        r2 = dumb_perm(x, src_perm, dst_perm)
        if test_dumb_c:
            r3 = dumb_perm_native(x, src_perm, dst_perm) 
        else:
            r3 = r2
        if r1 != r2 or r1 != r3 or r2 != r3: 
            print("n =", len(src_perm))
            print("Test failed!!")
            print("Native Fast\t", format(r1, '#034b'))
            print("Python\t\t", format(r2, '#034b'))
            if test_dumb_c:
                print("Native Slow", bin(r3))
            quit()

def test_nbit_perms(n):
    abc = [chr(ord('A') + x) for x in range(0, 64)]
    src_perm = abc[0:n]
    dst_perm = abc[0:n] 

    for _ in range(200):
        random.shuffle(src_perm)
        random.shuffle(dst_perm)
        test_perm(src_perm, dst_perm)

    print(f"{n}-bit bitperm test succeeded...")

def test_all():
    test_nbit_perms(8)
    test_nbit_perms(16)
    test_nbit_perms(32)
    test_nbit_perms(64)

if __name__ == "__main__":
    test_all()

from permgen import *
import timeit
import itertools

def test_perm(src_perm, dst_perm): 
    masks = render_masks(get_masks(src_perm, dst_perm))
    print(masks)

    for _ in range(255): 
        x = random.randint(0, 2**len(src_perm) - 1)
        r1 = perm_bitstring(x, len(src_perm), masks)
        r2 = dumb_perm(x, src_perm, dst_perm)
        if r1 != r2: 
            print("Test failed!!")
            print(bin(r1))
            print(bin(r2))
            quit()

    print("Tests succeeeded!")

def test_all_8bit_perms():
    abc = [chr(ord('A') + x) for x in range(0, 64)]
    src = abc[0:8]
    for dst in itertools.permutations(src):
        test_perm(src, dst)

def test_nbit_perms(n):
    abc = [chr(ord('A') + x) for x in range(0, 64)]
    src_perm = abc[0:n]
    dst_perm = abc[0:n] 

    for _ in range(200):
        random.shuffle(src_perm)
        random.shuffle(dst_perm)
        test_perm(src_perm, dst_perm)

import cProfile
cProfile.run('test_nbit_perms(64)')
quit()

test_nbit_perms(8)
test_nbit_perms(16)
test_nbit_perms(32)
test_nbit_perms(64)

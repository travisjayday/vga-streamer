from permgen import *
import timeit
import itertools
import cProfile

def test_perm(src_perm, dst_perm, test_dumb_c=True): 
    masks = render_mask(get_masks(src_perm, dst_perm))
    print(masks)

    for _ in range(255): 
        x = random.randint(0, 2**len(src_perm) - 1)
        x = 0x5c82b597
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

def bench_nbit_perm(n, masks_n, samples_per_mask):

    # generate permutations to execute
    raw_masks = [(x, s, d) for x, s, d in generate_random_masks(n, masks_n)]

    # generate samples numbers to that will be fed into permutations
    total_samples = masks_n * samples_per_mask
    samples = [random.randint(0, 2**n - 1) for _ in range(total_samples)]
    samples_data = (ctypes.c_uint64 * total_samples)(*samples)


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
    native_dumb_speed = so.bench_perm_bitstring_dumb(
            n, srcs_ptrs, dsts_ptrs, masks_n, samples_data, samples_per_mask)

    #########################
    # Benchmark Fast Native #
    #########################

    # convert the list of lists to a list of c, uint64 arrays
    mask_data = [(ctypes.c_uint64 * len(x))(*x) for x, y, z in raw_masks]

    # create an array of Uint64 pointers from the data arrays
    mask_ptrs = (ctypes.POINTER(ctypes.c_uint64) * masks_n)(*mask_data)
    native_fast_speed = so.bench_perm_bitstring(
            n, mask_ptrs, masks_n, samples_data, samples_per_mask)

    time.sleep(1)

    # Print final results
    print("\nSpeed-up Factor Results")
    print("-----------------------")
    print("Dumb C vs Dumb Python\t", round(python_speed / native_dumb_speed, 2))
    print("Fast C vs Dumb C\t", round(native_dumb_speed / native_fast_speed, 2))
    print("Fast C vs Dumb Python\t", round(python_speed / native_fast_speed, 2))

def test_all():
    #test_nbit_perms(8)
    #test_nbit_perms(16)
    test_nbit_perms(32)
    test_nbit_perms(64)

if __name__ == "__main__":
    bench_nbit_perm(64, 1000, 10000)
    #test_all()
#cProfile.run('test_all()')
#bench_nbit_perm(64, 1, 1)


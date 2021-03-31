import os
import time
import random
import ctypes

# Prepare the permutate function
so = ctypes.CDLL(os.path.abspath('./build/libdes.so'))
so.perm_bitstring.restype = ctypes.c_ulong
so.perm_bitstring.argtypes = (ctypes.c_uint64, ctypes.c_uint8, 
        ctypes.POINTER(ctypes.c_uint64))

# Define the fast benchmark function
so.bench_perm_bitstring.restype = ctypes.c_double
so.bench_perm_bitstring.argtypes = (
        ctypes.c_uint8,                                     # uint8_t n 
        ctypes.POINTER(ctypes.POINTER(ctypes.c_uint64)),    # uint64_t** masks
        ctypes.c_int,                                       # int masks_n
        ctypes.POINTER(ctypes.c_uint64),                    # uint64_t* samples
        ctypes.c_int                                        # int samples_n
)

# Define the dumb benchmark function
so.bench_perm_bitstring_dumb.restype = ctypes.c_double
so.bench_perm_bitstring_dumb.argtypes = (
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
    global so
    masks_arr = (ctypes.c_uint64 * len(masks))(*masks)
    return so.perm_bitstring(x, n, masks_arr)

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
 

def get_masks(start, end):
    """
    Given a starting permutation and an ending permutation, compute the 
    masks for it recursively. 
    @start: array with elements. For example, ['A', 'B', 'C', 'D']
    @end:   array with permutated elemented. For example, ['D', 'C', 'B', 'A']
    """
    n = len(start)

    # base case. A simple Mux. 
    if n == 2: 
        return [[0, 0]] if start[0] == end[0] else [[0, 1]]

    start_s1 = start[0:n//2] 
    start_s2 = start[n//2:n]
    end_s1 = end[0:n//2]
    end_s2 = end[n//2:n]

    # These are lists of tuples, each tuple represents a Mux
    start_s = [(x, y) for x, y in zip(start_s1, start_s2)]
    end_s = [(x, y) for x, y in zip(end_s1, end_s2)]
   
    # Masks keep track of which Muxes have been flipped 
    mask_start = [0] * (n // 2) 
    mask_end = [0] * (n // 2)

    # It is our goal to make set(start_s,0) == set(end_s,0). To do this,
    # we assert that the first mux position at start_s[0] is correct. 
    # Then all the other mux positions follow uniquely. We can solve this
    # using a ping pong game. We know start_s[0][0] is correct, hence the 
    # we need start_s[0][0] to be in the ending subset. Thus, we "want" 
    # start_s[0][0]. In the ending set, once we set start_s[0][0], we know 
    # that the other elemnt that gets replaced by start_s[0][0] in end_s
    # is "unwanted" in start_s. So we go back to start_s and flip again.
    s1, s2 = 0, 42
    unwanted = None
    wanted = start_s[0][0]
    past_wanted = set()     # keep tab on past wanted sets, to avoid cycles
    pings = 0

    def ping_back(arr, mask):
        nonlocal wanted, unwanted, pings
        pings += 1

        # Check if we've already encountered this item. If so, we're
        # free to assert that any next item is in the correct position
        if wanted in past_wanted: 
            for x in start_s:
                if x[0] not in past_wanted:
                    wanted = x[0]

        for i in range(len(arr)): 
            if arr[i][0] == unwanted:
                # A unwanted element is in a wanted position. Cross Now. 
                arr[i] = arr[i][::-1] 
                mask[i] = (mask[i] + 1) % 2
                wanted = arr[i][0]
                unwanted = None
                break
            if arr[i][0] == wanted:
                # A wanted element is in a wanted position. Hence, it's 
                # compliment must be unwanted.
                unwanted = arr[i][1] 
                past_wanted.add(wanted)
                wanted = None
                break
            if arr[i][1] == unwanted:
                # An unwanted element is in an unwanted position. Hence, 
                # its complimemt must be wanted.
                wanted = arr[i][0] 
                unwanted = None
                break
            if arr[i][1] == wanted:
                # A wanted element is in an unwanted position. Cross Now. 
                arr[i] = arr[i][::-1] 
                mask[i] = (mask[i] + 1) % 2
                unwanted = arr[i][1]
                past_wanted.add(wanted)
                wanted = None
                break

    # while set([x[0] for x in start_s]) != set([x[0] for x in end_s]), play pingpong
    # the upper bound to this is n//2
    for _ in range(n//2):
        # play the ping pong game until the sets are equal 
        ping_back(end_s, mask_end)
        ping_back(start_s, mask_start)

    # generate the final subwords based on the correct sets
    start_s1 = [x[0] for x in start_s]
    start_s2 = [x[1] for x in start_s]
    end_s1 = [x[0] for x in end_s]
    end_s2 = [x[1] for x in end_s]

    # create the first layer mask. Note that only the latter half matters, as 1 
    # signfiies the destination index to go into when we do the delta shift. 
    # Everything else (higher half) should be zero
    zero_subword = [0] * (n//2)
    masks = [zero_subword + mask_start] 

    # recurse deeper to generate all the inner layers
    mask_sublayer_left = get_masks(start_s1, end_s1)
    mask_sublayer_right = get_masks(start_s2, end_s2)

    # interweave the separate sublayer masks to create the full n-length 
    # masks for each sublayer
    masks += [x + y for x, y in zip(mask_sublayer_left, mask_sublayer_right)]

    # create the mask for the final layer. 
    masks.append(zero_subword + mask_end)

    return masks

def render_mask(m): 
    """ 
    Given a mask array representation from gen_mask, return a list
    of integers that represents that list of masks
    """
    masks = []
    for mask in m:
        res = int("".join(str(x) for x in mask), 2)
        masks.append(res)
    return masks

def pretty_print_mask(title, cvars, n, m):
    """
    Prints a mask in a nice, c-convertable way. 
    @title: Decorative text to print to console
    @cvars: C-variabel name or a list of C variable names
    @n:     Bitsize of the mask
    @m:     The mask or a list of masks
    """
    title = title.center(20)
    print('\n/**' + ''.join(['*'] * len(title)) + '**/')
    print('/*', title, '*/')
    print('/**' + ''.join(['*'] * len(title)) + '**/')
    if type(m[0]) != list:
        m = [m]
        cvars = [cvars]
    for var, mask_collection in zip(cvars, m):  
        print('uint64_t', var, '{')
        print(" ", end='')
        for i in range(len(mask_collection) - 1):
            print(" {0:#0{1}x},".format(mask_collection[i], (n >> 2) + 2),\
                    end='' if (i + 1) % (13 - (n >> 2)) != 0 else '\n ')
        print(" {0:#0{1}x}".format(mask_collection[-1], (n >> 2) + 2))
        print('};')

def print_des_masks():
    """
    Print all the masks required for DES
    """
    bit64 = [x for x in range(1, 65)]

    def print_s(n, d):
        title = f"S-Block {n}"
        title = title.center(20)
        print('\n/**' + ''.join(['*'] * len(title)) + '**/')
        print('/*', title, '*/')
        print('/**' + ''.join(['*'] * len(title)) + '**/')
     
        rows = [d[i*16: i*16 + 16] for i in range(4)] 
        h = [["{0:01x}".format(x) for x in m] for m in rows]
        for row in h:
            print(''.join(row)), end=', ')

    # s1 only acting on the first 16 bits of data
    s1 = [
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
    ]
    s2 = [
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
    ]
    s3 = [
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
    ]
    s4 = [
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
    ]
    s5 = [
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
    ]
    s6 = [
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
    ]
    s7 = [
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
    ]
    s8 = [
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
    ]

    for i in range(1, 9):
        print_s(i, locals()[f"s{i}"])

    pc1 = [
        57, 49, 41, 33, 25, 17,  9, 
         1, 58, 50, 42, 34, 26, 18,
        10,  2, 59, 51, 43, 35, 27, 
        19, 11,  3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
         7, 62, 54, 46, 38, 30, 22, 
        14,  6, 61, 53, 45, 37, 29,
        21, 13,  5, 28, 20, 12,  4
    ]
    dontcare = list(set(bit64) - set(pc1))
    random.shuffle(dontcare)
    pc1 = dontcare + pc1

    pretty_print_mask(
            "PC-1 Permutation Mask", 
            "perm_mask_p1", 64,
            render_mask(get_masks(bit64, pc1))
    )

    pc2 = [
        14, 17, 11, 24,  1,  5, 
         3, 28, 15,  6, 21, 10,
        23, 19, 12,  4, 26,  8,
        16,  7, 27, 20, 13,  2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    ]
    dontcare = list(set(bit64) - set(pc2))
    random.shuffle(dontcare)
    pc2 = dontcare + pc2
    pretty_print_mask(
            "PC-2 Permutation Mask", 
            "perm_mask_pc2", 64,
            render_mask(get_masks(bit64, pc2))
    )


def generate_random_masks(n, count): 
    abc = [chr(ord('A') + x) for x in range(0, 64)]
    src_perm = abc[0:n]
    dst_perm = abc[0:n] 

    for _ in range(count):
        random.shuffle(src_perm)
        random.shuffle(dst_perm)
        mask = render_mask(get_masks(src_perm, dst_perm))
        yield mask, src_perm, dst_perm

 
if __name__ == "__main__":
    print_des_masks()



'''
def swap(array, i, j):
    t = array[i]
    array[i] = array[j]
    array[j] = t

def butterfly_n(given, n_div2, mask):
    """ Computes the n-way butterfly of a given input.
            given   = list of elements, 
            n       : len(given) // 2, 
            mask    : bitstring 
    """
    output = given[:]
    [swap(output, n_div2  + i, i) for i in range(n_div2) if mask[i] == '1']
    return output 

def force(n): 
    print("Forcing", n, "bit")
    n_div2 = n // 2
    steps = 2**(n_div2)
    elems = bit64[0:n]
    for i in range(steps): 
        mask = format(i, f'0{n_div2}b')
        print(mask, '-->', butterfly_n(elems, n_div2, mask))

#elems = bit64[0:8]
#elems = butterfly_n(elems, 4, format(0x33, f'0{8}b'))
#force(16)

'''

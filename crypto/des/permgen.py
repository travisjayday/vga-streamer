import os
import time
import random
import ctypes

# Prepare the permutate function
so = ctypes.CDLL(os.path.abspath('./build/libdes.so'))
so.perm_bitstring.restype = ctypes.c_ulong
so.perm_bitstring.argtypes = (ctypes.c_uint64, ctypes.c_uint8, 
        ctypes.POINTER(ctypes.c_uint64))

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
 
    x = list(format(x, f'0{len(src_perm)}b'))
    mapping = {x: y for x, y in zip(src_perm, x)}
    result = [mapping[x] for x in dst_perm]
    result = int(''.join(result), 2)
    return result
 

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


    # it is our goal to make set(start_s,0) == set(end_s,0). To do this,
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

def render_masks(m): 
    """ 
    Given a mask array representation from gen_mask, return a list
    of integers that represents that list of masks
    """

    masks = []
    for mask in m:
        res = int("".join(str(x) for x in mask), 2)
        masks.append(res)
    return masks

def pretty_print_mask(title, n, m):
    print('')
    print(title)
    print(''.join(['-'] * len(title)))
    for mask in m:
        print("{0:#0{1}x}".format(mask, 20))
    print('')

def print_des_masks():
    bit64 = [x for x in range(1, 65)]
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
            "PC-1 Permutation Mask", 16,
            render_masks(get_masks(bit64, pc1))
    )

 
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

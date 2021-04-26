import os
import time
import random


logsilent = False
logbuf = ''
def log(*args, **kwargs):
    global logbuf, logsilent
    logbuf += ' '.join(map(str, args))
    def_end = '\n'
    if 'end' in kwargs:
        def_end = kwargs['end']
    logbuf += def_end
    if not logsilent:
        print(*args, **kwargs)

def get_log():
    global logbuf
    return logbuf

def log_level(silent):
    global logsilent
    logsilent = silent
 

def get_masks(start, end):
    """
    Given a starting permutation and an ending permutation, compute the 
    masks for it recursively. 
    @start: array with elements. For example, ['A', 'B', 'C', 'D']
    @end:   array with permutated elemented. For example, ['D', 'C', 'B', 'A']
    """
    n = len(start)
    if set(start) != set(end):
        return print("Error! Not a permutation!!")

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
    log('\n/**' + ''.join(['*'] * len(title)) + '**/')
    log('/*', title, '*/')
    log('/**' + ''.join(['*'] * len(title)) + '**/')
    if type(m[0]) != list:
        m = [m]
        cvars = [cvars]
    for var, mask_collection in zip(cvars, m):  
        log(f'uint64_t {var}[] =' + '{')
        log(" ", end='')
        for i in range(len(mask_collection) - 1):
            alpha = 14 if n == 64 else 12 
            # change the first value after % to modify numbers per line 
            log(" {0:#0{1}x},".format(mask_collection[i], (n >> 2) + 2),\
                    end='' if (i + 1) % (alpha - (n >> 2)) != 0 else '\n ')
        log(" {0:#0{1}x}".format(mask_collection[-1], (n >> 2) + 2))
        log('};')
 
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
    src = ['A', 'B', 'C', 'D']
    dst = ['B', 'C', 'D', 'A']
    get_masks(src, dst)

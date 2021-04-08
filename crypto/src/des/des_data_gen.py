import sys, os.path
root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '../..'))
tools_dir = os.path.join(root_dir, 'src/tools')
sys.path.append(tools_dir)

from permgen import *

def print_des_masks(to_file='', silent=False):
    """
    Print all the masks required for DES
    """
    bit64 = [x for x in range(1, 65)]

    log_level(silent)

    def print_s(n, d, last):
        rows = [d[i*16: i*16 + 16] for i in range(4)] 

        def printrow(row, comma):
            i = 0
            log('  ', end='')
            endings = [', ' for _ in range(len(row) - 1)]
            if not comma: endings[-1] = ''
            while i < len(row):
                log("{0:#04x}".format(row[i] | row[i + 1] << 4), end=endings[i])
                i += 2
            log('')
 
        printrow(rows[0], True)
        printrow(rows[2], True)
        printrow(rows[1], True)
        printrow(rows[3], not last)

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

    title = f"S-Blocks 1-8"
    title = title.center(20)
    log('\n/**' + ''.join(['*'] * len(title)) + '**/')
    log('/*', title, '*/')
    log('/**' + ''.join(['*'] * len(title)) + '**/')
    log('/* \n' +
        ' * S-Block defined as bytes. High nibble corresponds to \n' + 
        ' * i + 1 while low nibble corresponds to i. \n' +
        '*/') 

    log("uint8_t sblocks[] = {")
    for i in range(1, 9):
        print_s(i, locals()[f"s{i}"], i == 8)
    log("};")

    p = [
        16,  7, 20, 21,
        29, 12, 28, 17,
         1, 15, 23, 26, 
         5, 18, 31, 10,
         2,  8, 24, 14,
        32, 27,  3,  9,
        19, 13, 30,  6,
        22, 11,  4, 25
    ]
    pretty_print_mask(
            "P Permutation Mask",
            "perm_mask_p", 32, 
            render_mask(get_masks(bit64[0:32], p)) 
    )

    ip = [
        58, 50, 42, 34, 26, 18, 10, 2, 
        60, 52, 44, 36, 28, 20, 12, 4, 
        62, 54, 46, 38, 30, 22, 14, 6, 
        64, 56, 48, 40, 32, 24, 16, 8, 
        57, 49, 41, 33, 25, 17, 9,  1, 
        59, 51, 43, 35, 27, 19, 11, 3, 
        61, 53, 45, 37, 29, 21, 13, 5, 
        63, 55, 47, 39, 31, 23, 15, 7
    ]

    pretty_print_mask(
            "IP Permutation Mask",
            "perm_mask_ip", 64, 
            render_mask(get_masks(bit64, ip)) 
    )

    ipi = [
        40, 8, 48, 16, 56, 24, 64, 32, 
        39, 7, 47, 15, 55, 23, 63, 31, 
        38, 6, 46, 14, 54, 22, 62, 30, 
        37, 5, 45, 13, 53, 21, 61, 29, 
        36, 4, 44, 12, 52, 20, 60, 28, 
        35, 3, 43, 11, 51, 19, 59, 27, 
        34, 2, 42, 10, 50, 18, 58, 26, 
        33, 1, 41,  9, 49, 17, 57, 25
    ]

    pretty_print_mask(
            "IP^-1 Permutation Mask",
            "perm_mask_ipi", 64, 
            render_mask(get_masks(bit64, ipi)) 
    )

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
            "perm_mask_pc1", 64,
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
    bit48 = bit64[-8:] + bit64[0:56]
    print(bit48)
    print(pc2)
    print("len of don't care:", len(dontcare))
    pretty_print_mask(
            "PC-2 Permutation Mask", 
            "perm_mask_pc2", 64,
            render_mask(get_masks(bit48, pc2))
    )

    if len(to_file) != 0: 
        with open(to_file, 'w') as f:
            f.write(get_log())

 
if __name__ == "__main__":
    des_data = os.path.join(root_dir, 'src/des/des_data.h')
    print_des_masks(to_file=des_data, silent=True)

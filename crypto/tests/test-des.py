import random
from Crypto.Cipher import DES

import sys, os.path
root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
tools_dir = os.path.join(root_dir, 'src/tools')
sys.path.append(tools_dir)

from despy import * 

def test_des_blk(num_keys): 
    print("Testing DES...")
    for _ in range(num_keys):
        key = random.randint(0, 2**64 -1)
        des = DES.new(key.to_bytes(8, byteorder='big'), DES.MODE_ECB)

        for _ in range(255):
            x = random.randint(0, 2**64 - 1)
            r1 = encrypt_des_blk(x, key)
            r2 = des.encrypt(x.to_bytes(8, byteorder='big'))
            r2 = int.from_bytes(r2, 'big')

            if r1 != r2:
                print('test_des k=',key,'x=', x, 'FAILED')
                print("r1=",hex(r1))
                print('r2=',hex(r2))
                quit(-1)
    print("Test succeeded!")


if __name__ == "__main__":
    test_des_blk(1000)

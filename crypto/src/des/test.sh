echo 0: 0000000000000000 | xxd -r | openssl enc -des-ecb -nopad -K 0000000000000000 | xxd 

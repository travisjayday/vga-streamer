target: 
	gcc jpg.c targa.c -g -lm -DTEST_JPEG
	./a.out

bench: 
	gcc -Ofast table.c bench.c jpg.c targa.c -g -lm -DBENCH
huff:
	gcc -g -g huff.c
	./a.out
asm: 
	clang -S -O3 jpg.c targa.c -lm

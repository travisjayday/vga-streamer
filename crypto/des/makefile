des: clean
	gcc des.c -o build/des.out
	gcc -shared -o build/libdes.so -fPIC -O3 des.c
	./build/./des.out

permgen: clean
	gcc permgen.c -o build/permgen.out
	./build/./permgen.out

clean: 
	rm -rf build
	mkdir -p build

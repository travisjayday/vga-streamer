des_src 	= ./src/des
bitperm_src = ./src/bitperm
build_dir  	= ./build
tools_dir	= ./src/tools
tests_dir   = ./tests
benchs_dir   = ./benchs
des_lib		= $(build_dir)/libdes.so
bitperm_lib	= $(build_dir)/libbitperm.so
libs 		= -Iinclude
cflags		= -fPIC -O3 $(libs)

des: clean bitperm
	python3 $(des_src)/des_data_gen.py
	gcc -c $(des_src)/des.c $(cflags) -o $(build_dir)/des.o
	gcc -shared -o $(des_lib) $(cflags) $(build_dir)/des.o $(build_dir)/bitperm.o
	gcc -o $(build_dir)/des.out $(cflags) $(build_dir)/des.o $(build_dir)/bitperm.o

bitperm:
	gcc -c $(bitperm_src)/bitperm.c $(cflags) -o $(build_dir)/bitperm.o

test-bitperm:
	python3 $(tests_dir)/test-bitperm.py

bench-bitperm: bitperm
	gcc -c $(bitperm_src)/bench_bitperm.c $(cflags) -o $(build_dir)/bench_bitperm.o
	gcc -shared -o $(bitperm_lib) $(cflags) $(build_dir)/bitperm.o $(build_dir)/bench_bitperm.o
	python3 $(benchs_dir)/bench-bitperm.py

clean: 
	rm -rf $(build_dir)
	mkdir -p $(build_dir)

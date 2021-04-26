cflags=-g -Iinclude -DMAKE
libs=-lX11 -lm
build_dir=./build
cc=gcc

jpeg: clean common 
	$(cc) -c jpeg/jcoder.c $(cflags) -o $(build_dir)/j.o $(libs)
	$(cc) -c common/debug.c $(cflags) -o $(build_dir)/debug.o $(libs)
	$(cc) -o $(build_dir)/j.out $(cflags) $(build_dir)/j.o $(build_dir)/debug.o $(libs)
	./build/./j.out

clean: 
	rm -rf $(build_dir)
	mkdir -p $(build_dir)


include=-I/home/tjz/software/opencv-master/install/include/opencv4/ -I../jpeg
libs_dynamic=-lX11 -lXext `pkg-config opencv4 --cflags --libs`
libs_static=-lX11 -lXext `pkg-config --cflags --libs opencv4`
opt=-Ofast -mfpmath=both -march=native -m64 -funroll-loops -mavx2 
build=build
bin=bin


main:
	g++ -Ofast -mavx2 screenshot.cpp -c -o $(build)/screenshot.o $(include) $(libs_dynamic) $(opt) 
	g++ -Ofast -mavx2 ../jpeg/jpg.c -c -o $(build)/jpg.o
	g++ -Ofast -mavx2 ../jpeg/table.c -c -o $(build)/table.o
	g++ -Ofast main.cpp -o $(bin)/server.out $(build)/screenshot.o $(build)/jpg.o $(build)/table.o $(include) $(libs_dynamic) $(opt) 

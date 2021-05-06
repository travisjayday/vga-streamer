include=-I/home/tjz/software/opencv-master/install/include/opencv4/ -I../jpeg
libs=-lX11 -lXext `pkg-config opencv4 --cflags --libs`
opt=-Ofast -mfpmath=both -march=native -m64 -funroll-loops -mavx2 
build=build


main:
	g++ screenshot.cpp -c -o $(build)/screenshot.o $(include) $(libs) $(opt) 
	g++ ../jpeg/jpg.c -c -o $(build)/jpg.o
	g++ main.cpp $(build)/screenshot.o $(build)/jpg.o $(include) $(libs) $(opt) 

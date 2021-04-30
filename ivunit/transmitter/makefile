include=-I/home/tjz/software/opencv-master/install/include/opencv4/ 
libs=-lX11 -lXext `pkg-config opencv4 --cflags --libs`
opt=-Ofast -mfpmath=both -march=native -m64 -funroll-loops -mavx2 
build=build


main:
	g++ screenshot.cpp -c -o $(build)/screenshot.o $(include) $(libs) $(opt) 
	g++ main.cpp $(build)/screenshot.o $(include) $(libs) $(opt) 

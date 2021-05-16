# The Transmitter Server 

This directory contains a standalone build of the transmitter. 
The transmitter listenns for an incoming connection
from an ESP32. Upon connection, it uses OpenCV to crop a portion of the screen, 
split it into YCbCr channels using a custom color conversion 
matrix, DCT compres and huffman encode it using the ../jpeg 
library, sends the frame to the ESP32 which relays it to the PSoC which 
puts it on the VGA monitor. 

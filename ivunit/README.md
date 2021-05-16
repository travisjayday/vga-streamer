# JPEG-Like Compressor/Decompressor

The ivunit provides JPEG-like encoding/decoding functionality used in the PSoC6. 

It also provides the transfer TCP server used to transfer a data stream to the ESP32 which relays it to the 
PSoC6 for decoding and visualization. Encoding takes place in the transmitter; decoding takes place in the PSoC6. 

The `ivunit/jpeg/` directory provides generic compression / decompression routines. 

The `ivunit/transmitter/` directory provides the server program that runs on the computer you wish to initiate the datastream from.  

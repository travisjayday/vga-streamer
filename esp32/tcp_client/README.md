# TCP-SPI Relay

This is the ESP32 firmware that connects to the TCP server to capture the outgoing
video datastream and relays the datastream into manageable SPI transactions that 
the PSoC6 can request. 

ESP32 is a TCP Client and an SPI slave. 


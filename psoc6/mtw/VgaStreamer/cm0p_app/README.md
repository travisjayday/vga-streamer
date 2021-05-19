# PSoC 6 MCU: CM0+ VGA Driver  

This CM0+ project provides the VGA capabilities to the PSoC6. 

The CM0+ Core is responsible for initializing hardware such as DMA controllers, 
TCPWM PWM generators, and shared VGA framebuffers necessary
for outputting a standard, 640x480 @ 60Hz VGA signal with a 
virtual resolution of 352x480 pixels. 

Ports 8 and Port 9 combined provide 15 bit digital color output
which needs to be converted into 0-0.7V signals using a DAC. 
Three 5-bit R2R ladder DACs with R=100Ohm and 2R=200Ohm work well.

```
	   Port8 (LSB->MSB)			 Port9 (LSB->MSB)
  [ X B0 B1 B2 B3 B4 G0 G1 ]	[ G2 G3 G4 R0 R1 R2 R3 R4 ]
	   \...DAC..../   \.....DAC......./   \...DAC..../
			|                |                 |
		 Analog B         Analog G          Analog R
```
 
The VGA framebuffer is defined in shared memory so that both
cores and the DMA controllers can access it. There are two
framebuffers, a Port8 and a Port9 framebuffer which correspond
to the above GPIO Ports. 

Pinout: 
-----
- Hsync - Pin 10.4 

- Vsync - Pin 11.4 
- RGB   - Port8 and Port9 (see above) 
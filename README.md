# PSoC6/ESP32 VGA Streamer 

The goal of this project is to livestream a compressed video feed from a Linux machine to a VGA montior using the PSoC6 MCU and the ESP32 Microcontroller. 

This GitHub repository contains many subprojects. 

See the READMEs in the subfolders.


## Livestreaming Video Demonstration
Watch this demo video of livestreaming gameplay to the VGA monitor over WiFi: [Link to video](https://drive.google.com/file/d/1-jbulYnPb3N8D912lSg62HLdD7m2vJCN/view?usp=sharing)

## VGA Resolution Benchmark
Native, Full Resolution of PSoC6 VGA. RGB555 color at 352x480 pixels.
![Alt text](images/FullResolutionVGATest.jpg?raw=true "Full Resolution VGA Test")

## Screenshot of Livestream
Lowered Resolution of Streaming Live Video at 30FPS. 
![Alt text](images/StreamResolution.jpg?raw=true "Low Resolution Realtime Video Stream @ 30FPS")

## Breadboard Layout.
![Alt text](images/BoardLayout.jpg?raw=true "Breadboard layout")

## Software Diagram Overview
![Alt text](images/ProjectDiagram.png?raw=true "Project Diagram")

## Detailed Project Writeup
For the full project writeup, and detailed explanations on how the various subsystems interact, see [the whitepaper PDF](https://github.com/travisjayday/vga-streamer/blob/main/images/whitepaper.pdf)

## Thanks
Thank you to the Cypress forum admins who helped me in my time of need as I werstled with the PSoC6. And thank you to Steve for doubting the achievability of this project :-)

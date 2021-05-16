# Modus Toolbox Workspace for the PSoC6 MCU

This directory contains the Modus Toolbox workspace that contains the firmware for streaming video to a VGA monitor. The project is located in VgaStreamer and is split into two sub-projects: one for each CPU core. 

The CM0+ Core (`VgaStreamer/cm0p_app`) handles outputting to the VGA monitor.

The CM4 Core (`VgaStreamer/cm4_app`) handles the processing of incoming compressed video. 

See the READMEs located in these sub-proejcts for more info.  

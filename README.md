# eeSoundboard

This is my 4351th attempt to build a soundboard that I'm actually happy with. The other tries did work, but I was rather disappointed with them. It was either the hardware that was too complex or fragile or the firmware with included non-free libraries. Now I have a rather rigid hardware setup and a completely open source firmware. Such yay.

## Hardware

The prototype is made from a WeAct 3.0 development board, which is available from aliexpress for ~4€ (including shipping). It is my current favourite dev board and it features a NOR flash footprint which is awesome because it eliminates the need for a second board for the memory (rigidity, remember?).

I will include a pinout table later, if you want to rebuild it with the same hardware.

The soundboard uses a cheap PAM8403 amplifier board.

## Building

Building is only possible via eclipse at the moment. I consider adding cmake support or alike. Checkout all submodules, build the libopencm3 using make and open the eclipse project. It requires GNU MCU Eclipse and a arm-none-eabi-gcc compiler (I recommend using the latest version). 

## Running 

Either use an ST-Link to flash the firmware or trigger the DFU-Mode and use `dfu-util -a 0 -s 0x08000000 -R -D Debug/Soundboard.bin`.

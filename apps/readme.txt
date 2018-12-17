this is the avr32 apps folder.

each app can implement a specialized UI on the avr32.

aleph/avr32_lib/src contains modules for handling most lower-level functions.

include and/or copy at will.

## Development

There are two development modes

1) Load Blackfin modules from SD card
2) Store Blackfin module in program memory and load at boot time

The first mode allows more on-device development and expermentation features at the compromise of using an SD card as a single point of failure. In addition, there is a limitation in the type of SD card which the device driver will recognize. Any card labled with SDHC class 6 or lower will work. Anything with a higher class number (the one in a circle) will silently fail. Any card labled SDXC will also silently fail.

This limitation is a compelling reason to develop programs which can be stored in program memory.

TODO: how much program memory is there and how do we get bytes into it?

A typical workflow for developing without an SD card goes like so

1. Build a dev toolchain on your laptop
1. Open source code in your editor of choice, like vim or emacs
1. Make changes to files using the C functions provided by the platform
1. Compile your changes
1. Run tests (TODO: make tests)
1. Copy the new firmware to an SD card (install.sh script in root dir)
1. Insert SD card into Aleph
1. Hold down the mode switch (big button at top right) and power on the device
1. Choose your firmware to load
1. Wait for the firmware to load
1. Power off device and remove SD card
1. Power on device

It should have your code loaded and boot instantly.

TODO: this process seems a bit error prone and time consuming. Loading 205K of firmware takes ~3 minutes, which could add up to a lot for iterative development. I guess having a compiler and unit tests is a way to ensure your code will actually run on the device before loading the firmware? Dunno.


## API reference

There are functions defined in utils and avr32 directories. Read the header files to learn the function interfaces to call in your code. Is there a better way to get function references? I imagine so for editors like vim and emacs. Ctags?

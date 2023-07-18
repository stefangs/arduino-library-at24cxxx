# AT24CXXX Two-wire Serial EEPROM Library
This is an Arduino library for using the AT24CXXX-series serial persistant memory chips. The libary supports the following chips:
* AT24C01  - 128 bytes
* AT24C02  - 256 bytes
* AT24C04  - 512 bytes
* AT24C08  - 1024 bytes
* AT24C16  - 2048 bytes
* AT24C128 - 16384 bytes
* AT24C256 - 32768 bytes

The library interface is drop in compatible with the Arduino built in EEPROM API, so code written for the internal EEPROM will work with this library without modification.

The libray has the following features:
* Uses the page write feature of the AT24C chips which is up to 64 times faster than doing single byte writes and reduces wear on the memory cells.
* Keeps track of page sizes for the different chips and adjusts writes to page borders
* Handles the Write Cycle Time of the chips, avoiding the risk of trying to access the chip while it is busy processing an earlier write
* Can read and write arbitrarily long buffers, managing limitations of buffer sizes in I2C libraries
* Transparent error handling making it easy to detect errors in the communication with the chip (very useful in early stages of a project)
* Can read and write basic types and structs directly

# Examples

## Setting up
You need to include the .h file for the chip type you are using and create a chip-object with the address it is configured at. Since the I2C-bus is used, you also have to start the Wire-interface:
```C++
#include "at24c256.h"

AT24C256 eprom(0x50);

void setup() {
  Wire.begin();
 
  uint8_t byte = eprom.read(0);
}
```

## Reading and writing basic types
All basic types, such as int, long, double can be read or written with the `put` and `get` methods. You just specify the memory address and the variable to read or write:
```C++
int foo = 42, foo_in;
eprom.put(0, foo);      // Write the integer value 42 to address 0
eprom.get(0, foo_in);   // Read the integer value at address 0 into variable foo_in
```


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
* Uses the page write feature of the AT24C chips which is up to 64 times faster that doing single byte writes and reduces wear on the memory cells.
* Keeps track of page sizes for the different chips and adjusts writes to page borders
* Handles the Write Cycle Time of the chips, avoiding the risk of trying to access the chip while it is busy processing an earlier write
* Can read and write arbitrarily long buffers, managing limitations of buffer sizes in I2C libraries
* Transparent error handling making it easy to detect errors in the communication with the chip (very useful in early stages of a project)
* Can read and write basic types and structs directly

# Examples

## Reading and writing an integer   
  int foo = 42, foo_in;
  eprom.put(BASE, foo);
  Serial.println(eprom.get(BASE, foo_in));



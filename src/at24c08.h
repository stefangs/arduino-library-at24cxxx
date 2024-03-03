#ifndef AT24C08_H
#define AT24C08_H

#include "at24cxxx.h"

// Valid addresses for AT24C08
#define AT24C08_ADDRESS_0 (0x50)
#define AT24C08_ADDRESS_1 (0x54)


class AT24C08 : public AT24Cxxx {

  public:
    AT24C08(uint8_t address, TwoWire& i2c = Wire, uint8_t writeDelay = 6) :
      AT24Cxxx(address, i2c, writeDelay, 1024, 16) {}

  protected:
    virtual void writeAddress(uint16_t address) {
      twoWire->beginTransmission((uint8_t)((i2cAddress & 0xFC) | ((address >> 8) & 0x03)));
      twoWire->write((uint8_t)(address & 0xFF));
    }
};

#endif

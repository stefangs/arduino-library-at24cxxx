#ifndef AT24C16_H
#define AT24C16_H

#include "at24cxxx.h"

// Note that the at24c16 has one fixed i2c address which cannot be altered 
// so it does not have an address parameter
class AT24C16 : public AT24Cxxx {

  public:
    AT24C16(TwoWire& i2c = Wire, uint8_t writeDelay = 6) :
      AT24Cxxx(AT24C_ADDRESS_0, i2c, writeDelay, 2048, 16) {}

  protected:
    virtual void writeAddress(uint16_t address) {
      twoWire->beginTransmission((uint8_t)((i2cAddress & 0xF8) | ((address >> 8) & 0x07)));
      twoWire->write((uint8_t)(address & 0xFF));
    }
};

#endif

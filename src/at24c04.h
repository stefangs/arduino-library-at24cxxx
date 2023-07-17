#include "at24cxxx.h"

class AT24C04 : public AT24Cxxx {

  public:
    AT24C04(uint8_t address, TwoWire& i2c = Wire, uint8_t writeDelay = 6) :
      AT24Cxxx(address, i2c, writeDelay, 512, 16) {}
};
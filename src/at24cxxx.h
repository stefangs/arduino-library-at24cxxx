
#include "Wire.h"

class AT24Cxxx {

  public:
    AT24Cxxx(uint8_t address, int size, TwoWire& i2c, int writeDelay);
    uint8_t read( int idx );
    void write( int idx, uint8_t val);
    void update( int idx, uint8_t val);
    uint16_t length();
    int writeBuffer(uint16_t address, const uint8_t* data, size_t len);
    int readBuffer(uint16_t address, uint8_t* data, uint8_t len);

    template< typename T > T &get( int idx, T &t ){
      readBuffer(idx, (uint8_t*)&t, sizeof(T));
      return t;
    }

    template< typename T > const T &put( int idx, const T &t ){
      writeBuffer(idx, (uint8_t*)&t, sizeof(T));
      return t;
    }

  private:
    int rawWriteBuffer(uint16_t address, const uint8_t* data, size_t len);
    int size;
    uint8_t i2cAddress;
    int writeDelay;
    TwoWire* twoWire;
};

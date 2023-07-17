
#include "Wire.h"

class AT24Cxxx {

  public:
    AT24Cxxx(uint8_t address, TwoWire& i2c, int writeDelay, uint16_t size, uint8_t pageSize);
    uint8_t read( int idx );
    void write( int idx, uint8_t val);
    void update( int idx, uint8_t val);
    uint16_t length();
    int writeBuffer(uint16_t address, const uint8_t* data, size_t len);
    int readBuffer(uint16_t address, uint8_t* data, uint8_t len);
    /** 
     * Returns result from the last performed operation. 
     * The meaning of the values are:
     *  0 .. success
     *  1 .. length to long for buffer
     *  2 .. address send, NACK received - typically means no device at the address
     *  3 .. data send, NACK received
     *  4 .. other twi error (lost bus arbitration, bus error, ..)
     */
    uint8_t getLastError();

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
    uint8_t writeDelay;
    uint8_t lastError;
    uint8_t pageSize;
    TwoWire* twoWire;
};

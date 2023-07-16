
#include "at24cxxx.h"
#include "Arduino.h"

#define PAGE_SIZE (64)

AT24Cxxx::AT24Cxxx(uint8_t address, int size, TwoWire& i2c = Wire, int writeDelay = 30)  :
  i2cAddress(address), size(size), twoWire(&i2c), writeDelay(writeDelay) {
}

uint8_t
AT24Cxxx::read( int idx ){
  uint8_t result;
  readBuffer(idx, &result, 1);
  return result;
}

void
AT24Cxxx::write( int idx, uint8_t val){
  uint8_t data = val;
  writeBuffer(idx, &data, 1);
}

void
AT24Cxxx::update( int idx, uint8_t val){
  if (val != read(idx)) {
    write(idx, val);
  }
}

uint16_t
AT24Cxxx::length() {
  return size;
}

int
AT24Cxxx::rawWriteBuffer(uint16_t address, const uint8_t* data, size_t len){
	twoWire->beginTransmission(i2cAddress);
	twoWire->write((uint8_t)((address >> 8) & 0xFF));
	twoWire->write((uint8_t)(address & 0xFF));
  int written = twoWire->write(data, len);
	twoWire->endTransmission();
  delay(writeDelay); // AT24C256 needs 5-20 ms time after write (tWR Write Cycle Time) to become available again
  return written;
}

int
AT24Cxxx::writeBuffer(uint16_t address, const uint8_t* data, size_t len){
  uint8_t* dataToWrite = data;
  size_t lenRemaining = len;
  uint16_t nextAddress = address;
  int written = 0;
  do {
    uint16_t locationOnPage = nextAddress % PAGE_SIZE;
    size_t maxBytesToWrite = PAGE_SIZE - locationOnPage;
    size_t bytesToWrite = min(maxBytesToWrite, lenRemaining);
    written += rawWriteBuffer(nextAddress, dataToWrite, bytesToWrite);
    lenRemaining -= bytesToWrite;
    dataToWrite += bytesToWrite;
    nextAddress += bytesToWrite;
  } while (lenRemaining > 0);
  return written;
}

int
AT24Cxxx::readBuffer(uint16_t address, uint8_t* data, uint8_t len){
	twoWire->beginTransmission(i2cAddress);
	twoWire->write((uint8_t)((address >> 8) & 0xFF));
	twoWire->write((uint8_t)(address & 0xFF));
	twoWire->endTransmission();
	twoWire->requestFrom(i2cAddress, len);
	int byteNumber;
	for(byteNumber = 0; (byteNumber < len) && twoWire->available(); byteNumber++){
		data[byteNumber] = twoWire->read();
	}
  return byteNumber;
}

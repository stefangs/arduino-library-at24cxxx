
#include "at24cxxx.h"
#include "Arduino.h"

AT24Cxxx::AT24Cxxx(uint8_t address, TwoWire& i2c, int writeDelay, uint16_t size, uint8_t pageSize)  :
  i2cAddress(address), size(size), twoWire(&i2c), writeDelay(writeDelay), pageSize(pageSize) {
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

 uint8_t 
 AT24Cxxx::getLastError() {
  return lastError;
 }

int
AT24Cxxx::rawWriteBuffer(uint16_t address, const uint8_t* data, size_t len){
  lastError = 0;
	twoWire->beginTransmission(i2cAddress);
	twoWire->write((uint8_t)((address >> 8) & 0xFF));
	twoWire->write((uint8_t)(address & 0xFF));
  for (int written = 0; written < len; written++) {
    // Not using twoWire's built in buffer write since it hides errors from write.
    if (twoWire->write(data[written]) != 1) {
      // An error here (not 1) indicates that we have reached the end of the 
      // internal write buffer, so no more data can be written.
      // It is no point writing parts of a buffer, so we leave here and indicate
      // the error
      lastError = 1;
      return 0;
    }
  }
	lastError = twoWire->endTransmission();
  // The AT24Cxxx chips needs 5-20 ms time after write (tWR Write Cycle Time) 
  // to become available again for new operations. 
  // It is possible to poll the chip to ask if it is ready, but this is hard to
  // do through the TwoWire-API, so instead we just do a hard wait to ensure
  // that the chip is availbale again before we finish the operation.
  delay(writeDelay); 
  return lastError == 0 ? len : 0;
}

int
AT24Cxxx::writeBuffer(uint16_t address, const uint8_t* data, size_t len){
  const uint8_t* dataToWrite = data;
  size_t lenRemaining = len;
  uint16_t nextAddress = address;
  int written = 0;
  do {
    // Since page write to the AT24Cxxx chips on works within the pages
    // we must make sure to split our writes on page borders.
    // Therefore we start by finding out how far it is to the next page
    // border and only write as many bytes in one write operation.
    uint16_t locationOnPage = nextAddress % pageSize;
    size_t maxBytesToWrite = pageSize - locationOnPage;
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
  lastError = 0;
	twoWire->beginTransmission(i2cAddress);
	twoWire->write((uint8_t)((address >> 8) & 0xFF));
	twoWire->write((uint8_t)(address & 0xFF));
	lastError = twoWire->endTransmission();
	twoWire->requestFrom(i2cAddress, len);
	int byteNumber;
	for(byteNumber = 0; (byteNumber < len) && twoWire->available(); byteNumber++){
		data[byteNumber] = twoWire->read();
	}
  return byteNumber;
}

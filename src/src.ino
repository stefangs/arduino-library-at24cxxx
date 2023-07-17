

#include "at24c256.h"

#define BASE (1024L * 30)

AT24C256 eprom(0x50);     // Assumes an eprom is connected on this address
AT24C256 badEprom(0x58);  // Assumes an eprom is NOT connected on this address
char buffer[10];

struct Point {
  int x;
  int y;
};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Starting up");

  // Write and read an integer
  int foo = 42, foo_in;
  eprom.put(BASE, foo);
  Serial.println(eprom.get(BASE, foo_in));

  // Write and read a double
  double pi = 3.141593, pi_in;
  eprom.put(BASE, pi);
  eprom.get(BASE, pi_in);
  dtostrf(pi_in, 9, 6, buffer);
  Serial.println(buffer);

  // Write and read a struct
  Point point = {17, 42}, point_in;
  eprom.put(BASE, point);
  eprom.get(BASE, point_in);
  Serial.println(point_in.x);
  Serial.println(point_in.y);

  // Write and read a single byte
  eprom.write(BASE, 77);
  Serial.println(eprom.read(BASE));

  // Write and read a byte buffer
  uint8_t out[15] = "Test of buffer", in[15];
  eprom.writeBuffer(BASE, out, 15);
  eprom.readBuffer(BASE, in, 15);
  Serial.println((char*)in);

   // Error handling on write, no error
  eprom.write(BASE, 77);
  Serial.print("last error on write: ");
  // 0 means no error
  Serial.println(badEprom.getLastError());

 // Error handling on write, using a eprom address without eprom
  badEprom.write(BASE, 77);
  Serial.print("last error on write: ");
  // 2 usually means that no device is attached to the address
  Serial.println(badEprom.getLastError());  

 // Error handling on read, using a eprom address without eprom
  Serial.println(badEprom.read(BASE));
  Serial.print("last error on read: ");
  Serial.println(badEprom.getLastError());  

  // Read the size of the eeprom
  Serial.println(eprom.length());

    // Write and read a long byte buffer (> 32 which is TwoWire's internal buffer size and longer than 64)
  uint8_t out2[80] = "Writing a really long message, testing some of several buffer limits on the way", in2[80];
  Serial.print(eprom.writeBuffer(BASE, out2, 80));
  Serial.print(" bytes written, last error on write: ");
  Serial.println(eprom.getLastError());  
  Serial.print(eprom.readBuffer(BASE, in2, 80));
  Serial.print(" bytes read, last error on read: ");
  Serial.println(eprom.getLastError()); 
  in2[79] = '\0';
  Serial.println((char*)in2);

  
}

void loop() {
}

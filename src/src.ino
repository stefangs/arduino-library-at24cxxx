

#include "at24cxxx.h"

#define BASE (1024L * 60)

AT24Cxxx eprom(0x50, 32768, Wire, 30);
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
  eprom.get(BASE, foo_in);
  Serial.println(foo);

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

  // Write and read byte buffers
  uint8_t out[15] = "Test of buffer", in[15];
  eprom.writeBuffer(BASE, out, 15);
  eprom.readBuffer(BASE, in, 15);
  Serial.println((char*)in);

  // Read the size of the eeprom
  Serial.println(eprom.length());
  
}

void loop() {
}

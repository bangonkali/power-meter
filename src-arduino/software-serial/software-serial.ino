#include <SoftwareSerial.h>

/** 
* Serial COM17 38400
* SoftwareSerial COM16 9600
*/

SoftwareSerial mySerial(2, 3); // RX, TX 
char buffer;

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(38400);
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for Leonardo only
  // }

  // Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  // mySerial.println("Hello, world?");
}

void loop() // run over and over
{

  if (mySerial.available()) {
    buffer = mySerial.read();
    Serial.write(buffer);
    mySerial.print(buffer);
  }
  if (Serial.available()) {
    buffer = Serial.read();
    mySerial.write(buffer);
    //Serial.write(buffer);
  }
}
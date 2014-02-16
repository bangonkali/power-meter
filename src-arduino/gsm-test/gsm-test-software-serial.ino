#include "SoftwareSerial.h"


/** 
* Serial COM17 38400
* SoftwareSerial COM16 9600
*/

SoftwareSerial mySerial(2,3); // RX, TX 

char buffer;
char buffer_serial;
bool debug = true; // global debug variable

void ReadMySerial() {
  while (mySerial.available()) {
    buffer = mySerial.read();

    if (debug) {
      Serial.write(buffer);
    }
    //mySerial.write(buffer);
  }
}


void GSMSend(char *msg, bool debug = true) {
  if (debug == true) {
    Serial.println(msg);
  }

  mySerial.println(msg);
}

void setup()  
{
  char at_cmgs_cmd[30] = {'\0'};
  char msg1[160] = {'\0'};
  char ctl_z = 0x1A;

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  delay(300);

  // Hangs the system until the debugger on RX/TX is connected.
  if (debug) {
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
  }

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(300);
  // mySerial.println("Hello, world?");

  sprintf(msg1, "%s%c", "hello world!", ctl_z);
  sprintf(at_cmgs_cmd, "AT+CMGS=\"%s\"\r\n","09332034751");

  GSMSend("AT\r\n");
  ReadMySerial(); // OK
  delay(300);

  GSMSend("AT+CMGF=1\r\n");
  ReadMySerial(); // OK
  delay(300);

  GSMSend(at_cmgs_cmd);
  ReadMySerial(); // OK
  delay(300);

  GSMSend(msg1);
  delay(500);
  ReadMySerial(); // OK

}

// void serialEvent() {
//   while (Serial.available()) {
//     // buffer_serial = Serial.read();
//     // mySerial.write(buffer_serial);
//     // Serial.write(buffer_serial);
//   }
// }

void loop() // run over and over
{
  if (debug) {
    if (mySerial.available()) {
      buffer = mySerial.read();
      Serial.write(buffer);
      //mySerial.write(buffer);
    }
  }
}


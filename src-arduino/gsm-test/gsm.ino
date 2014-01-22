/*
  GSM Send Sketch for Arduino
  
  Initializes GSM Module and sends an SMS to recipient
  
  The circuit:
  *Arduino pin 0 (RX) - GSM Module (TX)
  *Arduino pin 1 (TX) - GSM Module (RX)
  
  Created 2010
  by Meann Zabanal
  Modified 
  by John for GSM Shield testing
*/
#include <SoftwareSerial.h>

char Rx_data[50];
unsigned char Rx_index = 0;
int i = 0;
char msg[160];
int sig;

SoftwareSerial SoftSerial1(2, 3);

void setup() {
  Softserial1.begin(38400);
  
  initGSM();
  send_msg("214", "?1515");
}

void loop() {
  //none
}

void send_msg(char *number, char *msg)
{
  char at_cmgs_cmd[30] = {'\0'};
  char msg1[160] = {'\0'};
  char ctl_z = 0x1A;

  sprintf(msg1, "%s%c", msg, ctl_z);
  sprintf(at_cmgs_cmd, "AT+CMGS=\"%s\"\r\n",number);
  
  sendGSM(at_cmgs_cmd);
  delay(100);
  delay(100);
  delay(100);
  sendGSM(msg1);
  delay(100);
}

void sendGSM(char *string){
  Softserial1.write(string);
  delay(90);
}

void clearString(char *strArray) {
  int j;
  for (j = 100; j > 0; j--)
    strArray[j] = 0x00;
}

void send_cmd(char *at_cmd, char clr){
  char *stat = '\0';
  while(!stat){
    sendGSM(at_cmd);
    delay(90);
    readSoftserial1String(Rx_data);
    
    stat = strstr(Rx_data, "OK");
  }
  if (clr){
    clearString(Rx_data);
    delay(200);
    stat = '\0';
  }
}

void initGSM(){
  
  send_cmd("AT\r\n",1);						
//  send_cmd("ATE0\r\n",1); // Turn off automatic echo of the GSM Module	
	
  send_cmd("AT+CMGF=1\r\n",1);			// Set message format to text mode
  //Sucess
  
  Softserial1.println("Success");
	
  delay(1000);
  delay(1000);
  delay(1000);
}

void readSerialString (char *strArray) {
  
  if(!Softserial1.available()) {
    return;
  }
  
  while(Softserial1.available()) {
    strArray[i] = Softserial1.read();
    i++;
  }
}


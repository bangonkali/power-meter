#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "SoftwareSerial.h"
#include <Wire.h>
#include "RTClib.h"


/*****************************************************************
 * RTC Componentes
 ****************************************************************/
RTC_DS1307 RTC;

/*****************************************************************
 * LCD Componentes
 ****************************************************************/
#define METER_ID 0001
// LCD interface (should agree with the diagram above)
//   make sure that the LCD RW pin is connected to GND
#define lcd_D7_port     PORTD                   // lcd D7 connection
#define lcd_D7_bit      PORTD4
#define lcd_D7_ddr      DDRD

#define lcd_D6_port     PORTD                   // lcd D6 connection
#define lcd_D6_bit      PORTD5
#define lcd_D6_ddr      DDRD

#define lcd_D5_port     PORTD                   // lcd D5 connection
#define lcd_D5_bit      PORTD6
#define lcd_D5_ddr      DDRD

#define lcd_D4_port     PORTD                   // lcd D4 connection
#define lcd_D4_bit      PORTD7
#define lcd_D4_ddr      DDRD

#define lcd_E_port      PORTB                   // lcd Enable pin
#define lcd_E_bit       PORTB0
#define lcd_E_ddr       DDRB

#define lcd_RS_port     PORTB                   // lcd Register Select pin
#define lcd_RS_bit      PORTB1
#define lcd_RS_ddr      DDRB

// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2

// LCD instructions
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position

// Function Prototypes
void lcd_write_4(unsigned char);
void lcd_write_instruction_4d(unsigned char);
void lcd_write_character_4d(unsigned char);
void lcd_write_string_4d(char *);
void lcd_init_4d(void);
void LCD_goto(unsigned char y, unsigned char x);

unsigned char W = 0x80;
unsigned long offset = 65530;
unsigned long V = 0;
unsigned long I = 0;
unsigned long C = 0;
unsigned long s = 0;

/*****************************************************************
 * GSM Componentes
 ****************************************************************/

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

void GSMSendSMS(char *msg, char *number) {
	char at_cmgs_cmd[30] = {'\0'};
	char msg1[160] = {'\0'};
	char ctl_z = 0x1A;

	sprintf(msg1, "%s%c", msg, ctl_z);
	sprintf(at_cmgs_cmd, "AT+CMGS=\"%s\"\r\n", number);

	GSMSend("AT\r\n", debug);
	ReadMySerial(); // OK
	delay(300);

	GSMSend("AT+CMGF=1\r\n", debug);
	ReadMySerial(); // OK
	delay(300);

	GSMSend(at_cmgs_cmd, debug);
	ReadMySerial(); // OK
	delay(300);

	GSMSend(msg1, debug);
	delay(300);
	ReadMySerial(); // OK
}

/*****************************************************************
 * ADE 7753 Componentes
 ****************************************************************/
void ADE7753en()
{
  PORTB = (0<<PORTB2);
  _delay_us(10);
}
void ADE7753dis()
{
  PORTB = (1<<PORTB2);
  _delay_us(10);
}
unsigned long read3byte(unsigned char reg)
{
  unsigned long msbb = 0;
  unsigned long msb = 0;
  unsigned long lsb = 0;
  unsigned long data = 0;
  ADE7753en();
  SPDR = reg;
  while(!(SPSR&(1<<SPIF)));
  SPDR = 0x00; //dummy data
  while(!(SPSR&(1<<SPIF)));
  msbb = SPDR;
  SPDR = 0x00; //dummy data
  while(!(SPSR&(1<<SPIF)));
  msb = SPDR;
  SPDR = 0x00; //dummy data
  while(!(SPSR&(1<<SPIF)));
  lsb = SPDR;
  ADE7753dis();
  data = (msbb<<16)|(msb<<8)|(lsb);
  return data;
}
unsigned short read2byte(unsigned char reg)
{
  unsigned long msb = 0;
  unsigned long lsb = 0;
  unsigned long data = 0;
  ADE7753en();
  SPDR = reg;
  while(!(SPSR&(1<<SPIF)));
  SPDR = 0x00; //dummy data
  while(!(SPSR&(1<<SPIF)));
  msb = SPDR;
  SPDR = 0x00; //dummy data
  while(!(SPSR&(1<<SPIF)));
  lsb = SPDR;
  ADE7753dis();
  data = (msb<<8)|(lsb);
  return data;
}
unsigned char read1byte(unsigned char reg)
{
  unsigned char lsb = 0;
  unsigned long data = 0;
  ADE7753en();
  SPDR = reg;
  while(!(SPSR&(1<<SPIF)));
  SPDR = 0x00; //dummy data
  while(!(SPSR&(1<<SPIF)));
  lsb = SPDR;
  ADE7753dis();
  data = (lsb);
  return data;
}
void write3byte(unsigned char reg, unsigned char dat2, unsigned char dat1, unsigned char dat0)
{
  reg = W|reg;
  ADE7753en();
  SPDR = reg;
  while(!(SPSR&(1<<SPIF)));
  SPDR = dat2;
  while(!(SPSR&(1<<SPIF)));
  SPDR = dat1;
  while(!(SPSR&(1<<SPIF)));
  SPDR = dat0;
  while(!(SPSR&(1<<SPIF)));
  ADE7753dis();
}
void write2byte(unsigned char reg, unsigned char dat1, unsigned char dat0)
{
  reg = W|reg;
  ADE7753en();
  SPDR = reg;
  while(!(SPSR&(1<<SPIF)));
  SPDR = dat1;
  while(!(SPSR&(1<<SPIF)));
  SPDR = dat0;
  while(!(SPSR&(1<<SPIF)));
  ADE7753dis();
}
void write1byte(unsigned char reg, unsigned char dat0)
{
  reg = W|reg;
  ADE7753en();
  SPDR = reg;
  while(!(SPSR&(1<<SPIF)));
  SPDR = dat0;
  while(!(SPSR&(1<<SPIF)));
  ADE7753dis();
}
void setup()
{
	/*****************************************************************
	 * ADE 7753 SPI Initialization
	 ****************************************************************/

	Serial.begin(9600);
	DDRB = (1<<PORTB5)|(1<<PORTB3)|(1<<PORTB2);
	PORTB = (1<<PORTB2);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPOL)|(0<<CPHA);
	_delay_ms(100);
	write2byte(0x09,0x80,0x08);
	_delay_ms(100);
	write2byte(0x19,0x0,0x00);
	_delay_ms(100);
	write1byte(0x0E,0x00);
	_delay_ms(100);
	write1byte(0x0D,0x00);
	_delay_ms(100);
	write1byte(0x0F,0x82);
	// configure the microprocessor pins for the data lines
	lcd_D7_ddr |= (1<<lcd_D7_bit);                  // 4 data lines - output
	lcd_D6_ddr |= (1<<lcd_D6_bit);
	lcd_D5_ddr |= (1<<lcd_D5_bit);
	lcd_D4_ddr |= (1<<lcd_D4_bit);
	// configure the microprocessor pins for the control lines
	lcd_E_ddr |= (1<<lcd_E_bit);                    // E line - output
	lcd_RS_ddr |= (1<<lcd_RS_bit);                  // RS line - output

	// initialize the LCD controller as determined by the defines (LCD instructions)
	lcd_init_4d();
	_delay_ms(3000);

	/*****************************************************************
	 * GSM Initialization
	 ****************************************************************/
	mySerial.begin(9600);


	/*****************************************************************
	 * RTC Initialization
	 ****************************************************************/
  // Wire.begin();
  // RTC.begin();  
  // RTC.adjust(DateTime(__DATE__, __TIME__));

  // if (! RTC.isrunning()) {
  //   Serial.println("RTC is NOT running!");
  //   // following line sets the RTC to the date & time this sketch was compiled
  // }
}

void loop()
{

	/*****************************************************************
	 * ADE 7753 Get Data
	 ****************************************************************/
  char data[17];
	char data2[17];
	V = read3byte(0x25);
	I = read3byte(0x16); 
	C = read3byte(0x03);
	s = read3byte(0x06);
	unsigned long ApparentP = (0.572*s);
	sprintf(data,"I0%d,D99,P%07d;",METER_ID,ApparentP);

	/*****************************************************************
	 * Print Data on LCD
	 ****************************************************************/
	Serial.println(data);
	LCD_goto(1,1);
	lcd_write_string_4d(data);

	/*****************************************************************
	 * Print Time on LCD -- Commented out because it does not run.
	 ****************************************************************/
  // DateTime now = RTC.now();

  // // Serial.print(now.year(), DEC);
  // // Serial.print('/');
  // // Serial.print(now.month(), DEC);
  // // Serial.print('/');
  // // Serial.print(now.day(), DEC);
  // // Serial.print(' ');
  // // Serial.print(now.hour(), DEC);
  // // Serial.print(':');
  // // Serial.print(now.minute(), DEC);
  // // Serial.print(':');
  // // Serial.print(now.second(), DEC);
  // // Serial.println();

  // // Serial.println();

  // if (! RTC.isrunning()) {
  //   sprintf(data,"RTC NOT RUNNING");
  // } else {
  //   sprintf(data,"%d/%d/%d %d:%d:%d", now.month(), now.day(), now.year(), now.hour(), now.minute(),  now.second());
  // }
  
  // LCD_goto(2,1);
  // lcd_write_string_4d(data);
  // clear_data(data);

  /*****************************************************************
   * Check if time has passed 1 minute
   ****************************************************************/
  if (millis()/(unsigned long)1000 % (unsigned long)60 == (unsigned long)0) {
    sprintf(data2,"SENDING SMS...");
    LCD_goto(2,1);
    lcd_write_string_4d(data2);
    clear_data(data2);

    GSMSendSMS(data, "09157764387");

    sprintf(data2,"FINISHED SMS...");
    LCD_goto(2,1);
    lcd_write_string_4d(data2);
    clear_data(data2);

    delay(1000);
  } else {
    sprintf(data,"SEND SMS IN: %2d", (unsigned long)60 - (millis()/(unsigned long)1000 % (unsigned long)60));
    LCD_goto(2,1);
    lcd_write_string_4d(data);
    clear_data(data);
  }

  delay(1000);
}

void clear_data(char *data) {
	for (int a = 0; a < sizeof(data) - 1; a ++){
		data[a] = 0;
	}
}

void lcd_init_4d(void)
{
  // Power-up delay
  _delay_ms(100);                                 // initial 40 mSec delay


  // Set up the RS and E lines for the 'lcd_write_4' subroutine.
  lcd_RS_port &= ~(1<<lcd_RS_bit);                // select the Instruction Register (RS low)
  lcd_E_port &= ~(1<<lcd_E_bit);                  // make sure E is initially low

  // Reset the LCD controller
  lcd_write_4(lcd_FunctionReset);                 // first part of reset sequence
  _delay_ms(10);                                  // 4.1 mS delay (min)

  lcd_write_4(lcd_FunctionReset);                 // second part of reset sequence
  _delay_us(200);                                 // 100uS delay (min)

  lcd_write_4(lcd_FunctionReset);                 // third part of reset sequence
  _delay_us(200);                                 // this delay is omitted in the data sheet
  
  lcd_write_4(lcd_FunctionSet4bit);               // set 4-bit mode
  _delay_us(80);                                  // 40uS delay (min)

  // Function Set instruction
  lcd_write_instruction_4d(lcd_FunctionSet4bit);   // set mode, lines, and font
  _delay_us(80);                                  // 40uS delay (min)


  // Display On/Off Control instruction
  lcd_write_instruction_4d(lcd_DisplayOff);        // turn display OFF
  _delay_us(80);                                  // 40uS delay (min)

  // Clear Display instruction
  lcd_write_instruction_4d(lcd_Clear);             // clear display RAM
  _delay_ms(4);                                   // 1.64 mS delay (min)

  // ; Entry Mode Set instruction
  lcd_write_instruction_4d(lcd_EntryMode);         // set desired shift characteristics
  _delay_us(80);                                  // 40uS delay (min)


  // Display On/Off Control instruction
  lcd_write_instruction_4d(lcd_DisplayOn);         // turn the display ON
  _delay_us(80);                                  // 40uS delay (min)
}


        void lcd_write_string_4d(char theString[])
        {
  volatile int i = 0;                             // character counter*/
  while (theString[i] != 0)
  {
  lcd_write_character_4d(theString[i]);
  i++;
  _delay_us(80);                              // 40 uS delay (min)
  }
  }



  void lcd_write_character_4d(uint8_t theData)
  {
  lcd_RS_port |= (1<<lcd_RS_bit);                 // select the Data Register (RS high)
  lcd_E_port &= ~(1<<lcd_E_bit);                  // make sure E is initially low
  lcd_write_4(theData);                           // write the upper 4-bits of the data
  lcd_write_4(theData << 4);                      // write the lower 4-bits of the data
  }


  void lcd_write_instruction_4d(uint8_t theInstruction)
  {
  lcd_RS_port &= ~(1<<lcd_RS_bit);                // select the Instruction Register (RS low)
  lcd_E_port &= ~(1<<lcd_E_bit);                  // make sure E is initially low
  lcd_write_4(theInstruction);                    // write the upper 4-bits of the data
  lcd_write_4(theInstruction << 4);               // write the lower 4-bits of the data
  }


  void lcd_write_4(uint8_t theByte)
  {
  lcd_D7_port &= ~(1<<lcd_D7_bit);                        // assume that data is '0'
  if (theByte & 1<<7) lcd_D7_port |= (1<<lcd_D7_bit);     // make data = '1' if necessary

  lcd_D6_port &= ~(1<<lcd_D6_bit);                        // repeat for each data bit
  if (theByte & 1<<6) lcd_D6_port |= (1<<lcd_D6_bit);

  lcd_D5_port &= ~(1<<lcd_D5_bit);
  if (theByte & 1<<5) lcd_D5_port |= (1<<lcd_D5_bit);

  lcd_D4_port &= ~(1<<lcd_D4_bit);
  if (theByte & 1<<4) lcd_D4_port |= (1<<lcd_D4_bit);

  // write the data
  // 'Address set-up time' (40 nS)
  lcd_E_port |= (1<<lcd_E_bit);                   // Enable pin high
  _delay_us(1);                                   // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
  lcd_E_port &= ~(1<<lcd_E_bit);                  // Enable pin low
  _delay_us(1);                                   // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
  }

  void LCD_goto(unsigned char y, unsigned char x)
  {
  unsigned char firstAddress[] = {0x80,0xC0,0x94,0xD4};
  
  lcd_write_instruction_4d(firstAddress[y-1] + x-1);
  _delay_ms(10);
  }

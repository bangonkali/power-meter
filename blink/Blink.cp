#line 1 "C:/Users/Bangonkali/Desktop/Blink/Blink.c"
void main() {
 TRISA = 0;
 TRISB = 0;
 TRISC = 0;
 TRISD = 0;
 TRISE = 0;

 do {
 LATA = 0x00;
 LATB = 0x00;
 LATC = 0x00;
 LATD = 0x00;
 LATE = 0x00;
 Delay_ms(1000);

 LATA = 0xFF;
 LATB = 0xFF;
 LATC = 0xFF;
 LATD = 0xFF;
 LATE = 0xFF;
 Delay_ms(1000);
 } while(1);
}

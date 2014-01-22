
_main:

;Blink.c,1 :: 		void main() {
;Blink.c,2 :: 		TRISA = 0;           // set direction to be output
	CLRF        TRISA+0 
;Blink.c,3 :: 		TRISB = 0;           // set direction to be output
	CLRF        TRISB+0 
;Blink.c,4 :: 		TRISC = 0;           // set direction to be output
	CLRF        TRISC+0 
;Blink.c,5 :: 		TRISD = 0;           // set direction to be output
	CLRF        TRISD+0 
;Blink.c,6 :: 		TRISE = 0;           // set direction to be output
	CLRF        TRISE+0 
;Blink.c,8 :: 		do {
L_main0:
;Blink.c,9 :: 		LATA = 0x00;       // Turn OFF LEDs on PORTA
	CLRF        LATA+0 
;Blink.c,10 :: 		LATB = 0x00;       // Turn OFF LEDs on PORTB
	CLRF        LATB+0 
;Blink.c,11 :: 		LATC = 0x00;       // Turn OFF LEDs on PORTC
	CLRF        LATC+0 
;Blink.c,12 :: 		LATD = 0x00;       // Turn OFF LEDs on PORTD
	CLRF        LATD+0 
;Blink.c,13 :: 		LATE = 0x00;       // Turn OFF LEDs on PORTE
	CLRF        LATE+0 
;Blink.c,14 :: 		Delay_ms(1000);    // 1 second delay
	MOVLW       61
	MOVWF       R11, 0
	MOVLW       225
	MOVWF       R12, 0
	MOVLW       63
	MOVWF       R13, 0
L_main3:
	DECFSZ      R13, 1, 1
	BRA         L_main3
	DECFSZ      R12, 1, 1
	BRA         L_main3
	DECFSZ      R11, 1, 1
	BRA         L_main3
	NOP
	NOP
;Blink.c,16 :: 		LATA = 0xFF;       // Turn ON LEDs on PORTA
	MOVLW       255
	MOVWF       LATA+0 
;Blink.c,17 :: 		LATB = 0xFF;       // Turn ON LEDs on PORTB
	MOVLW       255
	MOVWF       LATB+0 
;Blink.c,18 :: 		LATC = 0xFF;       // Turn ON LEDs on PORTC
	MOVLW       255
	MOVWF       LATC+0 
;Blink.c,19 :: 		LATD = 0xFF;       // Turn ON LEDs on PORTD
	MOVLW       255
	MOVWF       LATD+0 
;Blink.c,20 :: 		LATE = 0xFF;       // Turn ON LEDs on PORTE
	MOVLW       255
	MOVWF       LATE+0 
;Blink.c,21 :: 		Delay_ms(1000);    // 1 second delay
	MOVLW       61
	MOVWF       R11, 0
	MOVLW       225
	MOVWF       R12, 0
	MOVLW       63
	MOVWF       R13, 0
L_main4:
	DECFSZ      R13, 1, 1
	BRA         L_main4
	DECFSZ      R12, 1, 1
	BRA         L_main4
	DECFSZ      R11, 1, 1
	BRA         L_main4
	NOP
	NOP
;Blink.c,22 :: 		} while(1);          // Endless loop
	GOTO        L_main0
;Blink.c,23 :: 		}
L_end_main:
	GOTO        $+0
; end of _main

#define _XTAL_FREQ 4000000                // set crystal oscillator to 20MHz.
#define TMR1PRESCALE 8                     // timer1 prescaler is 8.
#define OUT RB3                            // use the name OUT for RC2 pin.
#define S1 RB0
#define S2 RB1
#define S3 RB2
#define S4 RA1
#define S5 RA2
#define S6 RA3
#define E  RA4

#include <xc.h>
 
void tmr1isr ();
void setUpRegisters();
void getFreq();

// BEGIN CONFIG
#pragma config FOSC = HS   // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF  // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON  // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF   // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config CP = OFF    // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG

// variables and constants declarations
unsigned long CCPR = 0;                    // holds the value needed to be put in CCP's registers.
unsigned long freq = 0; 
char Tnum = 0;


unsigned short lookUp[33] = {250, 242, 234, 226, 218, 211, 203, 193, 186, 
                            180, 174, 168, 162, 157, 151, 146, 141, 136, 
                            132, 127, 123, 119, 115, 111, 107, 103, 100, 
                            95, 88, 82, 77, 72, 67};

// main function:
void main() {
    
    __delay_ms(2000);  
    setUpRegisters();                              // delay 2s.
    getFreq();                          
    
    CCPR = freq;
    
  
   while (1) {                                          // infinite loop.
        if (CCP1IF == 1) { 
             tmr1isr ();  
        }
        
        if (E == 1){
            getFreq();
            CCPR = freq;
        }
   }
}
// Set up register
void setUpRegisters(){
    
   TRISB = 0b11110111;         // port B3 is output.
   TRISA = 0b11111111;
   PORTB = 0;                  // init port b
   PORTA = 0;                  // init port A
   
   CMCON=0x07;                // Disable comparators
   
   T1CON = 0b00110000;        // timer1 uses prescaler value of 8 and it is off.
   TMR1H = 0;                 // timer1 registers have 0 (clear).
   TMR1L = 0;
 
   CCP1CON = 0x0b;            // set CCP module to compare mode and trigger special event when interrupt happens.
   CCPR = 0;                  // load 0 in CCPR.
   CCP1IF = 0;                // clear CCP1 interrupt flag.
   CCP1IE = 1;                // enable CCP1 interrupt.
   INTCON = 0xC0;             // enable global and peripheral interrupt.
   T1CON = 0b00110001;        // start timer1 with the same settings like before.

}
// interrupt service routine
void  tmr1isr () {
    
      if (OUT == 1) {                           // if the output was 1 -> was "on-time".        
           OUT = 0;                                  // set output to 0 in order to achieve "off-time".
           }                 
      else {                                    // if the output was 0 -> was "off-time".
         OUT = 1;                               // set output to 1 in order to achieve "on-time"
      }
        
      CCPR1H = CCPR >> 8;                       // right-shift CCPR by 8 then load it into CCPR1H register (load higher byte).
      CCPR1L = CCPR;                            // put the lower byte of CCPR in CCPR1L register.
      CCP1IF = 0;             
}
// Set up desired frequency 
void getFreq() {
    
    // Get input values 
    Tnum = S6;
    Tnum = Tnum << 1; 
    Tnum = Tnum + S5;
    Tnum = Tnum << 1;
    Tnum = Tnum + S4;
    Tnum = Tnum << 1;
    Tnum = Tnum + S3;
    Tnum = Tnum << 1;
    Tnum = Tnum + S2;
    Tnum = Tnum << 1;
    Tnum = Tnum + S1;
    
    // Index for the lookup table
    Tnum -= 30;
    
    freq = lookUp[Tnum];
    
    freq = (_XTAL_FREQ/freq)/(4*8); // N = (t * F_crystal) / (4 * prescaler)
    
    freq = freq * 0.50; // half duty cycle
    
}
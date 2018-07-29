/*
Displays the prime numbers when clicking a button.
V1:
    - Numbers go from 0 to 255. After that the counter resets.
    - Display is done with 8 LEDs.
V2:
    - LEDs were changed for a 4-digit 7-segment display.
    - Numbers go from 0 to 9999 (Max). After that the counter resets.
    - the last number displayed is stored in EEPROM.

Hardware connections (using the hapkit board)
    - A 4-digit 7-segment display (D2-D13)
    - A push button connected to port A0.
    - A buzzer connected to port A1.

The primes[] array is stored in numbers.h.
The file numbers.h contains a lot of numbers, and to generate the numbers, the python script "numbersGenerator.py" is used.
*/

#include "numbers.h"
#include <EEPROM.h>

#define OFF 10

/* Hardware ports */
int button = 14; // A0
int buzzer = 15; // A1
int d4 = 2;
int G  = 3;
int C  = 4;
int dp = 5;
int D  = 6;
int E  = 7;
int B  = 8;
int d3 = 9;
int d2 = 10;
int F  = 11;
int A  = 12;
int d1 = 13;

/* Global variables */
unsigned int counter = 0;
unsigned int current_number = 0;
int buttonState = 0;
int pressState = 0; // 0 = up, 1 = down (pressed)
// eeprom addresses
int current_address = 0; 
int counter_address = 2;

unsigned int segments[] = {
    // 0xABCDEFG
    0b1111110, // 0
    0b0110000, // 1
    0b1101101, // 2
    0b1111001, // 3
    0b0110011, // 4
    0b1011011, // 5
    0b1011111, // 6
    0b1110000, // 7
    0b1111111, // 8
    0b1110011 // 9
    };

/* function prototypes */
void num2disp(unsigned int);
void dispSegments(unsigned int);
// stores an unsigned long number in the eeprom in the specified address
void store2byteInt(unsigned int, int);
// reads an unsigned long number in the eeprom in the specified address.
unsigned int read2byteInt(int);

void setup() {
    /* Buzzer */
    pinMode(buzzer, OUTPUT);
    /* Button */
    pinMode(button, INPUT);

    /* 4-digit 7-segment display */
    pinMode(d4, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(dp, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(d3, OUTPUT);
    pinMode(d2, OUTPUT);
    pinMode(F, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(d1, OUTPUT);


    /*
    // write a 0 to the eeprom addresses (for reset).
    store2byteInt(0,current_address);
    store2byteInt(0,counter_address);
    */

    // restore the last number written in the EEPROM
    current_number = read2byteInt(current_address);
    counter = read2byteInt(counter_address);
}


void loop() {
    // display current number
    num2disp(current_number);
    // read the state of the pushbutton value:
    buttonState = digitalRead(button);

    // button is pressed
  if (buttonState == HIGH) {
    if (pressState == 0) {
        pressState = 1;
        /* turn on the LEDs to the current number
        and increment the counter */
        // pass the next element pointed to by counter in numbers
        if (counter >= sizeof(primes) / sizeof(primes[0])) {
            counter = 0;
            current_number = 0;
        }
        // activate numbers one to one until reaching the current prime
        while(current_number < primes[counter]) {
            digitalWrite(buzzer, HIGH);
            // execute 8 times
            for (int i=0; i<4; i++) {
                num2disp(current_number);
            }
            digitalWrite(buzzer, LOW);
            for (int i=0; i<4; i++) {
                num2disp(current_number);
            }
            current_number++;
        }
        counter++;
        // store the number and counter in eeprom
        store2byteInt(current_number, current_address);
        store2byteInt(counter, counter_address);

        /* wait for debounce */
        delay(10);
    }
  } else {
        if (pressState == 1) {
            pressState = 0;
            // turn LED off:
            digitalWrite(buzzer, LOW);
            /* wait for debounce */
            delay(10);
        }
  }
}

void num2disp(unsigned int n) {

    // divide the number by 10 to get the 1's
    unsigned int ones = n % 10;
    n = n/10;
    unsigned int tens = n % 10;
    n = n/10;
    unsigned int hunds = n % 10;
    n = n/10;
    unsigned int thnds = n % 10;

    // first digit
    digitalWrite(d1, LOW);
    // display digit
    dispSegments(thnds);
    delay(2);
    // turn off segments
    dispSegments(OFF);
    // turn cathode off
    digitalWrite(d1, HIGH);

    // second digit
    digitalWrite(d2, LOW);
    // display digit
    dispSegments(hunds);
    delay(2);
    // turn off segments
    dispSegments(OFF);
    // turn cathode off
    digitalWrite(d2, HIGH);

    // third digit
    digitalWrite(d3, LOW);
    // display digit
    dispSegments(tens);
    delay(2);
    // turn off segments
    dispSegments(OFF);
    // turn cathode off
    digitalWrite(d3, HIGH);

    // fourth digit
    digitalWrite(d4, LOW);
    // display digit
    dispSegments(ones);
    delay(2);
    // turn off segments
    dispSegments(OFF);
    // turn cathode off
    digitalWrite(d4, HIGH);
}

void dispSegments(unsigned int n) {
    if (n == OFF) {
        digitalWrite(A, LOW);
        digitalWrite(B, LOW);
        digitalWrite(C, LOW);
        digitalWrite(D, LOW);
        digitalWrite(E, LOW);
        digitalWrite(F, LOW);
        digitalWrite(G, LOW);
        return;
    }

   if (0b1000000 & segments[n]) { digitalWrite(A, HIGH); }
   else { digitalWrite(A, LOW); }
   if (0b0100000 & segments[n]) { digitalWrite(B, HIGH); }
   else { digitalWrite(B, LOW); }
   if (0b0010000 & segments[n]) { digitalWrite(C, HIGH); }
   else { digitalWrite(C, LOW); }
   if (0b0001000 & segments[n]) { digitalWrite(D, HIGH); }
   else { digitalWrite(D, LOW); }
   if (0b0000100 & segments[n]) { digitalWrite(E, HIGH); }
   else { digitalWrite(E, LOW); }
   if (0b0000010 & segments[n]) { digitalWrite(F, HIGH); }
   else { digitalWrite(F, LOW); }
   if (0b0000001 & segments[n]) { digitalWrite(G, HIGH); }
   else { digitalWrite(G, LOW); }
}


void store2byteInt(unsigned int n, int address) {
    // get the two bytes of n
    byte two = (n & 0xFF);
    byte one = ((n >> 8) & 0xFF);
    // write in EEPROM
    EEPROM.write(address, two);
    EEPROM.write(address+1, one);
}


unsigned int read2byteInt(int address) {
    // read the two bytes of n
    int  two = EEPROM.read(address);
    int one = EEPROM.read(address+1);

    // compose the original number
    return ((two << 0)&0xFF)+((one << 8)&0xFF);
}

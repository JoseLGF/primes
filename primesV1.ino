/*
Displays the prime numbers when clicking a button.
V1:
    - Numbers go from 0 to 255. After that the counter resets.
    - Display is done with 8 LEDs.

Hardware connections (using the hapkit board)
    - 8 LEDs connected to ports D2-D9.
    - A push button connected to port D10.
    - A buzzer connected to port D11.

The primes[] array is stored in numbers.h.
The file numbers.h contains a lot of numbers, and to generate the numbers, the python script "numbersGenerator.py" is used.
*/

#include "numbers.h"

/* Hardware ports */
int leds[] = {2, 3, 4, 5, 6, 7, 8, 9};
int l1 = 2;
int l2 = 3;
int l3 = 4;
int l4 = 5;
int l5 = 6;
int l6 = 7;
int l7 = 8;
int l8 = 9;
int button = 10;
int buzzer = 11;

/* Program variables */
unsigned int counter = 0;
unsigned int current_number = 0;
int buttonState = 0;
int pressState = 0; // 0 = up, 1 = down (pressed)

/* function prototypes */
void test(void);
void num2leds(unsigned int);

void setup() {
    /* 8 LEDs form the number (0-255) */
    pinMode(l1, OUTPUT);
    pinMode(l2, OUTPUT);
    pinMode(l3, OUTPUT);
    pinMode(l4, OUTPUT);
    pinMode(l5, OUTPUT);
    pinMode(l6, OUTPUT);
    pinMode(l7, OUTPUT);
    pinMode(l8, OUTPUT);
    /* Buzzer */
    pinMode(buzzer, OUTPUT);

    pinMode(button, INPUT);

    test();
}


void loop() {
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
        while(current_number <= primes[counter]) {
            num2leds(current_number);
            digitalWrite(buzzer, HIGH);
            delay(75);
            digitalWrite(buzzer, LOW);
            delay(75);
            current_number++;
        }
        counter++;
        /* wait for debounce */
        delay(100);
    }
  } else {
        if (pressState == 1) {
            pressState = 0;
            // turn LED off:
            digitalWrite(buzzer, LOW);
            /* wait for debounce */
            delay(100);
        }
  }
}

/* Sequence for testing LEDs */
void test() {
    digitalWrite(l1, HIGH);
    delay(250);
    digitalWrite(l1, LOW);
    digitalWrite(l2, HIGH);
    delay(250);
    digitalWrite(l2, LOW);
    digitalWrite(l3, HIGH);
    delay(250);
    digitalWrite(l3, LOW);
    digitalWrite(l4, HIGH);
    delay(250);
    digitalWrite(l4, LOW);
    digitalWrite(l5, HIGH);
    delay(250);
    digitalWrite(l5, LOW);
    digitalWrite(l6, HIGH);
    delay(250);
    digitalWrite(l6, LOW);
    digitalWrite(l7, HIGH);
    delay(250);
    digitalWrite(l7, LOW);
    digitalWrite(l8, HIGH);
    delay(250);
    digitalWrite(l8, LOW);
}

void num2leds(unsigned int n) {
   // divide the number by 2 until remainder is 0
   for (int i=0; i<8; i++) {
        if (n%2 == 1) {
            digitalWrite(leds[i], HIGH);
        }
        else {
            digitalWrite(leds[i], LOW);
        }
        n = n/2;
   }
}

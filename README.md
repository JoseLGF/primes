# primes
Counts prime numbers in a microcontroller.

# V1.0 
  - Uses 8 LEDs to display the number.
  - Counter goes from 0 to 255. After that the counter resets.
# V2:
   - LEDs were changed for a 4-digit 7-segment display.
   - Numbers go from 0 to 7000 (Max). After that the counter resets.
   - the last number displayed is stored in EEPROM.
   - The numbers from the EEPROM are retrieved when the uC is powered on.
## Hardware connections (using the hapkit board) for V2
   - A 4-digit 7-segment display (D2-D13)
   - A push button connected to port A0.
   - A buzzer connected to port A1.

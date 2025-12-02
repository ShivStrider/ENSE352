#include "stm32f103rb.h"

// guess[4]: Stores the 4 hexadecimal digits (0-15) entered by the user.
int guess[4];

// FUNCTION PROTOTYPES
void setup_gpio(void);   // Configures pins
void delay(int count);   // Creates a time delay
void set_leds(int val);  // Controls the 4 LEDs based on a value (0-15)
int read_switches(void); // Reads the DIP switches and returns the value

int main(void) {
  // Configure the Hardware
  //  We call this once at startup to prepare the pins.
  setup_gpio();

  // Main Game Loop
  //  This loop runs forever.
  while (1) {
    int current_digit = 0;

    // Loop until the user has entered all 4 digits for their guess.
    while (current_digit < 4) {
      // Update LEDs to prompt the user
      // We show a binary pattern to indicate which digit we are waiting for.
      if (current_digit == 0)
        set_leds(8); // 1000 (Digit 1)
      if (current_digit == 1)
        set_leds(12); // 1100 (Digit 2)
      if (current_digit == 2)
        set_leds(14); // 1110 (Digit 3)
      if (current_digit == 3)
        set_leds(15); // 1111 (Digit 4)

      // Check for Button Press
      // The User Button is on PC13. It is Active Low (0 when pressed).
      // We check if the 13th bit of the Input Data Register is 0.
      if ((GPIOC_IDR & (1 << 13)) == 0) {
        // Button was pressed!

        // Read the DIP Switches
        // We capture the current state of the switches.
        guess[current_digit] = read_switches();

        // Advance to the next digit
        current_digit++;

        // E. Debounce (Prevent multiple reads)
        // Wait for the user to RELEASE the button before continuing.
        delay(200000); // Short delay to handle mechanical bounce
        while ((GPIOC_IDR & (1 << 13)) == 0) {
          // Do nothing, just wait for release
        }
        delay(200000); // Short delay after release
      }
    }

    // Guess Complete Sequence
    // All 4 digits have been recorded.

    // Turn off LEDs briefly
    set_leds(0);
    delay(500000);

    // Flash LEDs twice to confirm success (as per requirements)
    set_leds(15); // All ON
    delay(500000);
    set_leds(0); // All OFF
    delay(500000);
    set_leds(15); // All ON
    delay(500000);
    set_leds(0); // All OFF

    // Wait for Reset
    // Wait for the user to press the button again to start a new round.
    while ((GPIOC_IDR & (1 << 13)) != 0) {
      // Wait for press (Input is High/1 while not pressed)
    }
    // Wait for release
    while ((GPIOC_IDR & (1 << 13)) == 0) {
    }
    delay(500000);
  }
}

// Controls the 4 external LEDs based on a 4-bit value.
// parameter: val: A number from 0-15.
// Bit 0 controls PB0
// Bit 1 controls PA4
// Bit 2 controls PA1
// Bit 3 controls PA0

void set_leds(int val) {
  // LED 0 (Connected to PB0)
  // We check the 1st bit of 'val'. If 1, set pin HIGH. If 0, set pin LOW.
  if (val & 1)
    GPIOB_ODR |= (1 << 0);
  else
    GPIOB_ODR &= ~(1 << 0);

  // LED 1 (Connected to PA4)
  if (val & 2)
    GPIOA_ODR |= (1 << 4);
  else
    GPIOA_ODR &= ~(1 << 4);

  // LED 2 (Connected to PA1)
  if (val & 4)
    GPIOA_ODR |= (1 << 1);
  else
    GPIOA_ODR &= ~(1 << 1);

  // LED 3 (Connected to PA0)
  if (val & 8)
    GPIOA_ODR |= (1 << 0);
  else
    GPIOA_ODR &= ~(1 << 0);
}

// Reads the state of the 4 DIP switches.
// return: The integer value (0-15) represented by the switches.

int read_switches(void) {
  int switch_value = 0;

  // The switches use pull-up resistors, so they read 0 (LOW) when ON.

  // Switch 1 (PB4) - Corresponds to Value 1
  if ((GPIOB_IDR & (1 << 4)) == 0) {
    switch_value = switch_value + 1;
  }

  // Switch 2 (PB6) - Corresponds to Value 2
  if ((GPIOB_IDR & (1 << 6)) == 0) {
    switch_value = switch_value + 2;
  }

  // Switch 3 (PB8) - Corresponds to Value 4
  if ((GPIOB_IDR & (1 << 8)) == 0) {
    switch_value = switch_value + 4;
  }

  // Switch 4 (PB9) - Corresponds to Value 8
  if ((GPIOB_IDR & (1 << 9)) == 0) {
    switch_value = switch_value + 8;
  }

  return switch_value;
}

// Configures the GPIO pins for the project.
// Enable Clocks for Port A, B, C
// Set LEDs as Outputs (Push-Pull)
// Set Switches/Button as Inputs (Floating)

void setup_gpio(void) {
  // Enable Peripheral Clocks
  // We write to APB2ENR to turn on the "power" for the ports.
  // Bit 2=GPIOA, Bit 3=GPIOB, Bit 4=GPIOC
  RCC_APB2ENR |= (1 << 2) | (1 << 3) | (1 << 4);

  // Configure LEDs (Outputs)
  // We want Output Push-Pull, 50MHz. The code is 0x3 (Binary 0011).

  // PA0 (LED 3)
  GPIOA_CRL &= ~(0xF << 0); // Clear current settings
  GPIOA_CRL |= (0x3 << 0);  // Set to Output

  // PA1 (LED 2)
  GPIOA_CRL &= ~(0xF << 4);
  GPIOA_CRL |= (0x3 << 4);

  // PA4 (LED 1)
  GPIOA_CRL &= ~(0xF << 16);
  GPIOA_CRL |= (0x3 << 16);

  // PB0 (LED 0)
  GPIOB_CRL &= ~(0xF << 0);
  GPIOB_CRL |= (0x3 << 0);

  // Configure Switches (Inputs)
  // We want Floating Input mode. The code is 0x4 (Binary 0100).

  // PB4 (Switch 1)
  GPIOB_CRL &= ~(0xF << 16);
  GPIOB_CRL |= (0x4 << 16);

  // PB6 (Switch 2)
  GPIOB_CRL &= ~(0xF << 24);
  GPIOB_CRL |= (0x4 << 24);

  // PB8 (Switch 3) - Note: Pins 8-15 are in CRH register
  GPIOB_CRH &= ~(0xF << 0);
  GPIOB_CRH |= (0x4 << 0);

  // PB9 (Switch 4)
  GPIOB_CRH &= ~(0xF << 4);
  GPIOB_CRH |= (0x4 << 4);

  // Configure Button (Input)
  // PC13 (User Button)
  GPIOC_CRH &= ~(0xF << 20); // Clear settings for Pin 13
  GPIOC_CRH |= (0x4 << 20);  // Set to Floating Input
}

// Simple busy-wait delay.
// Parameter: count: How long to wait.

void delay(int count) {
  int i = 0;
  while (i < count) {
    i++;
  }
}

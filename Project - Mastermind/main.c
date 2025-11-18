#include "stm32f103rb.h"

// Global array to hold the user's guess
int guess[4];

// --- Simple Function Prototypes ---
void setup_gpio(void);
void delay(int count);
void set_leds(int val);

int main(void)
{
    // 1. Setup the hardware
    setup_gpio();

    // Main Game Loop
    while(1)
    {
        int current_digit = 0;

        // Loop until we have all 4 digits
        while(current_digit < 4)
        {
            // Show which digit we are on using LEDs
            // Digit 1 = 1000, Digit 2 = 1100, etc.
            if(current_digit == 0) set_leds(8); // 1000 in binary is 8
            if(current_digit == 1) set_leds(12); // 1100 in binary is 12
            if(current_digit == 2) set_leds(14); // 1110 in binary is 14
            if(current_digit == 3) set_leds(15); // 1111 in binary is 15

            // Check if the BLUE BUTTON is pressed
            // The button is on PC13. It reads 0 when pressed.
            // We check bit 13.
            if( (GPIOC_IDR & (1<<13)) == 0 )
            {
                // Button was pressed!

                // 1. Read the DIP Switches
                int switch_value = 0;

                // Check Switch 1 (PB4)
                // If PB4 is 0 (LOW), the switch is ON.
                if( (GPIOB_IDR & (1<<4)) == 0 )
                {
                    switch_value = switch_value + 1;
                }

                // Check Switch 2 (PB6)
                if( (GPIOB_IDR & (1<<6)) == 0 )
                {
                    switch_value = switch_value + 2;
                }

                // Check Switch 3 (PB8)
                if( (GPIOB_IDR & (1<<8)) == 0 )
                {
                    switch_value = switch_value + 4;
                }

                // Check Switch 4 (PB9)
                if( (GPIOB_IDR & (1<<9)) == 0 )
                {
                    switch_value = switch_value + 8;
                }

                // Save the value
                guess[current_digit] = switch_value;

                // Move to the next digit
                current_digit++;

                // Wait for button release so we don't double-count
                delay(200000); // debounce
                while( (GPIOC_IDR & (1<<13)) == 0 )
                {
                    // Do nothing, just wait for release
                }
                delay(200000); // debounce
            }
        }

        // All 4 digits entered!
        // Turn off LEDs
        set_leds(0);
        delay(500000);

        // Flash LEDs twice to show we are done
        set_leds(15); // All ON
        delay(500000);
        set_leds(0);  // All OFF
        delay(500000);
        set_leds(15); // All ON
        delay(500000);
        set_leds(0);  // All OFF

        // Wait for button press to restart
        while( (GPIOC_IDR & (1<<13)) != 0 )
        {
            // Wait for press
        }
        // Wait for release
        while( (GPIOC_IDR & (1<<13)) == 0 ) {}
        delay(500000);
    }
}

// --- Helper Functions ---

// This function turns specific LEDs on or off
// val is a number like 0-15.
// Bit 0 = PB0 (LED 0)
// Bit 1 = PA4 (LED 1)
// Bit 2 = PA1 (LED 2)
// Bit 3 = PA0 (LED 3)
void set_leds(int val)
{
    // LED 0 (PB0)
    if(val & 1) GPIOB_ODR |= (1<<0);  // Turn ON
    else        GPIOB_ODR &= ~(1<<0); // Turn OFF

    // LED 1 (PA4)
    if(val & 2) GPIOA_ODR |= (1<<4);
    else        GPIOA_ODR &= ~(1<<4);

    // LED 2 (PA1)
    if(val & 4) GPIOA_ODR |= (1<<1);
    else        GPIOA_ODR &= ~(1<<1);

    // LED 3 (PA0)
    if(val & 8) GPIOA_ODR |= (1<<0);
    else        GPIOA_ODR &= ~(1<<0);
}

void setup_gpio(void)
{
    // 1. Turn on Clocks for Port A, B, and C
    // Bit 2 = Port A, Bit 3 = Port B, Bit 4 = Port C
    RCC_APB2ENR |= (1<<2) | (1<<3) | (1<<4);

    // 2. Configure LEDs as Outputs (50MHz Push-Pull)
    // We use 0x3 for output mode.

    // PA0 (LED 3)
    GPIOA_CRL &= ~(0xF << 0); // Clear old setting
    GPIOA_CRL |= (0x3 << 0);  // Set new setting

    // PA1 (LED 2)
    GPIOA_CRL &= ~(0xF << 4);
    GPIOA_CRL |= (0x3 << 4);

    // PA4 (LED 1)
    GPIOA_CRL &= ~(0xF << 16);
    GPIOA_CRL |= (0x3 << 16);

    // PB0 (LED 0)
    GPIOB_CRL &= ~(0xF << 0);
    GPIOB_CRL |= (0x3 << 0);

    // 3. Configure Switches as Inputs
    // We use 0x4 for Floating Input.

    // PB4 (Switch 1)
    GPIOB_CRL &= ~(0xF << 16);
    GPIOB_CRL |= (0x4 << 16);

    // PB6 (Switch 2)
    GPIOB_CRL &= ~(0xF << 24);
    GPIOB_CRL |= (0x4 << 24);

    // PB8 (Switch 3) - Note: CRH is for pins 8-15
    GPIOB_CRH &= ~(0xF << 0);
    GPIOB_CRH |= (0x4 << 0);

    // PB9 (Switch 4)
    GPIOB_CRH &= ~(0xF << 4);
    GPIOB_CRH |= (0x4 << 4);

    // 4. Configure Blue Button (PC13)
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |= (0x4 << 20);
}

void delay(int count)
{
    // Just loop to waste time
    int i = 0;
    while(i < count)
    {
        i++;
    }
}
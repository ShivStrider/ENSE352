// main.c 
// ENSE352 
// DATE: December 2, 2025
// AUTHOR: Shivam Soni (200474721) 
// DESCRIPTION: This program implements the Mastermind code-breaking game on the
// STM32F103RB Nucleo board. The player tries to guess a 4-digit 
// hex code within 8 attempts.
// 
// Hardware Used:
//   - 4 LEDs on PA0, PA1, PA4, PB0
//   - 4 DIP Switches on PB4, PB6, PB8, PB9  
//   - Blue User Button on PC13

#include "main.h"

// Global Variables
int secret_code[4];   // The 4-digit code the player needs to guess
int guess[4];         // Stores the player's current guess
int guess_count;      // How many guesses the player has made
unsigned int random_seed = 12345;  // Seed for random number generation

// Main Function
int main(void) {
    // Set up all the GPIO pins first
    setup_gpio();
    
    // Game runs forever
    while (1) {
        // Wait for player to press button to start
        wait_for_button_press();
        wait_for_button_release();
        delay(300000);
        
        // Play one round of the game
        play_game();
    }
}

// Game Functions

// Main game loop - runs one complete game
void play_game(void) {
    int exact, wrong_pos;
    
    // Reset for new game
    guess_count = 0;
    generate_secret_code();
    
    // Keep playing until player wins or runs out of guesses
    while (guess_count < MAX_GUESSES) {
        
        // Get the player's guess (4 hex digits)
        get_player_guess();
        guess_count++;
        
        // Check how good the guess was
        calculate_feedback(&exact, &wrong_pos);
        
        // If all 4 digits are correct, player wins!
        if (exact == 4) {
            win_sequence();
            return;
        }
        
        // Show the feedback on LEDs
        display_feedback(exact, wrong_pos);
        
        // Wait for player to press button for next guess
        wait_for_button_press();
        wait_for_button_release();
        delay(300000);
    }
    
    // If we get here, player used all 8 guesses - they lose
    lose_sequence();
}

// Generate a random 4-digit secret code
// Each digit is 0-15 (hex 0-F)
void generate_secret_code(void) {
#if DEBUG_MODE
    // Hardcoded for testing - guess 1,2,3,4 to win
    secret_code[0] = 1;
    secret_code[1] = 2;
    secret_code[2] = 3;
    secret_code[3] = 4;
#else
    // Generate random code
    for (int i = 0; i < 4; i++) {
        secret_code[i] = get_random() % 16;
    }
#endif
}

// Get 4 hex digits from the player using DIP switches
// LEDs show which digit we're waiting for:
//   Digit 1: 1000 (LED 3 on)
//   Digit 2: 1100 (LEDs 3,2 on)
//   Digit 3: 1110 (LEDs 3,2,1 on)
//   Digit 4: 1111 (all LEDs on)
void get_player_guess(void) {
    int current_digit = 0;
    
    while (current_digit < 4) {
        
        // Show which digit we're on using LEDs
        if (current_digit == 0) set_leds(8);   // 1000 in binary
        if (current_digit == 1) set_leds(12);  // 1100 in binary
        if (current_digit == 2) set_leds(14);  // 1110 in binary
        if (current_digit == 3) set_leds(15);  // 1111 in binary
        
        // Wait for button press to confirm the digit
        if (is_button_pressed()) {
            
            // Read what the switches are set to
            guess[current_digit] = read_switches();
            current_digit++;
            
            // Debounce the BUTTON (not the DIP switches)
            // Push buttons bounce when pressed, causing multiple false reads
            // DIP switches don't bounce since they stay in position
            delay(200000);
            wait_for_button_release();
            delay(200000);
        }
    }
    
    // Turn off LEDs briefly before showing feedback
    set_leds(0);
    delay(300000);
}

// Calculate feedback for the guess using Mastermind rules:
//   - exact: digit is correct AND in the right position
//   - wrong_pos: digit exists in code but wrong position
// 
// We use two passes to avoid counting the same digit twice
void calculate_feedback(int *exact, int *wrong_pos) {
    
    // These track which positions we've already matched
    // So we don't count the same digit twice
    int code_used[4] = {0, 0, 0, 0};
    int guess_used[4] = {0, 0, 0, 0};
    
    *exact = 0;
    *wrong_pos = 0;
    
    // FIRST PASS: Find exact matches (right digit, right position)
    for (int i = 0; i < 4; i++) {
        if (guess[i] == secret_code[i]) {
            (*exact)++;
            code_used[i] = 1;   // Mark as used
            guess_used[i] = 1;
        }
    }
    
    // SECOND PASS: Find wrong position matches
    // Only look at digits that weren't exact matches
    for (int i = 0; i < 4; i++) {
        // Skip if this guess digit was already an exact match
        if (guess_used[i]) continue;
        
        // Check against all code digits
        for (int j = 0; j < 4; j++) {
            // Skip if this code digit was already matched
            if (code_used[j]) continue;
            
            // Found a match in wrong position
            if (guess[i] == secret_code[j]) {
                (*wrong_pos)++;
                code_used[j] = 1;  // Mark as used so we don't match again
                break;  // Move to next guess digit
            }
        }
    }
}

// Display feedback on LEDs:
//   - Solid LEDs = exact matches
//   - Flashing LEDs = wrong position matches
// Example: 2 exact + 1 wrong_pos = 2 solid + 1 flashing
void display_feedback(int exact, int wrong_pos) {
    int solid_leds = 0;
    int flash_leds = 0;
    
    // Build the solid LED pattern
    // If exact=2, we want 0011 (2 LEDs on)
    for (int i = 0; i < exact; i++) {
        solid_leds |= (1 << i);
    }
    
    // Build the flashing LED pattern (starts after solid LEDs)
    // If exact=2 and wrong_pos=1, we want bit 2 to flash
    for (int i = 0; i < wrong_pos; i++) {
        flash_leds |= (1 << (exact + i));
    }
    
    // Show the feedback - flash 8 times so player can see it
    for (int cycle = 0; cycle < 8; cycle++) {
        // All feedback LEDs on (solid + flashing)
        set_leds(solid_leds | flash_leds);
        delay(200000);
        
        // Only solid LEDs on (flashing ones turn off)
        set_leds(solid_leds);
        delay(200000);
    }
    
    // Keep solid LEDs on until player presses button
    set_leds(solid_leds);
}

// Win sequence - player guessed correctly!
// 1. Blink all 4 LEDs on/off 4 times
// 2. Show how many guesses it took (in binary)
// 3. Wait for button to start new game
void win_sequence(void) {
    // Blink all LEDs 4 times
    for (int i = 0; i < 4; i++) {
        set_leds(15);  // 1111 = all on
        delay(400000);
        set_leds(0);   // all off
        delay(400000);
    }
    
    // Show guess count in binary
    // guess_count is 1-8, which fits in 4 bits
    set_leds(guess_count);
    
    // Wait for button to start new game
    wait_for_button_press();
    wait_for_button_release();
    delay(300000);
    set_leds(0);
}

// Lose sequence - player ran out of guesses
// Show alternating pattern 4 times then wait for button
void lose_sequence(void) {
    // Blink alternating pattern 4 times
    // 1010 and 0101 look different from the win pattern
    for (int i = 0; i < 4; i++) {
        set_leds(10);  // 1010
        delay(300000);
        set_leds(5);   // 0101
        delay(300000);
    }
    set_leds(0);
    
    // Wait for button to start new game
    wait_for_button_press();
    wait_for_button_release();
    delay(300000);
    set_leds(0);
}

// LED Functions

// Set the 4 LEDs based on a 4-bit value (0-15)
// Bit 0 (value 1) -> LED on PB0 (Arduino A3)
// Bit 1 (value 2) -> LED on PA4 (Arduino A2)
// Bit 2 (value 4) -> LED on PA1 (Arduino A1)
// Bit 3 (value 8) -> LED on PA0 (Arduino A0)
// Example: set_leds(5) = 0101 = LEDs on PA1 and PB0 turn on
void set_leds(int val) {
    // LED 0 (PB0) - check bit 0
    if (val & 1)
        GPIOB_ODR |= (1 << 0);   // Set bit 0 high
    else
        GPIOB_ODR &= ~(1 << 0);  // Set bit 0 low
    
    // LED 1 (PA4) - check bit 1
    if (val & 2)
        GPIOA_ODR |= (1 << 4);
    else
        GPIOA_ODR &= ~(1 << 4);
    
    // LED 2 (PA1) - check bit 2
    if (val & 4)
        GPIOA_ODR |= (1 << 1);
    else
        GPIOA_ODR &= ~(1 << 1);
    
    // LED 3 (PA0) - check bit 3
    if (val & 8)
        GPIOA_ODR |= (1 << 0);
    else
        GPIOA_ODR &= ~(1 << 0);
}

// Input Functions

// Read the 4 DIP switches and return value 0-15
// Switches are wired with pull-ups so:
//   Switch ON = reads 0 (LOW)
//   Switch OFF = reads 1 (HIGH)
// 
// Mapping (DIP positions match binary visually):
//   Switch 1 (PB4) = 8 (MSB)
//   Switch 2 (PB6) = 4
//   Switch 3 (PB8) = 2
//   Switch 4 (PB9) = 1 (LSB)
// So to enter 0101 (value 5), turn on switches 2 and 4
int read_switches(void) {
    int val = 0;
    
    // Check each switch - remember 0 means ON due to pull-ups
    if ((GPIOB_IDR & (1 << 4)) == 0)  // Switch 1
        val += 8;
    if ((GPIOB_IDR & (1 << 6)) == 0)  // Switch 2
        val += 4;
    if ((GPIOB_IDR & (1 << 8)) == 0)  // Switch 3
        val += 2;
    if ((GPIOB_IDR & (1 << 9)) == 0)  // Switch 4
        val += 1;
    
    return val;
}

// Check if user button is pressed
// Button is on PC13 and is active-low (0 when pressed)
int is_button_pressed(void) {
    // Check bit 13 of Port C input register
    // Returns 1 if pressed (bit is 0), 0 if not pressed
    return ((GPIOC_IDR & (1 << 13)) == 0);
}

// Wait until button is pressed (blocking)
void wait_for_button_press(void) {
    while (!is_button_pressed()) {
        // Do nothing, just wait
    }
}

// Wait until button is released (blocking)
void wait_for_button_release(void) {
    while (is_button_pressed()) {
        // Do nothing, just wait
    }
}

// Utility Functions

// Simple delay using a busy loop
// The actual time depends on clock speed
// count ~= 300000 is roughly half a second
void delay(int count) {
    // volatile so compiler doesn't optimize this away
    volatile int i = 0;
    while (i < count) {
        i++;
    }
}

// Generate a pseudo-random number using LCG algorithm
// Not cryptographically secure but good enough for a game
unsigned int get_random(void) {
    // Standard LCG formula: seed = (a * seed + c) mod m
    random_seed = (random_seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return random_seed;
}

// GPIO Setup

// Configure all the GPIO pins we need
// 1. Enable the clock for each GPIO port we use
// 2. Set each pin as input or output with the right mode
// 
// For STM32F103, the config registers work like this:
//   - CRL controls pins 0-7, CRH controls pins 8-15
//   - Each pin uses 4 bits: 2 for MODE, 2 for CNF
//   - Output push-pull 50MHz = 0x3
//   - Input floating = 0x4
void setup_gpio(void) {
    
    // Enable clocks for GPIO ports A, B, C
    // Without this, the ports won't work at all
    // Bit 2 = GPIOA, Bit 3 = GPIOB, Bit 4 = GPIOC
    RCC_APB2ENR |= (1 << 2) | (1 << 3) | (1 << 4);
    
    // Configure LED pins as outputs (push-pull, 50MHz = 0x3)
    
    // PA0 (LED 3) - bits [3:0] of CRL
    GPIOA_CRL &= ~(0xF << 0);   // Clear the 4 bits first
    GPIOA_CRL |= (0x3 << 0);    // Set to output
    
    // PA1 (LED 2) - bits [7:4] of CRL
    GPIOA_CRL &= ~(0xF << 4);
    GPIOA_CRL |= (0x3 << 4);
    
    // PA4 (LED 1) - bits [19:16] of CRL
    GPIOA_CRL &= ~(0xF << 16);
    GPIOA_CRL |= (0x3 << 16);
    
    // PB0 (LED 0) - bits [3:0] of CRL
    GPIOB_CRL &= ~(0xF << 0);
    GPIOB_CRL |= (0x3 << 0);
    
    // Configure switch pins as inputs (floating input = 0x4)
    
    // PB4 (Switch 1) - bits [19:16] of CRL
    GPIOB_CRL &= ~(0xF << 16);
    GPIOB_CRL |= (0x4 << 16);
    
    // PB6 (Switch 2) - bits [27:24] of CRL
    GPIOB_CRL &= ~(0xF << 24);
    GPIOB_CRL |= (0x4 << 24);
    
    // PB8 (Switch 3) - bits [3:0] of CRH (pins 8-15 use CRH)
    GPIOB_CRH &= ~(0xF << 0);
    GPIOB_CRH |= (0x4 << 0);
    
    // PB9 (Switch 4) - bits [7:4] of CRH
    GPIOB_CRH &= ~(0xF << 4);
    GPIOB_CRH |= (0x4 << 4);
    
    // PC13 (User Button) - bits [23:20] of CRH
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |= (0x4 << 20);
    
    // Make sure all LEDs start off
    set_leds(0);
}

// main.h
// Header file for Mastermind game
// Contains defines, global variable declarations, and function prototypes

#ifndef MAIN_H
#define MAIN_H

#include "stm32f103rb.h"

// Game Settings
#define MAX_GUESSES 8   // Player loses after this many wrong guesses
#define DEBUG_MODE 0    // Set to 1 for testing (code = 1,2,3,4), 0 for random

// Global Variables (defined in main.c)
extern int secret_code[4];   // The 4-digit code to guess
extern int guess[4];         // Player's current guess
extern int guess_count;      // Number of guesses made
extern unsigned int random_seed;  // For random number generation

// GPIO Setup
void setup_gpio(void);

// LED Control
void set_leds(int val);

// Input Reading
int read_switches(void);
int is_button_pressed(void);
void wait_for_button_press(void);
void wait_for_button_release(void);

// Timing
void delay(int count);

// Game Logic
void generate_secret_code(void);
void get_player_guess(void);
void calculate_feedback(int *exact, int *wrong_pos);
void display_feedback(int exact, int wrong_pos);

// Game States
void play_game(void);
void win_sequence(void);
void lose_sequence(void);

// Utility
unsigned int get_random(void);

#endif

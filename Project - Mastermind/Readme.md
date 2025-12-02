# ENSE 352 Term Project: Mastermind

**Name:** Shivam Jigneshbhai Soni

**Course:** ENSE 352 - Fundamentals of Computer Systems Architectures

**Instructor:** Cameron Oehler

## Project Description
This project is a C implementation of the code-breaking game "Mastermind" on the STM32F103RB (Nucleo) microcontroller.

## What is Mastermind?

Mastermind is a code-breaking game. The computer picks a secret 4-digit code and you have to guess it within 8 tries. After each guess, you get feedback showing how close you were.

## Hardware Setup

- **Nucleo Board:** STM32F103RB
- **LEDs:** 4 LEDs connected to PA0, PA1, PA4, PB0
- **DIP Switches:** 4 switches connected to PB4, PB6, PB8, PB9
- **Button:** Blue user button (PC13)

## How to Play

### Step 1: Start the Game
Press the blue button to start a new game.

### Step 2: Enter Your Guess
You need to enter 4 hex digits (0-15 each).

The LEDs show which digit you're entering:
- Digit 1: `1000` (one LED on)
- Digit 2: `1100` (two LEDs on)
- Digit 3: `1110` (three LEDs on)
- Digit 4: `1111` (all LEDs on)

Set your DIP switches to the value you want, then press the button.

**DIP Switch Values:**
```
[SW1][SW2][SW3][SW4]
  8    4    2    1
```

Examples:
- Value 1: Turn on SW4 only
- Value 5: Turn on SW2 and SW4 (4+1=5)
- Value 15: Turn on all switches (8+4+2+1=15)

### Step 3: Read Feedback
After your guess, the LEDs show feedback:
- **Solid LED** = correct digit in correct position
- **Flashing LED** = correct digit but wrong position

Example: If 2 LEDs are solid and 1 is flashing, you have 2 digits exactly right and 1 digit that's in the code but in the wrong spot.

Press the button to make your next guess.

### Step 4: Win or Lose

**Win:** If you guess all 4 digits correctly:
- All LEDs blink 4 times
- Then LEDs show how many guesses you used (in binary)

**Lose:** If you use all 8 guesses:
- Alternating pattern (1010/0101) blinks 4 times

Press button to play again.

## Files

- `main.c` - The game code
- `main.h` - Header file with defines and function prototypes
- `stm32f103rb.h` - Register definitions for the microcontroller
- `README.md` - This file

## Testing

For testing, you can set `DEBUG_MODE` to 1 in main.c. This makes the secret code always 1,2,3,4.

To test with debug mode: enter 1, 2, 3, 4 using the switches:
1. SW4 only → press button
2. SW3 only → press button  
3. SW3 + SW4 → press button
4. SW2 only → press button

You should see the win sequence (all LEDs blink 4 times).


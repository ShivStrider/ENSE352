# ENSE 352 Term Project: Mastermind

**Name:** Shivam Jigneshbhai Soni

**Course:** ENSE 352 - Fundamentals of Computer Systems Architectures

**Instructor:** Cameron Oehler

## Project Description
This project is a C implementation of the code-breaking game "Mastermind" on the STM32F103RB (Nucleo) microcontroller.

The goal of the game is to enter a 4-digit hexadecimal code (using DIP switches) to guess a secret sequence. This milestone implements the core input logic, allowing the user to input 4 digits sequentially with visual feedback on LEDs.

## Hardware Configuration

The project uses the following pin connections on the Nucleo board:

### Inputs
| Component | Pin | Description |
|-----------|-----|-------------|
| **DIP Switch 1** | `PB4` | Bit 0 (Value 1) |
| **DIP Switch 2** | `PB6` | Bit 1 (Value 2) |
| **DIP Switch 3** | `PB8` | Bit 2 (Value 4) |
| **DIP Switch 4** | `PB9` | Bit 3 (Value 8) |
| **User Button** | `PC13`| Confirm Selection |

*Note: DIP switches are connected with pull-up resistors (Active LOW logic).*

### Outputs (LEDs)
| Component | Pin | Description |
|-----------|-----|-------------|
| **LED 0** | `PB0` | Right-most LED |
| **LED 1** | `PA4` | Middle-Right LED |
| **LED 2** | `PA1` | Middle-Left LED |
| **LED 3** | `PA0` | Left-most LED |

## How to Play (Milestone Demo)

1.  **Start:** Reset the board. The left-most LED (`PA0`) will turn ON. This indicates the game is waiting for **Digit 1**.
2.  **Make a Selection:** Set the 4 DIP switches to your desired hex value (0-15).
    * *Example:* To enter '5', turn on Switch 1 (Value 1) and Switch 3 (Value 4).
3.  **Confirm:** Press the **Blue User Button**.
4.  **Next Digit:** The LEDs will update to show which digit needs to be entered next:
    * **Digit 1:** LED 3 ON (`1000`)
    * **Digit 2:** LED 3 & 2 ON (`1100`)
    * **Digit 3:** LED 3, 2 & 1 ON (`1110`)
    * **Digit 4:** All LEDs ON (`1111`)
5. **Completion:** After entering the 4th digit, the LEDs will turn off, then **flash twice**. This confirms the guess has been stored in memory.
6. **Restart:** Press the User Button again to reset the loop and enter a new guess.

## Future Implementation (Final Project)
The final version will include:
* Comparison logic to check the user's guess against a generated secret code.
* Feedback using LEDs: Solid LEDs for correct number/correct place, flashing LEDs for correct number/wrong place.

* Win/Loss states (blinking sequences) after 8 attempts.

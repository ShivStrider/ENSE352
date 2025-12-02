/**
 * STM32F103RB Register Definitions
 * 
 * Header file containing memory-mapped register definitions for the
 * STM32F103RB microcontroller used on the Nucleo board.
 * 
 * These addresses are derived from the STM32F103xx Reference Manual.
 */

#ifndef STM32F103RB_H
#define STM32F103RB_H

#include <stdint.h>

// RCC (Reset and Clock Control) Registers

// APB2 Peripheral Clock Enable Register
// Used to enable clocks for GPIO ports
// Bit 2: GPIOA, Bit 3: GPIOB, Bit 4: GPIOC
#define RCC_APB2ENR   (*((volatile uint32_t *)0x40021018))


// GPIO Port A Registers (Base: 0x40010800)
// Port Configuration Register Low (Pins 0-7)
#define GPIOA_CRL     (*((volatile uint32_t *)0x40010800))

// Port Configuration Register High (Pins 8-15)
#define GPIOA_CRH     (*((volatile uint32_t *)0x40010804))

// Port Input Data Register
#define GPIOA_IDR     (*((volatile uint32_t *)0x40010808))

// Port Output Data Register
#define GPIOA_ODR     (*((volatile uint32_t *)0x4001080C))

// GPIO Port B Registers (Base: 0x40010C00)
// Port Configuration Register Low (Pins 0-7)
#define GPIOB_CRL     (*((volatile uint32_t *)0x40010C00))

// Port Configuration Register High (Pins 8-15)
#define GPIOB_CRH     (*((volatile uint32_t *)0x40010C04))

// Port Input Data Register
#define GPIOB_IDR     (*((volatile uint32_t *)0x40010C08))

// Port Output Data Register
#define GPIOB_ODR     (*((volatile uint32_t *)0x40010C0C))

// GPIO Port C Registers (Base: 0x40011000)
// Port Configuration Register Low (Pins 0-7)
#define GPIOC_CRL     (*((volatile uint32_t *)0x40011000))

// Port Configuration Register High (Pins 8-15)
#define GPIOC_CRH     (*((volatile uint32_t *)0x40011004))

// Port Input Data Register
#define GPIOC_IDR     (*((volatile uint32_t *)0x40011008))

// Port Output Data Register
#define GPIOC_ODR     (*((volatile uint32_t *)0x4001100C))

// GPIO Configuration Values
// Configuration modes for CRL/CRH registers (4 bits per pin)
// Lower 2 bits: MODE (Input/Output speed)
// Upper 2 bits: CNF (Configuration)

// Input modes (MODE = 00)
#define GPIO_MODE_INPUT           0x0

// Output modes
#define GPIO_MODE_OUTPUT_10MHZ    0x1
#define GPIO_MODE_OUTPUT_2MHZ     0x2
#define GPIO_MODE_OUTPUT_50MHZ    0x3

// Input configurations (when MODE = 00)
#define GPIO_CNF_INPUT_ANALOG     (0x0 << 2)
#define GPIO_CNF_INPUT_FLOATING   (0x1 << 2)
#define GPIO_CNF_INPUT_PUPD       (0x2 << 2)

// Output configurations
#define GPIO_CNF_OUTPUT_PP        (0x0 << 2)  // Push-Pull
#define GPIO_CNF_OUTPUT_OD        (0x1 << 2)  // Open-Drain
#define GPIO_CNF_AF_PP            (0x2 << 2)  // Alternate Function Push-Pull
#define GPIO_CNF_AF_OD            (0x3 << 2)  // Alternate Function Open-Drain

// Common combined configurations
#define GPIO_INPUT_FLOATING       0x4  // Floating input (CNF=01, MODE=00)
#define GPIO_INPUT_PUPD           0x8  // Input with pull-up/down (CNF=10, MODE=00)
#define GPIO_OUTPUT_PP_50MHZ      0x3  // Push-pull output, 50MHz (CNF=00, MODE=11)
#define GPIO_OUTPUT_PP_2MHZ       0x2  // Push-pull output, 2MHz (CNF=00, MODE=10)

#endif // STM32F103RB_H

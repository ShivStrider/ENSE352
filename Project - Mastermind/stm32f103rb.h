// stm32f103rb.h
// ENSE352 
// DATE: December 2, 2025
// AUTHOR: Shivam Soni (200474721) 
// STM32F103RB Register Definitions
// 
// Header file containing memory-mapped register definitions for the
// STM32F103RB microcontroller used on the Nucleo board.
// 
// These addresses are derived from the STM32F103xx Reference Manual.

#ifndef STM32F103RB_H
#define STM32F103RB_H

#include <stdint.h>

// RCC (Reset and Clock Control) Registers

// APB2 Peripheral Clock Enable Register
// Used to enable clocks for GPIO ports
// Bit 2: GPIOA, Bit 3: GPIOB, Bit 4: GPIOC
#define RCC_APB2ENR   (*((volatile uint32_t *)0x40021018))

// GPIO Port A Registers (Base: 0x40010800)

#define GPIOA_CRL     (*((volatile uint32_t *)0x40010800))  // Config Register Low (Pins 0-7)
#define GPIOA_CRH     (*((volatile uint32_t *)0x40010804))  // Config Register High (Pins 8-15)
#define GPIOA_IDR     (*((volatile uint32_t *)0x40010808))  // Input Data Register
#define GPIOA_ODR     (*((volatile uint32_t *)0x4001080C))  // Output Data Register

// GPIO Port B Registers (Base: 0x40010C00)

#define GPIOB_CRL     (*((volatile uint32_t *)0x40010C00))  // Config Register Low (Pins 0-7)
#define GPIOB_CRH     (*((volatile uint32_t *)0x40010C04))  // Config Register High (Pins 8-15)
#define GPIOB_IDR     (*((volatile uint32_t *)0x40010C08))  // Input Data Register
#define GPIOB_ODR     (*((volatile uint32_t *)0x40010C0C))  // Output Data Register

// GPIO Port C Registers (Base: 0x40011000)

#define GPIOC_CRL     (*((volatile uint32_t *)0x40011000))  // Config Register Low (Pins 0-7)
#define GPIOC_CRH     (*((volatile uint32_t *)0x40011004))  // Config Register High (Pins 8-15)
#define GPIOC_IDR     (*((volatile uint32_t *)0x40011008))  // Input Data Register
#define GPIOC_ODR     (*((volatile uint32_t *)0x4001100C))  // Output Data Register

#endif

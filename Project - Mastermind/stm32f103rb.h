#ifndef STM32F10X_H
#define STM32F10X_H

#include <stdint.h>

// These are just the memory addresses from the manual.
// We cast them to (volatile uint32_t *) so we can write to them.

// RCC (Clock Control) Registers
#define RCC_APB2ENR   (*((volatile uint32_t *)0x40021018))

// GPIO Port A Registers
#define GPIOA_CRL     (*((volatile uint32_t *)0x40010800))
#define GPIOA_ODR     (*((volatile uint32_t *)0x4001080C))
#define GPIOA_IDR     (*((volatile uint32_t *)0x40010808))

// GPIO Port B Registers
#define GPIOB_CRL     (*((volatile uint32_t *)0x40010C00))
#define GPIOB_CRH     (*((volatile uint32_t *)0x40010C04))
#define GPIOB_ODR     (*((volatile uint32_t *)0x40010C0C))
#define GPIOB_IDR     (*((volatile uint32_t *)0x40010C08))

// GPIO Port C Registers (for the Blue Button)
#define GPIOC_CRH     (*((volatile uint32_t *)0x40011004))
#define GPIOC_IDR     (*((volatile uint32_t *)0x40011008))

#endif
#include "stdint.h"
#include "inc/tm4c123gh6pm.h"

void portinit();
void systickinit();
void saatayar(); // RCC configuration
void saatayar2(); // RCC2 configuration
void PLL_Init(void); // Custom PLL initialization
void saatayar3direkt(); // Direct clock configuration

int main(void) {
    saatayar();
    saatayar3direkt();
    portinit();
    systickinit();

    int a = 0;
    while (1) {
        if (NVIC_ST_CTRL_R & 0x10000) { // Check if SysTick timer has triggered
            a++;
            if (a == 8) { // Toggle LED every 8 ticks
                GPIO_PORTF_DATA_R ^= 0x04;
                a = 0;
            }
        }
    }
}

void portinit() {
    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for Port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock Port F
    GPIO_PORTF_CR_R = 0x01;   // Enable configuration for PF0
    GPIO_PORTF_DIR_R |= 0x0E; // Set PF1, PF2, PF3 as output
    GPIO_PORTF_DEN_R |= 0x1F; // Enable digital functionality for PF0-PF4
    GPIO_PORTF_PUR_R = 0x11; // Enable pull-up resistors for PF0 and PF4
}

void systickinit() {
    NVIC_ST_CTRL_R = 0;  // Disable SysTick
    NVIC_ST_RELOAD_R = 10000000 - 1; // Set reload value for 1-second delay
    NVIC_ST_CURRENT_R = 0; // Clear current value
    NVIC_ST_CTRL_R = (1 << 2) | (1 << 0); // Enable SysTick with system clock
}

void PLL_Init(void) {
    SYSCTL_RCC2_R |= 0x80000000;  // Use RCC2
    SYSCTL_RCC2_R |= 0x00000800;  // Bypass PLL
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) + 0x00000540; // Configure 16 MHz crystal
    SYSCTL_RCC2_R &= ~0x00000070;  // Main oscillator source
    SYSCTL_RCC2_R &= ~0x00002000;  // Activate PLL
    SYSCTL_RCC2_R |= 0x40000000;   // Use 400 MHz PLL
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22); // Set clock divider
    while ((SYSCTL_RIS_R & 0x00000040) == 0) {} // Wait for PLL to lock
    SYSCTL_RCC2_R &= ~0x00000800;  // Enable PLL
}

void saatayar2() {
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2; // Use RCC2
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2; // Bypass PLL
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~SYSCTL_RCC_XTAL_M) | SYSCTL_RCC_XTAL_16MHZ; // Configure 16 MHz crystal
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M; // Main oscillator source
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2; // Activate PLL
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400; // Use 400 MHz PLL
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~(SYSCTL_RCC2_SYSDIV2_M | SYSCTL_RCC2_SYSDIV2LSB)) + (4 << 22); // Set clock divider
    while ((SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) == 0) {} // Wait for PLL to lock
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2; // Enable PLL
}

void saatayar() {
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_USERCC2; // Use RCC
    SYSCTL_RCC_R |= SYSCTL_RCC_BYPASS; // Bypass PLL
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~SYSCTL_RCC_XTAL_M) | SYSCTL_RCC_XTAL_16MHZ; // Configure 16 MHz crystal
    SYSCTL_RCC_R &= ~SYSCTL_RCC_OSCSRC_M; // Main oscillator source
    SYSCTL_RCC_R &= ~SYSCTL_RCC_PWRDN; // Activate PLL
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_DIV400; // Use 400 MHz PLL
    SYSCTL_RCC_R &= ~SYSCTL_RCC_SYSDIV_M; // Clear clock divider
    SYSCTL_RCC_R |= SYSCTL_RCC_USESYSDIV + (4 << 22); // Set clock divider
    while ((SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) == 0) {} // Wait for PLL to lock
    SYSCTL_RCC_R &= ~SYSCTL_RCC_BYPASS; // Enable PLL
}

void saatayar3direkt() {
    SYSCTL_RCC2_R &= ~0xC0000000;   // Use RCC, clear div400
    SYSCTL_RCC_R &= 0xF83FD00F; // Clear specific fields
    SYSCTL_RCC_R |= 0x02400D40; // Configure for 80 MHz
    SYSCTL_RCC_R &= ~0x00000800; // Enable PLL
}

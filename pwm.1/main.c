#include "stdint.h"
#include "inc/tm4c123gh6pm.h"

void Delay_ms(int time_ms) {
    int i, j;
    for (i = 0; i < time_ms; i++)
        for (j = 0; j < 3180; j++)
            {}  // Execute NOP for 1ms
}

int main(void) {
    volatile unsigned long delay;

    // Enable PWM clock
    SYSCTL_RCGC0_R |= 0x00100000;
    delay = SYSCTL_RCGC0_R;

    // Enable GPIO Port B clock
    SYSCTL_RCGC2_R |= 0x00000002;
    delay = SYSCTL_RCGC2_R;

    // Configure PB6 and PB7 for alternate functions
    GPIO_PORTB_AFSEL_R |= 0xC0;
    GPIO_PORTB_DEN_R |= 0xC0;
    GPIO_PORTB_AMSEL_R &= ~0xC0;

    // Assign PWM signals to PB6 and PB7
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0x00FFFFFF) | 0x44000000;

    // Enable PWM clock divider and set to divide by 2
    SYSCTL_RCC_R |= 0x00100000;
    SYSCTL_RCC_R &= ~0x000E0000;

    // Configure PWM generator for countdown mode
    PWM0_0_CTL_R = 0x00000000;
    PWM0_0_GENA_R = 0x0000008C;

    // Set PWM load value for a 50% duty cycle
    PWM0_0_LOAD_R = 320 - 1;
    int duty_cycle = 320 - 1;
    PWM0_0_CMPA_R = duty_cycle;

    // Enable PWM generator and output
    PWM0_0_CTL_R = 0x00000001;
    PWM0_ENABLE_R = 0x01;

    while (1) {
        PWM0_0_CMPA_R = duty_cycle;
        duty_cycle--;
        Delay_ms(10);

        if (duty_cycle < 0) {
            duty_cycle = 320 - 1;
        }
    }

    return 0;
}

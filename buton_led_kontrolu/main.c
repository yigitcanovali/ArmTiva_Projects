#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"

// SW1 ve SW2 bayraklar� (SORU 3 i�in gerekli)
volatile bool sw1_pressed = false; // SW1'in durumu
volatile bool sw2_pressed = false; // SW2'nin durumu

// SORU 3: GPIO Kesme ��leyicisi
void GPIOKesmesi(void) {
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // Kesme bayraklar�n� temizle

    // SW1 (PF4) kontrol�
    if ((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) & GPIO_PIN_4) == 0) { // SW1 bas�l�
        sw1_pressed = true; // SW1'e bas�ld� bayra�� set edilir
    } else {
        sw1_pressed = false; // SW1 b�rak�ld�
    }

    // SW2 (PF0) kontrol�
    if ((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) & GPIO_PIN_0) == 0) { // SW2 bas�l�
        sw2_pressed = true; // SW2'ye bas�ld� bayra�� set edilir
    } else {
        sw2_pressed = false; // SW2 b�rak�ld�
    }
}

// SORU 4: Timer Kesme ��leyicisi
void TIMERKesmesi(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Timer kesme bayra��n� temizle
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1) ^ GPIO_PIN_1); // PF1 LED'ini toggle yap
}

int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // 40 MHz sistem saatini ayarla

    // SORU 1: PF1 (k�rm�z� LED), PF3 (ye�il LED) ��k�� olarak ayarla
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // GPIOF portunu etkinle�tir
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_3); // PF1 ve PF3 ��k��
    HWREG(GPIO_PORTF_BASE + 0x3FC) |= (1<<1)|(1<<3)|(1<<5);
    // SORU 2: PF0 (SW2) ve PF4 (SW1) giri� olarak ayarla
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // PF0 ve PF4 giri�
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; // PF0 i�in kilidi a�
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;    // PF0 i�in eri�im izni
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;           // Kilidi tekrar kapat
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU); // Pull-up diren� etkinle�tir

    // SORU 3: PF0 ve PF4 i�in kesme ayarlar�
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE); // D��en kenar kesmesi
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // PF0 ve PF4 i�in kesmeyi etkinle�tir
    GPIOIntRegister(GPIO_PORTF_BASE, GPIOKesmesi); // GPIO kesme i�leyicisini ba�la
    IntEnable(INT_GPIOF); // NVIC'de GPIOF kesmesini etkinle�tir
    IntMasterEnable(); // T�m kesmeleri etkinle�tir

    // SORU 4: Timer Ayarlar�
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Timer0 i�in clock sinyalini etkinle�tir
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // Timer0'� periyodik moda ayarla
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 4); // 4 Hz i�in y�k de�eri (40MHz / 4)
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Timer kesmesini etkinle�tir
    TimerIntRegister(TIMER0_BASE, TIMER_A, TIMERKesmesi); // Timer kesme i�leyicisini ba�la
    IntEnable(INT_TIMER0A); // Timer0A kesmesini etkinle�tir
    TimerEnable(TIMER0_BASE, TIMER_A); // Timer0'� ba�lat

    while (1) {
        // SW1 (PF4) kontrol�: K�rm�z� LED (PF1)
        if (sw1_pressed) {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // K�rm�z� LED yak
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // K�rm�z� LED s�nd�r
        }

        // SW2 (PF0) kontrol�: Ye�il LED (PF3)
        if (sw2_pressed) {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Ye�il LED yak
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); // Ye�il LED s�nd�r
        }
    }
}

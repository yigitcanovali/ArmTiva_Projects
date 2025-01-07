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

// SW1 ve SW2 bayraklarý (SORU 3 için gerekli)
volatile bool sw1_pressed = false; // SW1'in durumu
volatile bool sw2_pressed = false; // SW2'nin durumu

// SORU 3: GPIO Kesme Ýþleyicisi
void GPIOKesmesi(void) {
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // Kesme bayraklarýný temizle

    // SW1 (PF4) kontrolü
    if ((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) & GPIO_PIN_4) == 0) { // SW1 basýlý
        sw1_pressed = true; // SW1'e basýldý bayraðý set edilir
    } else {
        sw1_pressed = false; // SW1 býrakýldý
    }

    // SW2 (PF0) kontrolü
    if ((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) & GPIO_PIN_0) == 0) { // SW2 basýlý
        sw2_pressed = true; // SW2'ye basýldý bayraðý set edilir
    } else {
        sw2_pressed = false; // SW2 býrakýldý
    }
}

// SORU 4: Timer Kesme Ýþleyicisi
void TIMERKesmesi(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Timer kesme bayraðýný temizle
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1) ^ GPIO_PIN_1); // PF1 LED'ini toggle yap
}

int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // 40 MHz sistem saatini ayarla

    // SORU 1: PF1 (kýrmýzý LED), PF3 (yeþil LED) çýkýþ olarak ayarla
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // GPIOF portunu etkinleþtir
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_3); // PF1 ve PF3 çýkýþ
    HWREG(GPIO_PORTF_BASE + 0x3FC) |= (1<<1)|(1<<3)|(1<<5);
    // SORU 2: PF0 (SW2) ve PF4 (SW1) giriþ olarak ayarla
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // PF0 ve PF4 giriþ
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; // PF0 için kilidi aç
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;    // PF0 için eriþim izni
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;           // Kilidi tekrar kapat
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU); // Pull-up direnç etkinleþtir

    // SORU 3: PF0 ve PF4 için kesme ayarlarý
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE); // Düþen kenar kesmesi
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // PF0 ve PF4 için kesmeyi etkinleþtir
    GPIOIntRegister(GPIO_PORTF_BASE, GPIOKesmesi); // GPIO kesme iþleyicisini baðla
    IntEnable(INT_GPIOF); // NVIC'de GPIOF kesmesini etkinleþtir
    IntMasterEnable(); // Tüm kesmeleri etkinleþtir

    // SORU 4: Timer Ayarlarý
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Timer0 için clock sinyalini etkinleþtir
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // Timer0'ý periyodik moda ayarla
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 4); // 4 Hz için yük deðeri (40MHz / 4)
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Timer kesmesini etkinleþtir
    TimerIntRegister(TIMER0_BASE, TIMER_A, TIMERKesmesi); // Timer kesme iþleyicisini baðla
    IntEnable(INT_TIMER0A); // Timer0A kesmesini etkinleþtir
    TimerEnable(TIMER0_BASE, TIMER_A); // Timer0'ý baþlat

    while (1) {
        // SW1 (PF4) kontrolü: Kýrmýzý LED (PF1)
        if (sw1_pressed) {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // Kýrmýzý LED yak
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // Kýrmýzý LED söndür
        }

        // SW2 (PF0) kontrolü: Yeþil LED (PF3)
        if (sw2_pressed) {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Yeþil LED yak
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); // Yeþil LED söndür
        }
    }
}

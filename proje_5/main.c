#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/hibernate.h"

void HibernationInit(void);
void LEDBlink(void);

int main(void) {
    // Clock yapýlandýrmasý
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // GPIO Baþlangýcý
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); // PF1 - LED
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);  // PF4 - Düðme
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Hibernation Baþlat
    HibernationInit();

    while (1) {
        // Düðmeye basýlý deðilse (PF4 HIGH)
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == GPIO_PIN_4) {
            // Hibernation moduna geç
            HibernateRequest();
        }

        // Düðmeye basýlmýþsa LED yanýp söner
        LEDBlink();
    }
}

void HibernationInit(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    HibernateEnableExpClk(SysCtlClockGet());
    HibernateWakeSet(HIBERNATE_WAKE_PIN); // GPIO pin ile uyandýrma
}

void LEDBlink(void) {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // LED ON
    SysCtlDelay(SysCtlClockGet() / 10); // Gecikme
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);      // LED OFF
    SysCtlDelay(SysCtlClockGet() / 10); // Gecikme
}

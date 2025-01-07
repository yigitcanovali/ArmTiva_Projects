#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/hibernate.h"

void HibernationInit(void);
void LEDBlink(void);

int main(void) {
    // Clock yap�land�rmas�
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // GPIO Ba�lang�c�
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); // PF1 - LED
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);  // PF4 - D��me
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Hibernation Ba�lat
    HibernationInit();

    while (1) {
        // D��meye bas�l� de�ilse (PF4 HIGH)
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == GPIO_PIN_4) {
            // Hibernation moduna ge�
            HibernateRequest();
        }

        // D��meye bas�lm��sa LED yan�p s�ner
        LEDBlink();
    }
}

void HibernationInit(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    HibernateEnableExpClk(SysCtlClockGet());
    HibernateWakeSet(HIBERNATE_WAKE_PIN); // GPIO pin ile uyand�rma
}

void LEDBlink(void) {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // LED ON
    SysCtlDelay(SysCtlClockGet() / 10); // Gecikme
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);      // LED OFF
    SysCtlDelay(SysCtlClockGet() / 10); // Gecikme
}

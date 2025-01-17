#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.c"
#include "driverlib/adc.h"
#include "inc/tm4c123gh6pm.h"


void SetInitSettings();
uint32_t value[1];

void ADCInterruptHandler()
{
    // Clear ADC interrupt flag
    ADCIntClear(ADC0_BASE, 3);

    // Get the ADC conversion result
    ADCSequenceDataGet(ADC0_BASE, 3, value);

    // Trigger the ADC conversion again
    ADCProcessorTrigger(ADC0_BASE, 3);
}

int main(void)
{
    SetInitSettings();

    while(1)
    {
        // Main loop
    }

    return 0;
}

void SetInitSettings()
{
    // Set system clock to 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Enable GPIO Port F and ADC0 peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Configure ADC0, sequence 3
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);

    // Enable interrupts
    IntMasterEnable();
    IntEnable(INT_ADC0SS3);

    // Register and enable the ADC interrupt handler
    ADCIntClear(ADC0_BASE, 3);
    ADCIntRegister(ADC0_BASE, 3, ADCInterruptHandler);
    ADCIntEnable(ADC0_BASE, 3);

    // Trigger the ADC conversion for the first time
    ADCProcessorTrigger(ADC0_BASE, 3);
}

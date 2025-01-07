#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/hibernate.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

void ADCInit(void);
void TimerInit(void);
void HibernationInit(void);
float ReadTemperature(void);
void Timer0AIntHandler(void);

volatile bool systemWakeUp = false;

int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    ADCInit();
    TimerInit();
    HibernationInit();

    float temperatureSum = 0.0;
    uint8_t readingCount = 0;

    while (1) {
        float temperature = ReadTemperature();
        temperatureSum += temperature;
        readingCount++;

        if (readingCount == 10) {
            float averageTemperature = temperatureSum / readingCount;
            temperatureSum = 0.0;
            readingCount = 0;
        }

        systemWakeUp = false;
        HibernateRequest();

        while (!systemWakeUp) {
            // Hibernation bekleniyor
        }
    }
}

void ADCInit(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
}

float ReadTemperature(void) {
    uint32_t adcValue;

    ADCProcessorTrigger(ADC0_BASE, 3);
    while (!ADCIntStatus(ADC0_BASE, 3, false));
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, &adcValue);

    float temperature = (adcValue / 4095.0) * 3.3 * 100;
    return temperature;
}

void TimerInit(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    uint32_t timerPeriod = SysCtlClockGet() / 2; // 0.5 saniye
    TimerLoadSet(TIMER0_BASE, TIMER_A, timerPeriod - 1);

    IntEnable(INT_TIMER0A); // INT_TIMER0A kesmesi etkinleþtirildi
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void Timer0AIntHandler(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    systemWakeUp = true;
}

void HibernationInit(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    HibernateEnableExpClk(SysCtlClockGet());
    HibernateRTCSet(0);
    HibernateRTCEnable();
    HibernateWakeSet(HIBERNATE_WAKE_PIN | HIBERNATE_WAKE_RTC);
    HibernateRTCMatchSet(0, HibernateRTCGet() + 2);
}

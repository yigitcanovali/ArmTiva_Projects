#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/fpu.h"

// Fonksiyon Prototipleri
void ADCInit(void);
float ReadTemperature(void);

int main(void) {
    // Sistem Clock ve FPU yapýlandýrmasý
    FPUEnable();                        // Floating Point iþlemleri için FPU etkinleþtir
    FPULazyStackingEnable();            // Lazy Stacking ile performans iyileþtirmesi
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // ADC'yi baþlat
    ADCInit();

    float temperatureCelsius = 0.0;     // Celsius sýcaklýk deðeri
    float temperatureSum = 0.0;         // Ortalama için toplam sýcaklýk
    uint8_t readingCount = 0;           // Okuma sayacý

    while (1) {
        // ADC'den sýcaklýk okuma
        temperatureCelsius = ReadTemperature();

        // Sýcaklýk verisini topla
        temperatureSum += temperatureCelsius;
        readingCount++;

        // Ortalama sýcaklýk hesaplama (örneðin her 10 ölçümde bir)
        if (readingCount == 10) {
            float averageTemperature = temperatureSum / readingCount;
            temperatureSum = 0.0;  // Toplamý sýfýrla
            readingCount = 0;

            // Ortalama sýcaklýk kullanýlabilir, örneðin bir ekrana yazdýrýlabilir
            // (burada sadece placeholder olarak býrakýyoruz)
        }

        // Küçük bir gecikme (örneðin 1 saniye)
        SysCtlDelay(SysCtlClockGet() / 3);
    }
}

// ADC Baþlatma Fonksiyonu
void ADCInit(void) {
    // ADC0 ve PE3 pinlerini etkinleþtir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);  // ADC0 modülü etkinleþtir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // GPIO Port E etkinleþtir

    // PE3 pinini ADC olarak yapýlandýr
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    // ADC Sequence 3 yapýlandýrmasý
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); // Yazýlým tetiklemeli
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
}

// Sýcaklýk Okuma Fonksiyonu
float ReadTemperature(void) {
    uint32_t adcValue;

    // ADC ölçüm tetikle
    ADCProcessorTrigger(ADC0_BASE, 3);
    while (!ADCIntStatus(ADC0_BASE, 3, false)); // ADC iþlemi tamamlanana kadar bekle
    ADCIntClear(ADC0_BASE, 3);                 // ADC bayraðýný temizle
    ADCSequenceDataGet(ADC0_BASE, 3, &adcValue); // ADC verisini al

    // ADC verisini Celsius sýcaklýða çevir
    // Varsayým: 0-3.3V arasýnda bir sensör kullanýyoruz.
    float temperature = (adcValue / 4095.0) * 3.3 * 100; // Örnek dönüþüm
    return temperature;
}

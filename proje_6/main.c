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
    // Sistem Clock ve FPU yap�land�rmas�
    FPUEnable();                        // Floating Point i�lemleri i�in FPU etkinle�tir
    FPULazyStackingEnable();            // Lazy Stacking ile performans iyile�tirmesi
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // ADC'yi ba�lat
    ADCInit();

    float temperatureCelsius = 0.0;     // Celsius s�cakl�k de�eri
    float temperatureSum = 0.0;         // Ortalama i�in toplam s�cakl�k
    uint8_t readingCount = 0;           // Okuma sayac�

    while (1) {
        // ADC'den s�cakl�k okuma
        temperatureCelsius = ReadTemperature();

        // S�cakl�k verisini topla
        temperatureSum += temperatureCelsius;
        readingCount++;

        // Ortalama s�cakl�k hesaplama (�rne�in her 10 �l��mde bir)
        if (readingCount == 10) {
            float averageTemperature = temperatureSum / readingCount;
            temperatureSum = 0.0;  // Toplam� s�f�rla
            readingCount = 0;

            // Ortalama s�cakl�k kullan�labilir, �rne�in bir ekrana yazd�r�labilir
            // (burada sadece placeholder olarak b�rak�yoruz)
        }

        // K���k bir gecikme (�rne�in 1 saniye)
        SysCtlDelay(SysCtlClockGet() / 3);
    }
}

// ADC Ba�latma Fonksiyonu
void ADCInit(void) {
    // ADC0 ve PE3 pinlerini etkinle�tir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);  // ADC0 mod�l� etkinle�tir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // GPIO Port E etkinle�tir

    // PE3 pinini ADC olarak yap�land�r
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    // ADC Sequence 3 yap�land�rmas�
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); // Yaz�l�m tetiklemeli
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
}

// S�cakl�k Okuma Fonksiyonu
float ReadTemperature(void) {
    uint32_t adcValue;

    // ADC �l��m tetikle
    ADCProcessorTrigger(ADC0_BASE, 3);
    while (!ADCIntStatus(ADC0_BASE, 3, false)); // ADC i�lemi tamamlanana kadar bekle
    ADCIntClear(ADC0_BASE, 3);                 // ADC bayra��n� temizle
    ADCSequenceDataGet(ADC0_BASE, 3, &adcValue); // ADC verisini al

    // ADC verisini Celsius s�cakl��a �evir
    // Varsay�m: 0-3.3V aras�nda bir sens�r kullan�yoruz.
    float temperature = (adcValue / 4095.0) * 3.3 * 100; // �rnek d�n���m
    return temperature;
}

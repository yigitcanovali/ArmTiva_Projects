#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

// Global Deðiþkenler
char saatBilgisi[9] = "00:00:00"; // Baþlangýç saati
uint32_t adcValue = 0;
bool yeniSaatGeldi = false; // Yeni saat bilgisi gönderildi mi?
bool saatGuncelle = true;   // Saat sürekli güncellensin mi?

// UART Kesme Servis Rutini
void UART0Handler(void) {
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT); // Kesme bayraðýný temizle

    static char buffer[16]; // Komutlar ve saat verisi için buffer
    static int index = 0;

    while (UARTCharsAvail(UART0_BASE)) {
        char receivedChar = UARTCharGet(UART0_BASE);

        // Gelen veriyi buffer'a yaz
        if (receivedChar == '\n') {
            buffer[index] = '\0'; // Null-terminator ekle
            index = 0;            // Ýndeksi sýfýrla

            // "RESET_SYSTEM" Komutu
            if (strcmp(buffer, "RESET_SYSTEM") == 0) {
                LCDTemizle();                    // LCD temizle
                strcpy(saatBilgisi, "00:00:00"); // Saat sýfýrla
                yeniSaatGeldi = false;          // Yeni saat bilgisi sýfýrla
                saatGuncelle = true;            // Saat güncellemeyi aktif et
                adcValue = 0;                   // ADC deðerini sýfýrla
            }
            // Saat Gönderimi
            else if (strlen(buffer) == 8 && buffer[2] == ':' && buffer[5] == ':') {
                strcpy(saatBilgisi, buffer);    // Gelen saat bilgisini kaydet
                yeniSaatGeldi = true;          // Yeni saat iþaretini ayarla
                saatGuncelle = true;           // Saat güncellemeyi aktif et
            }
        } else {
            if (index < sizeof(buffer) - 1) {
                buffer[index++] = receivedChar;
            }
        }
    }
}

// ADC Ölçüm ve UART Veri Gönderimi
void sendADCandTime() {
    char buffer[32];
    int i;

    // ADC Deðeri Oku
    ADCProcessorTrigger(ADC0_BASE, 3);
    while (!ADCIntStatus(ADC0_BASE, 3, false)) {}
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, &adcValue);

    // UART Üzerinden Saat ve ADC Verisi Gönder
    snprintf(buffer, sizeof(buffer), "Time:%s ADC:%03d\n", saatBilgisi, adcValue);
    for (i = 0; buffer[i] != '\0'; i++) {
        UARTCharPut(UART0_BASE, buffer[i]);
    }

    // Eðer yeni saat bilgisi geldiyse, saati sýfýrdan baþlat
    if (yeniSaatGeldi) {
        yeniSaatGeldi = false; // Yeni saat iþaretini sýfýrla
    } else if (saatGuncelle) {
        // Saat Bilgisini Güncelle
        int hour, min, sec;
        sscanf(saatBilgisi, "%2d:%2d:%2d", &hour, &min, &sec);
        sec++;
        if (sec == 60) { sec = 0; min++; }
        if (min == 60) { min = 0; hour++; }
        if (hour == 24) { hour = 0; }
        snprintf(saatBilgisi, sizeof(saatBilgisi), "%02d:%02d:%02d", hour, min, sec);
    }
}

// LCD Güncelleme
void updateLCD() {
    char buffer[16];

    // Üst Satýrda Metin
    LCDgit(0, 0); // Ýlk satýr, ilk sütun
    lcdyaz("Veriler:");

    // Sol Alt Kýsýmda ADC Verisi
    LCDgit(1, 0); // Ýkinci satýr, ilk sütun
    snprintf(buffer, sizeof(buffer), "P:%04d", adcValue); // ADC deðeri 4 haneli
    lcdyaz(buffer);

    // Sað Alt Kýsýmda Saat
    LCDgit(1, 8); // Ýkinci satýr, sekizinci sütun
    lcdyaz(saatBilgisi); // Saat verisini olduðu gibi yazdýr
}

// Baþlatma ve Konfigürasyon Ayarlarý
void initSystem() {
    // Sistem Saati Ayarý
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // UART Ayarlarý
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX); // RX Pin Yapýlandýrma
    GPIOPinConfigure(GPIO_PA1_U0TX); // TX Pin Yapýlandýrma
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTIntRegister(UART0_BASE, UART0Handler);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    IntMasterEnable();

    // ADC Ayarlarý
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);

    // LCD Ayarlarý
    LCDilkayarlar();
    LCDTemizle();
}

int main(void) {
    // Sistem Baþlatma
    initSystem();

    while (1) {
        sendADCandTime(); // ADC ve Saat Verisini Gönder
        updateLCD();      // LCD'yi Güncelle
        SysCtlDelay(SysCtlClockGet() / 3); // 1 Saniye Gecikme
    }
}

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

// Global De�i�kenler
char saatBilgisi[9] = "00:00:00"; // Ba�lang�� saati
uint32_t adcValue = 0;
bool yeniSaatGeldi = false; // Yeni saat bilgisi g�nderildi mi?
bool saatGuncelle = true;   // Saat s�rekli g�ncellensin mi?

// UART Kesme Servis Rutini
void UART0Handler(void) {
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT); // Kesme bayra��n� temizle

    static char buffer[16]; // Komutlar ve saat verisi i�in buffer
    static int index = 0;

    while (UARTCharsAvail(UART0_BASE)) {
        char receivedChar = UARTCharGet(UART0_BASE);

        // Gelen veriyi buffer'a yaz
        if (receivedChar == '\n') {
            buffer[index] = '\0'; // Null-terminator ekle
            index = 0;            // �ndeksi s�f�rla

            // "RESET_SYSTEM" Komutu
            if (strcmp(buffer, "RESET_SYSTEM") == 0) {
                LCDTemizle();                    // LCD temizle
                strcpy(saatBilgisi, "00:00:00"); // Saat s�f�rla
                yeniSaatGeldi = false;          // Yeni saat bilgisi s�f�rla
                saatGuncelle = true;            // Saat g�ncellemeyi aktif et
                adcValue = 0;                   // ADC de�erini s�f�rla
            }
            // Saat G�nderimi
            else if (strlen(buffer) == 8 && buffer[2] == ':' && buffer[5] == ':') {
                strcpy(saatBilgisi, buffer);    // Gelen saat bilgisini kaydet
                yeniSaatGeldi = true;          // Yeni saat i�aretini ayarla
                saatGuncelle = true;           // Saat g�ncellemeyi aktif et
            }
        } else {
            if (index < sizeof(buffer) - 1) {
                buffer[index++] = receivedChar;
            }
        }
    }
}

// ADC �l��m ve UART Veri G�nderimi
void sendADCandTime() {
    char buffer[32];
    int i;

    // ADC De�eri Oku
    ADCProcessorTrigger(ADC0_BASE, 3);
    while (!ADCIntStatus(ADC0_BASE, 3, false)) {}
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, &adcValue);

    // UART �zerinden Saat ve ADC Verisi G�nder
    snprintf(buffer, sizeof(buffer), "Time:%s ADC:%03d\n", saatBilgisi, adcValue);
    for (i = 0; buffer[i] != '\0'; i++) {
        UARTCharPut(UART0_BASE, buffer[i]);
    }

    // E�er yeni saat bilgisi geldiyse, saati s�f�rdan ba�lat
    if (yeniSaatGeldi) {
        yeniSaatGeldi = false; // Yeni saat i�aretini s�f�rla
    } else if (saatGuncelle) {
        // Saat Bilgisini G�ncelle
        int hour, min, sec;
        sscanf(saatBilgisi, "%2d:%2d:%2d", &hour, &min, &sec);
        sec++;
        if (sec == 60) { sec = 0; min++; }
        if (min == 60) { min = 0; hour++; }
        if (hour == 24) { hour = 0; }
        snprintf(saatBilgisi, sizeof(saatBilgisi), "%02d:%02d:%02d", hour, min, sec);
    }
}

// LCD G�ncelleme
void updateLCD() {
    char buffer[16];

    // �st Sat�rda Metin
    LCDgit(0, 0); // �lk sat�r, ilk s�tun
    lcdyaz("Veriler:");

    // Sol Alt K�s�mda ADC Verisi
    LCDgit(1, 0); // �kinci sat�r, ilk s�tun
    snprintf(buffer, sizeof(buffer), "P:%04d", adcValue); // ADC de�eri 4 haneli
    lcdyaz(buffer);

    // Sa� Alt K�s�mda Saat
    LCDgit(1, 8); // �kinci sat�r, sekizinci s�tun
    lcdyaz(saatBilgisi); // Saat verisini oldu�u gibi yazd�r
}

// Ba�latma ve Konfig�rasyon Ayarlar�
void initSystem() {
    // Sistem Saati Ayar�
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // UART Ayarlar�
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX); // RX Pin Yap�land�rma
    GPIOPinConfigure(GPIO_PA1_U0TX); // TX Pin Yap�land�rma
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTIntRegister(UART0_BASE, UART0Handler);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    IntMasterEnable();

    // ADC Ayarlar�
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);

    // LCD Ayarlar�
    LCDilkayarlar();
    LCDTemizle();
}

int main(void) {
    // Sistem Ba�latma
    initSystem();

    while (1) {
        sendADCandTime(); // ADC ve Saat Verisini G�nder
        updateLCD();      // LCD'yi G�ncelle
        SysCtlDelay(SysCtlClockGet() / 3); // 1 Saniye Gecikme
    }
}

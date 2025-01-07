#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "lcd.h"

void otuzhexgonder() {
    SysCtlDelay(100000);
    GPIOPinWrite(LCDPORT, RS, 0); // RS = 0 (Komut)
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, 0x30); // 0x30 gönder
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW
}

void lcdkomut(unsigned char c) {
    GPIOPinWrite(LCDPORT, RS, 0); // RS = 0 (Komut)

    // Üst nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xF0));
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW

    // Alt nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c << 4) & 0xF0);
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW

    SysCtlDelay(2000); // Komut sonrasý kýsa gecikme
}

void lcdkarakteryaz(unsigned char c) {
    GPIOPinWrite(LCDPORT, RS, RS); // RS = 1 (Veri)

    // Üst nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xF0));
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW

    // Alt nibble gönder
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c << 4) & 0xF0);
    GPIOPinWrite(LCDPORT, E, E); // Enable HIGH
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0); // Enable LOW

    SysCtlDelay(2000); // Veri sonrasý kýsa gecikme
}

void LCDilkayarlar() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // PortB etkinleþtir
    GPIOPinTypeGPIOOutput(LCDPORT, RS | E | D4 | D5 | D6 | D7); // Pinleri çýkýþ yap

    // Önceki verilerden kalan olasý artýklarý temizlemek için tüm pinleri sýfýrla
    GPIOPinWrite(LCDPORT, RS | E | D4 | D5 | D6 | D7, 0x00); // Tüm pinleri LOW yap
    SysCtlDelay(50000);  // LCD’nin baþlatýlmasý için gecikme

    // 4-bit mod baþlatmak için üç kez 0x30 komutu gönder
    otuzhexgonder();
    SysCtlDelay(50000);
    otuzhexgonder();
    SysCtlDelay(50000);
    otuzhexgonder();
    SysCtlDelay(50000);

    // 4-bit moda geçiþ komutu
    lcdkomut(0x02); // LCD'yi 4-bit modda baþlat
    SysCtlDelay(50000);

    // LCD ayar komutlarý
    lcdkomut(0x28); // 2 satýr, 5x7 karakter
    SysCtlDelay(50000);
    lcdkomut(0x0C); // Ekran açýk, imleç kapalý
    SysCtlDelay(50000);
    lcdkomut(0x06); // Ýmleci saða kaydýr
    SysCtlDelay(50000);

    // Ekraný temizle
    LCDTemizle();
    SysCtlDelay(50000);
}

void LCDTemizle() {
    lcdkomut(0x01); // Ekraný temizle komutu
    SysCtlDelay(2000);
}

void LCDgit(unsigned char row, unsigned char col) {
    unsigned char address;
    if (row == 0) {
        address = col;
    } else {
        address = 0x40 + col;
    }
    lcdkomut(0x80 | address);
}

void lcdyaz(const char *str) {
    while (*str) {
        lcdkarakteryaz(*str++);
    }
}

// LCD'ye sayýsal bir deðeri yazdýrmak için fonksiyon
void lcdSayiyaz(int value) {
    char buffer[16]; // Sayýyý yazmak için buffer
    sprintf(buffer, "%d", value); // Sayýyý stringe çevir
    lcdyaz(buffer); // Stringi LCD'ye yaz
}

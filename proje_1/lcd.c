#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.c"
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
    // LCD kontrol pinlerini çýkýþ olarak ayarla
    HWREG(SYSCTL_RCGCGPIO) |= SYSCTL_RCGCGPIO_R1; // Port B'yi etkinleþtir
    SysCtlDelay(160000); // Portun hazýr olmasý için bekle (yaklaþýk 15 ms)

    HWREG(GPIO_PORTB_BASE + GPIO_O_DIR) |= (RS | E | D4 | D5 | D6 | D7); // Çýkýþ pinleri
    HWREG(GPIO_PORTB_BASE + GPIO_O_DEN) |= (RS | E | D4 | D5 | D6 | D7); // Dijital etkinleþtir

    // LCD baþlatma komutlarý
    SysCtlDelay(160000); // Minimum 15 ms bekle
    lcdkomut(0x30);
    SysCtlDelay(8000);   // Minimum 4.1 ms bekle
    lcdkomut(0x30);
    SysCtlDelay(1000);   // Minimum 100 µs bekle
    lcdkomut(0x30);
    SysCtlDelay(1000);   // Kýsa bekleme

    // 4-bit moda geçiþ komutu
    lcdkomut(0x20);
    SysCtlDelay(1000); // Kýsa bekleme

    // Fonksiyon ayarlarý (2 satýr, 5x10 karakter)
    lcdkomut(0x3C); // 2 satýr, 5x10 piksel
    SysCtlDelay(1000);

    // Ekraný aç, imleci ve blink modunu aktif et
    lcdkomut(0x0F); // Ekran, imleç ve blink açýk
    SysCtlDelay(1000);

    // Ekraný temizle ve imleci baþa döndür
    LCDTemizle();
    SysCtlDelay(2000); // Temizleme komutundan sonra biraz daha uzun bekleme

    // Giriþ kipini ayarla (saða kaydýr)
    lcdkomut(0x06); // Giriþ Modu: Ýmleç saða kayar
    SysCtlDelay(1000);
}

void lcdfonksiyonayarla(unsigned char DL, unsigned char N, unsigned char F) {
    unsigned char cmd = 0x20 | (DL << 4) | (N << 3) | (F << 2);
    lcdkomut(cmd);
}

void LCDTemizle() {
    lcdkomut(0x01); // Ekraný temizle komutu
    SysCtlDelay(2000);
}

void LCDgit(unsigned char row, unsigned char col) {
    unsigned char address;
    if(row == 0) {
        address = col;
    } else {
        address = 0x40 + col;
    }
    lcdkomut(0x80 | address);
}

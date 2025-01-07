#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "lcd.h"

int main(void) {
    // Sistem saatini 50 MHz olarak ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // LCD ilk ayarlar�n� yap
    LCDilkayarlar();

    // LCD'nin �al��maya haz�r olmas� i�in bekleme
    SysCtlDelay(50000);  // Ba�lang�� gecikmesi

    // �mleci 1. sat�r, 7. s�tuna getir
    LCDgit(0, 6);

    // "a" karakterini yazd�r
    lcdkarakteryaz('a');

    // Sonsuz d�ng�
    while(1) {}

    return 0;
}

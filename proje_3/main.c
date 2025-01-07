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

    // LCD ilk ayarlarýný yap
    LCDilkayarlar();

    // LCD'nin çalýþmaya hazýr olmasý için bekleme
    SysCtlDelay(50000);  // Baþlangýç gecikmesi

    // Ýmleci 1. satýr, 7. sütuna getir
    LCDgit(0, 6);

    // "a" karakterini yazdýr
    lcdkarakteryaz('a');

    // Sonsuz döngü
    while(1) {}

    return 0;
}

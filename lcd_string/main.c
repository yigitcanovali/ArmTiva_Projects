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

int main(void) {
    // Sistem saatini 50 MHz olarak ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // LCD ilk ayarlar�n� yap
    LCDilkayarlar();

    // LCD'nin �al��maya haz�r olmas� i�in k�sa bir bekleme
    SysCtlDelay(50000);

    // �mleci ilk sat�r�n ilk s�tununa getir
    LCDgit(0, 0);

    // "Hello World" ifadesini LCD'ye yazd�r
    lcdyaz("yigitcanovali");

    // Sonsuz d�ng�
    while(1) {}

    return 0;
}

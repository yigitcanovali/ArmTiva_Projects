#include "stdint.h"
#include "stdbool.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "stdio.h"
#include "inc/hw_memmap.h"

// Fonksiyon prototipleri
void gecikme(int deger);
int degeral(uint32_t port, uint8_t pin);
void degerata(uint32_t port, uint8_t pin, uint8_t deger);
void surearttir(long *deger);
void sureazalt(long *deger);
void debounce(void);  // Debounce için eklenen fonksiyon

void main(void) // Ayarlamalarým
{
    uint32_t saatim;
    long deger = 20000000;

    // Saat ayarý ve GPIO F etkinleþtirme
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    saatim = SysCtlClockGet();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // GPIO yapýlandýrma
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    while(1)
    {
        // LED'i yak
        degerata(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02); // PF1 LED
        gecikme(deger);

        // LED'i söndür
        degerata(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
        gecikme(deger);

        // SW1 basýlý mý? (PF4)
        if(degeral(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
        {
            debounce();  // Debounce iþlemi
            if (degeral(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)  // Tekrar kontrol et
            {
                sureazalt(&deger); // Delay süresini azalt
            }
        }

        // SW2 basýlý mý? (PF0)
        if(degeral(GPIO_PORTF_BASE, GPIO_PIN_0) == 0)
        {
            debounce();  // Debounce iþlemi
            if (degeral(GPIO_PORTF_BASE, GPIO_PIN_0) == 0)  // Tekrar kontrol et
            {
                surearttir(&deger); // Delay süresini artýr
            }
        }
    }
}

// Gecikme fonksiyonu
void gecikme(int deger)
{
    SysCtlDelay(deger); // SysCtlDelay geri dönüþ deðeri olmayan bir fonksiyondur
}

// GPIO'dan deðer okuma
int degeral(uint32_t port, uint8_t pin)
{
    return GPIOPinRead(port, pin);
}

// GPIO'ya deðer yazma
void degerata(uint32_t port, uint8_t pin, uint8_t deger)
{
    GPIOPinWrite(port, pin, deger);
}

// Gecikme süresini artýr
void surearttir(long *deger)
{
    *deger += 10000000; // Delay süresini artýr
}

// Gecikme süresini azalt
void sureazalt(long *deger)
{
    if (*deger > 1000000)
    {
        *deger -= 10000000; // Delay süresini azalt
    }
}

// Debounce iþlemi (basit gecikme ile)
void debounce(void)
{
    SysCtlDelay(160000);  // Kýsa bir gecikme (~10ms) titreþimleri önler
}

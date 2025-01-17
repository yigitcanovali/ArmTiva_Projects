#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/sysctl.c"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/fpu.h"
#include <math.h>


void setinitsettings();
void saatkontrol();
void pcyesaatgonder();
void adcseridengonder(int deger);
void singonder();

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SERIES_LENGTH 100

float gSeriesData[SERIES_LENGTH];

int32_t i32DataCount = 0;


int saatdizi[8]={1,3,10,4,5,10,3,5};
int timerkesmesi;
uint32_t adcdata[4];

int gelenveri; // seriportttan
bool saatmi=false;
int gelensaatno=0;
int main(void)
{
    setinitsettings();

    while(1)
    {
        timerkesmesi=TimerIntStatus(TIMER0_BASE, true);

        if (timerkesmesi!=0){ // burada 1 sn oldu
            TimerIntClear(TIMER0_BASE, timerkesmesi);
            // 1 sn gemitir
            if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)){
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0);
            } else {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,2);
            }
            saatkontrol(); // saat guncellendi
            pcyesaatgonder(); // saat gitti
            ADCProcessorTrigger(ADC0_BASE, 1);
        }

        if(ADCIntStatus(ADC0_BASE, 1, false)){
            ADCIntClear(ADC0_BASE, 1);
            ADCSequenceDataGet(ADC0_BASE, 1, adcdata);
            uint32_t ortalamadeger=(adcdata[0]+adcdata[1]+adcdata[2]+adcdata[3])/4;
            adcseridengonder(ortalamadeger);
        }

        if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
        {
            UARTCharPut(UART0_BASE, '{'); //adc gidiuo
            UARTCharPut(UART0_BASE, 'b');
            UARTCharPut(UART0_BASE, 'o');
            UARTCharPut(UART0_BASE, 'b');
            UARTCharPut(UART0_BASE, 'b');
            UARTCharPut(UART0_BASE, '}'); //adc gidiuo
            UARTCharPut(UART0_BASE, '\n');
        }

        if (UARTCharsAvail(UART0_BASE)){
            gelenveri=UARTCharGet(UART0_BASE);
            // %12:23:45 - ahmet1
            if (gelenveri=='%'){
                // saat gelecek 8 tane gelecek
                saatmi=true;
                gelensaatno=0;
                TimerDisable(TIMER0_BASE, TIMER_A);
            }

            else if (saatmi==true){
                saatdizi[gelensaatno]=gelenveri-48;
                gelensaatno++;
                if (gelensaatno==8){
                    saatmi=false;
                    gelensaatno=0;
                    TimerEnable(TIMER0_BASE, TIMER_A);
                }
            }
        }

        singonder();
    }
}

void adcseridengonder(int deger){
    // 5, 25, 345, 2345
    char hane3;
    char hane2;
    char hane1;
    char hane0;

    if (deger<10){
        hane3=0; hane2=0; hane1=0; hane0=deger;
    } else if (deger<100) {
        hane3=0; hane2=0; hane0=(deger % 10); hane1=(deger-hane0)/10;
    } else if (deger<1000) {
        hane3=0; hane0=(deger % 10); hane1=((deger-hane0)/10)%10; hane2=(deger-hane1*10-hane0*1)/100;
    } else {
        hane0=(deger % 10); hane1=((deger-hane0)/10) % 10; hane2=((deger-10*hane1-1*hane0)%100);
        hane3=(deger-100*hane2-10*hane1-1*hane0)/1000;
    }

    UARTCharPut(UART0_BASE, '('); //adc gidiuo
    UARTCharPut(UART0_BASE, hane3+48);
    UARTCharPut(UART0_BASE, hane2+48);
    UARTCharPut(UART0_BASE, hane1+48);
    UARTCharPut(UART0_BASE, hane0+48);
    UARTCharPut(UART0_BASE, ')'); //adc gidiuo
    UARTCharPut(UART0_BASE, '\n');


}
void  pcyesaatgonder(){
    int i;
    UARTCharPut(UART0_BASE, '[');
    for (i=0;i<8;i++){
        UARTCharPut(UART0_BASE, saatdizi[i]+48);
    }
    UARTCharPut(UART0_BASE, '\n');
}
void saatkontrol(){
  int sa=saatdizi[0]*10+saatdizi[1];
  int dk=saatdizi[3]*10+saatdizi[4];
  int sn=saatdizi[6]*10+saatdizi[7];

  sn++;
  if (sn==60)
  {
    sn=0;
    dk++;
    if (dk==60){
        dk=0;
        sa++;
        if (sa==24){
            sa=0;
        }
    }
  }

  // 15
  saatdizi[1]=sa % 10;
  saatdizi[0]=(sa-saatdizi[1])/10;

  saatdizi[4]=dk % 10;
  saatdizi[3]=(dk-saatdizi[4])/10;

  saatdizi[7]=sn % 10;
  saatdizi[6]=(sn-saatdizi[7])/10;


}

void singonder(){

    char hane3;
    char hane2;
    char hane1;
    char hane0;

    float fRadians;
    int gSeriesData[SERIES_LENGTH];
    fRadians = ((2 * M_PI) / SERIES_LENGTH);
    while(i32DataCount < SERIES_LENGTH)
    {
        gSeriesData[i32DataCount] = (int)(sinf(fRadians * i32DataCount)*1000)+1000;
        i32DataCount++;
    }
    int i,deger;

    for (i=0;i<100;i++){
        deger=gSeriesData[i];
        if (deger<10){
            hane3=0; hane2=0; hane1=0; hane0=deger;
        } else if (deger<100) {
            hane3=0; hane2=0; hane0=(deger % 10); hane1=(deger-hane0)/10;
        } else if (deger<1000) {
            hane3=0; hane0=(deger % 10); hane1=((deger-hane0)/10)%10; hane2=(deger-hane1*10-hane0*1)/100;
        } else {
            hane0=(deger % 10); hane1=((deger-hane0)/10) % 10; hane2=((deger-10*hane1-1*hane0)%100);
            hane3=(deger-100*hane2-10*hane1-1*hane0)/1000;
        }
        SysCtlDelay(10000);
        UARTCharPut(UART0_BASE, '*'); //adc gidiuo
        UARTCharPut(UART0_BASE, hane3+48);
        UARTCharPut(UART0_BASE, hane2+48);
        UARTCharPut(UART0_BASE, hane1+48);
        UARTCharPut(UART0_BASE, hane0+48);
        UARTCharPut(UART0_BASE, '\n');
    }
}

void setinitsettings(){

      FPULazyStackingEnable();
      FPUEnable();
      SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
      GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
      HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
      HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;

      GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4); //pin0 ve pin4 input
      GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU); // pin0 ve pin4 pull-up
      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);

      SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
      TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC); // down sayc
      TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()); // 40 000 000 1 sn iin

      TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
      TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // bayrak temizlendi

      SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

      // sadece hangi pinleri uart olarak kullanacagz
      GPIOPinConfigure(GPIO_PA0_U0RX);
      GPIOPinConfigure(GPIO_PA1_U0TX);
      GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1);

      UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                          UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
      UARTEnable(UART0_BASE);

      SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // ADC HAYAT=1

      ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0); /// adc0 kullan ve sequence 1 se

      ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS); // scaklk sensr seildi
      ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
      ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
      ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
      ADCSequenceEnable(ADC0_BASE, 1); // adc ayarlanmas bitti
      TimerEnable(TIMER0_BASE, TIMER_A); // setb tr0
}


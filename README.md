# ArmTiva_Projects
 This repository contains projects developed using the Tiva LaunchPad. The projects demonstrate various applications and functionalities implemented with the Tiva C Series microcontroller platform.
 
## lcd_string:
**Türkçe:** LCD üzerine string veri yazdırma uygulaması.  
**İngilizce:** An application to display string data on an LCD.

## gecikme_kontrolu:
**Türkçe:** SW1 ve SW2 kullanılarak kırmızı LED’in yanıp sönme süresini artırıp azaltma uygulaması.  
**İngilizce:** An application to increase or decrease the blinking duration of a red LED using SW1 and SW2.

## buton_led_kontrolu:
**Türkçe:** SW1 ve SW2 kullanarak kırmızı ve yeşil LED’leri aktif etme.  
**İngilizce:** An application to activate red and green LEDs using SW1 and SW2.
# Proje Açıklamaları / Project Descriptions

## **adccalisma**
### Türkçe
- **Açıklama:** ADC (Analog-Digital Converter) kullanımı ile bir sensörden veri okuma işlemi gerçekleştirilmiştir. ADC'nin tetiklenmesi, kesme ile kontrol edilmiştir.
### English
- **Description:** An ADC (Analog-Digital Converter) is used to read data from a sensor. The ADC is triggered and controlled using interrupts.
  
## **apisizcalisma**
### Türkçe
- **Açıklama:** API kullanılmadan, manuel donanım ayarları yapılarak I2C haberleşmesi gerçekleştirilmiştir. Hem veri okuma hem yazma işlemleri yapılmıştır.
### English
- **Description:** I2C communication is implemented manually without using APIs. Both data reading and writing operations are performed.
  
## **pwm.1**
### Türkçe
- **Açıklama:** PWM (Pulse Width Modulation) kullanılarak bir LED'in parlaklığını değiştirme işlemi yapılmıştır. PWM sinyali, belirli bir periyotta ve değişken duty cycle ile oluşturulmuştur.
### English
- **Description:** PWM (Pulse Width Modulation) is used to adjust the brightness of an LED. A PWM signal with a specific period and variable duty cycle is generated.
  
##  **seriportdenemesi**
### Türkçe
- **Açıklama:** Mikrodenetleyici ile seri port haberleşmesi sağlanmıştır. UART kullanılarak bilgisayar ve mikrodenetleyici arasında veri alışverişi gerçekleştirilmiştir.
### English
- **Description:** Serial port communication is established with the microcontroller. Data exchange between the microcontroller and the computer is performed using UART.
  
## **timerkesmeliapili**
### Türkçe
- **Açıklama:** Timer modülü ve kesme (interrupt) mekanizması kullanılarak bir LED'in periyodik olarak yanıp sönmesi sağlanmıştır. API fonksiyonları ile kolaylık sağlanmıştır.
### English
- **Description:** A timer module and interrupt mechanism are used to toggle an LED periodically. APIs are utilized for easier implementation.
  
## **apisizsaat.1**
### Türkçe
- **Açıklama:** Mikrodenetleyicinin saat ayarları manuel olarak yapılandırılmış ve bir saat modülü oluşturulmuştur. API kullanılmadan RCC ve PLL doğrudan kontrol edilmiştir.
### English
- **Description:** The microcontroller's clock settings are manually configured, and a clock module is implemented. RCC and PLL are controlled directly without using APIs.

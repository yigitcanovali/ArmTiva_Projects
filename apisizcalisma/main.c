#include "stdint.h"
#include "stdio.h"
#include "inc/tm4c123gh6pm.h"

char data[3] = {65, 66, 67};

void init_i2c();
uint8_t Read();
void Write(uint8_t data);
void WriteMulti(char datam[], int adet);

uint8_t Read() {
    I2C0_MSA_R = (0x55 << 1) + 0; // Address + write
    I2C0_MCS_R = 0x07; // Stop, start, run
    while (I2C0_MCS_R & 0x40); // Poll busy bit

    I2C0_MSA_R = (0x55 << 1) + 1; // Address + read
    I2C0_MCS_R = 0x07; // Start run
    while (I2C0_MCS_R & 0x40); // Poll busy bit

    return I2C0_MDR_R;
}

void Write(uint8_t data) {
    I2C0_MSA_R = (0x55 << 1) + 0; // Address + write
    I2C0_MDR_R = data;
    I2C0_MCS_R = 0x07; // Stop, start, run
    while (I2C0_MCS_R & 0x40); // Poll busy bit
}

void WriteMulti(char datam[], int adet) {
    I2C0_MSA_R = (0x55 << 1) + 0; // Address + write

    I2C0_MDR_R = 'K'; // Start signal
    I2C0_MCS_R = 0x03; // Start, run
    while (I2C0_MCS_R & 0x40); // Poll busy bit

    int i;
    for (i = 0; i < adet; i++) {
        I2C0_MDR_R = datam[i];
        if (i == adet - 1) {
            I2C0_MCS_R = 0x07; // Stop, start, run
        } else {
            I2C0_MCS_R = 0x01; // Continue
        }
        while (I2C0_MCS_R & 0x40); // Poll busy bit
    }
}

int main() {
    init_i2c();
    WriteMulti(data, 3);
    Write('A');
    uint8_t receivedValue = Read();

    // Optionally print received value (for debug)
    printf("Received: %d\n", receivedValue);

    return 0;
}

void init_i2c() {
    // Enable I2C0 clock
    SYSCTL_RCGCI2C_R |= 0x01;

    // Enable GPIO Port B clock
    SYSCTL_RCGCGPIO_R |= 0x02;

    // Configure PB2 and PB3 for I2C
    GPIO_PORTB_DEN_R |= 0x0C;  // Enable digital for PB2 and PB3
    GPIO_PORTB_AFSEL_R |= 0x0C; // Enable alternate function for PB2 and PB3
    GPIO_PORTB_ODR_R |= 0x08; // Enable open-drain for PB3 (SDA)
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF00FF) | 0x00003300; // Assign I2C function to PB2 and PB3

    // Configure I2C Master
    I2C0_MCR_R = 0x10; // Enable I2C master mode

    // Set SCL clock speed to 100 Kbps
    I2C0_MTPR_R = 7; // TPR value for 100 Kbps
}

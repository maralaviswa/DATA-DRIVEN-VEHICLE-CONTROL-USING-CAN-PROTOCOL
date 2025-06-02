#include <LPC21xx.H>
#include "delay.h"
#include "lcd.h"
#include"DS18B20_defines.h"
#include"DS18B20.h"

// 1-Wire Reset
unsigned char ow_reset(void) {
    unsigned char presence;
    DS18B20_DIR |= DS18B20_PIN;   // Set as output
    DS18B20_CLR = DS18B20_PIN;    // Pull low
    delay_us(480);                // 480Âµs reset pulse
    DS18B20_DIR &= ~DS18B20_PIN;  // Set as input	/
    delay_us(70);                 // Wait 70Âµs
    presence = (DS18B20_PIN_REG & DS18B20_PIN) ? 0 : 1;
    delay_us(410);                // Complete timeslot
    return presence;
}

// Write a bit to the 1-Wire bus
void ow_write_bit(unsigned char bit) {
    DS18B20_DIR |= DS18B20_PIN;   // Set as output
    DS18B20_CLR = DS18B20_PIN;    // Pull low
    if(bit) {
        delay_us(6);              // 6Âµs for '1'
        DS18B20_DIR &= ~DS18B20_PIN;
        delay_us(64);
    } else {
        delay_us(60);             // 60Âµs for '0'
        DS18B20_DIR &= ~DS18B20_PIN;
        delay_us(10);
    }
}

// Read a bit from the 1-Wire bus
unsigned char ow_read_bit(void) {
    unsigned char bit;
    DS18B20_DIR |= DS18B20_PIN;   // Set as output
    DS18B20_CLR = DS18B20_PIN;    // Pull low
    delay_us(6);
    DS18B20_DIR &= ~DS18B20_PIN;  // Set as input
    delay_us(9);
    bit = (DS18B20_PIN_REG & DS18B20_PIN) ? 1 : 0;
    delay_us(55);
    return bit;
}

// Write a byte to the 1-Wire bus
void ow_write_byte(unsigned char byte) {
    unsigned char i;
    for(i=0; i<8; i++) {
        ow_write_bit(byte & 0x01);
        byte >>= 1;
    }
}

// Read a byte from the 1-Wire bus
unsigned char ow_read_byte(void) {
    unsigned char i, byte = 0;
    for(i=0; i<8; i++) {
        byte >>= 1;
        if(ow_read_bit()) byte |= 0x80;
    }
    return byte;
}

// Start temperature conversion
void ds18b20_start_convert(void) {
    ow_reset();
    ow_write_byte(SKIP_ROM);
    ow_write_byte(CONVERT_T);
}

// Read temperature from DS18B20
float ds18b20_read_temp(void) {
    unsigned char lsb, msb;
    short temp;
    
    if(!ow_reset()) return -1000.0f; // More distinct error value
    
    ow_write_byte(SKIP_ROM);
    ow_write_byte(READ_SCRATCHPAD);
    
    lsb = ow_read_byte();
    msb = ow_read_byte();
    
    // Combine bytes and convert to temperature
    temp = (msb << 8) | lsb;
    return (float)temp * 0.0625f;
}



#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#define EXT_INT0 14
#define EXT_INT0_PIN 16
#define EXT_INT0_LED 22


#define EXT_INT1 15
#define EXT_INT1_PIN 14
#define EXT_INT1_LED 23

#define EXT_INTO2 16
#define EXT_INT2_PIN 15
#define EXT_INT2_LED 21


void interupt_0(void) __irq;
void interupt_1(void) __irq;
void interupt_2(void) __irq;

void ini_INTERUPT(void);

#endif



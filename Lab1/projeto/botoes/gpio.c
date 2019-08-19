#include "gpio.h"


void GPIO_Init()
{

    // These configurations are set for each pin in
    // the ports J and N, that is, pins J0 and N0.

    // Activating ports J and N.
    SYSCTL_RCGCGPIO_R |= GPIO_JN;

    //1b.   ap?s isso verificar no PRGPIO se a porta est? pronta para uso.
    while ((SYSCTL_PRGPIO_R & (GPIO_JN)) != (GPIO_JN)) { }

    // Selecting GPIO with port control.
    GPIO_PORTJ_PCTL_R = 0x00;
    GPIO_PORTN_PCTL_R = 0x00;

    // Disabling analog mode.
    GPIO_PORTJ_AMSEL_R = 0x00;
    GPIO_PORTN_AMSEL_R = 0x00;

    // Disabling alternate function.
    GPIO_PORTJ_AFSEL_R = 0x00;
    GPIO_PORTN_AFSEL_R = 0x00;

    // Enabling digital mode.
    GPIO_PORTJ_DEN_R = 0x01;
    GPIO_PORTN_DEN_R = 0x01;

    // Setting port J as input
    GPIO_PORTJ_DIR_R = 0x00;

    // Setting port N as output
    GPIO_PORTN_DIR_R = 0x01;

    // Enabling internal pull-down resistor for port J
    GPIO_PORTJ_PUR_R = 0x01;



    // Global interrupt enable.
    EnableInterrupts();

    // Activates interrupts for port J.
    NVIC_EN1_R |= 0xFFF;

    NVIC_PRI12_R &= ~0xF0000000; 

    // Activate interrupt for pin J0.
    GPIO_PORTJ_SI_R = 0x01;

    // Sets interrupt trigger in edge mode.
    GPIO_PORTJ_IS_R &= ~0x01;    // 0b1111 1011

    // Sets single edge mode.
    GPIO_PORTJ_IBE_R &= ~0x01;

    // Sets rising edge mode.
    GPIO_PORTJ_IEV_R |= 0x01;

    // Set interrupt as not masked.
    GPIO_PORTJ_IM_R |= 0x01;
}


void GPIOJ_Handler(void)
{
    // Interrupt ACK.
    GPIO_PORTJ_ICR_R |= 0x01;

    static int counter = 0;

    counter++;
    if (counter > 5000)
    {
        counter = 0;
        writePortN(1);
    }
}

uint32_t readPortJ()
{
    return GPIO_PORTJ_DATA_R & 0x01;
}

void writePortN(uint32_t arg)
{
    int temp = GPIO_PORTN_DATA_R & 0xFFFFFFFE;
    temp |= (arg & 0x01);
    GPIO_PORTN_DATA_R = temp;
}

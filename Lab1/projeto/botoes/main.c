//=======================================================================================
/**
 * Authors:
 *  - Matheus Bigarelli
 *  - Victor Belinello
 * 
 *  Task 1 for Embedded Systems course from UTFPR - Curitiba.
*/
//=======================================================================================
#include <stdint.h>

#include "tm4c1294ncpdt.h"
#include "gpio.h"

extern void PLL_Init(void);
extern void Systick_Init(void);
extern void SysTick_Wait1ms(uint32_t time);

//---------------------------------------------------------------------------------------
// Function definitions
void initialize(void);

//=======================================================================================

// Main function - Initializes GPIOs
int main()
{
    initializeComponents();

    while(1)
    {
        SysTick_Wait1ms(1000);
        writePortN(0);
    }


    return 0;
}


void initializeComponents()
{
    PLL_Init();
    GPIO_Init();
    SysTick_Init();
    // displayInit();
}


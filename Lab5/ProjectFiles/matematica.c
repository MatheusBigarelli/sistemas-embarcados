#include "matematica.h"

double factorial(double n)
{
    uint32_t i;
    double fact = 1;
    for (i = 1; i <= n; ++i)
        fact *= i;

    return fact;
}

// 0x682 ticks
void ThreadA(const void *args)
{
    uint16_t x, sum;
    volatile uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (x = 0; x <= 256; x++)
        {
            sum += 2 * x + 2;
        }
        timeInTicks = osKernelSysTick() - initialTimeTick;
        threadYield();
    }
}

// 0x49AB ticks
void ThreadB(const void *args)
{
    double n, sum;
    volatile uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (n = 1; n <= 16; n++)
        {
            sum += (2 << (int)n) / factorial(n);
        }
        timeInTicks = osKernelSysTick() - initialTimeTick;
        threadYield();
    }
}

// 0x15F5 ticks
void ThreadC(const void *args)
{
    double n, sum;
    volatile uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (n = 1; n <= 72; n++)
        {
            sum += (n + 1) / n;
        }
        timeInTicks = osKernelSysTick() - initialTimeTick;
        threadYield();
    }
}

// 0xCF0 ticks
void ThreadD(const void *args)
{
    double sum;
    volatile uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 1 + 5 / factorial(3) + 5 / factorial(5) + 5 / factorial(7) + 5 / factorial(9);
        timeInTicks = osKernelSysTick() - initialTimeTick;
        threadYield();
    }
}

// 0x1E29 ticks
void ThreadE(const void *args)
{
    double x, sum;
    volatile uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (x = 1; x <= 100; x++)
        {
            sum += x * (3.14159265358979323846 * 3.14159265358979323846);
        }
        timeInTicks = osKernelSysTick() - initialTimeTick;
        threadYield();
    }
}

// 0x2829 ticks
void ThreadF(const void *args)
{
    double y, sum;
    volatile uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (y = 0; y <= 128; y++)
        {
            sum += y*y*y / (1 << (int)y);
        }
        timeInTicks = osKernelSysTick() - initialTimeTick;
        threadYield();
    }
}
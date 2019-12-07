#include "matematica.h"

double factorial(double n)
{
    uint32_t i;
    double fact = 1;
    for (i = 1; i <= n; ++i)
        fact *= i;

    return fact;
}

void ThreadA(const void *args)
{
    uint16_t x, sum;
    uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (x = 0; x <= 256; x++)
        {
            sum += 2 * x + 2;
            timeInTicks = osKernelSysTick() - initialTimeTick;
            osThreadYield();
        }
    }
}

void ThreadB(const void *args)
{
    double n, sum;
    uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (n = 1; n <= 16; n++)
        {
            sum += (2 << (int)n) / factorial(n);
            timeInTicks = osKernelSysTick() - initialTimeTick;
            osThreadYield();
        }
    }
}

void ThreadC(const void *args)
{
    double n, sum;
    uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (n = 1; n <= 72; n++)
        {
            sum += (n + 1) / n;
            timeInTicks = osKernelSysTick() - initialTimeTick;
            osThreadYield();
        }
    }
}

void ThreadD(const void *args)
{
    double sum;
    uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 1 + 5 / factorial(3) + 5 / factorial(5) + 5 / factorial(7) + 5 / factorial(9);
        timeInTicks = osKernelSysTick() - initialTimeTick;
        osThreadYield();
    }
}

void ThreadE(const void *args)
{
    double x, sum;
    uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (x = 1; x <= 100; x++)
        {
            sum += x * pow(3.14159265358979323846, 2);
            timeInTicks = osKernelSysTick() - initialTimeTick;
            osThreadYield();
        }
    }
}

void ThreadF(const void *args)
{
    double y, sum;
    uint32_t initialTimeTick = osKernelSysTick(), timeInTicks;
    while (true)
    {
        sum = 0;
        for (y = 0; y <= 128; y++)
        {
            sum += pow(y, 3) / (1 << (int)y);
            timeInTicks = osKernelSysTick() - initialTimeTick;
            osThreadYield();
        }
    }
}
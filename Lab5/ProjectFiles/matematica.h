#ifndef __MATEMATICA_H__
#define __MATEMATICA_H__

#include "util.h"

#include <math.h>

void ThreadA(const void *args);
void ThreadB(const void *args);
void ThreadC(const void *args);
void ThreadD(const void *args);
void ThreadE(const void *args);
void ThreadF(const void *args);
double factorial(double n);

#endif
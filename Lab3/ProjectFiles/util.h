#ifndef __UTIL__H
#define __UTIL__H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "cmsis_os.h"

void flipVert(uint32_t image[], uint16_t rows, uint16_t columns);
void intToString(int64_t value, char * pBuf, uint32_t len, uint32_t base, uint8_t zeros);

// Definições das threads
void Entrada(void const *arg);
void Eddie(void const *arg);
void Inimigos(void const *arg);
void ItensBrilhantes(void const *arg);
void PainelDeInstrumentos(void const *arg);
void Saida(void const *arg);


#endif

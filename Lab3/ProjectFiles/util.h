#ifndef __UTIL__H
#define __UTIL__H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "cmsis_os.h"

#define JOY_SENSITIVITY_X 50
#define JOY_SENSITIVITY_Y 90

#define JUMP_LIMIT 6

void flipVert(uint8_t image[], uint16_t rows, uint16_t columns);
void intToString(int64_t value, char * pBuf, uint32_t len, uint32_t base, uint8_t zeros);



// Defini��es das threads
void Entrada(void const *arg);
void Eddie(void const *arg);
void Inimigos(void const *arg);
void ItensBrilhantes(void const *arg);
void PainelDeInstrumentos(void const *arg);
void Saida(void const *arg);


#endif

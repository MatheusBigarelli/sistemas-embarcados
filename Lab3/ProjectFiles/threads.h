#ifndef __THREADS__H
#define __THREADS__H
#include "draw.h"
#include "joy.h"
#include "util.h"

#define SIGNAL_INPUT_READY 			0x000001
#define SIGNAL_EDDIE_READY 			0x000010
#define SIGNAL_ENEMIES_READY 		0x000100
#define SIGNAL_ITENS_READY 			0x001000
#define SIGNAL_BACKGROUND_READY 0x010000
#define SIGNAL_OUTPUT_READY 		0x100000

#define SIGNAL_EDDIE_LADDER			0x200000

// Definicoes das threads
void Entrada(void const *arg);
void Eddie(void const *arg);
void Inimigos(void const *arg);
void ItensBrilhantes(void const *arg);
void PainelDeInstrumentos(void const *arg);
void Saida(void const *arg);



#endif
#ifndef __UTIL__H
#define __UTIL__H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "cmsis_os.h"


#define FREQ_C  262
#define FREQ_Db 277
#define FREQ_D  294
#define FREQ_Eb 311
#define FREQ_E  330
#define FREQ_F  349
#define FREQ_Gb 392
#define FREQ_G  392
#define FREQ_Ab 415
#define FREQ_A  440
#define FREQ_Bb 466
#define FREQ_B  494

#define hzToPeriod(f) (120000000/64/f)

#define NOTE_C  hzToPeriod(FREQ_C)
#define NOTE_Db  hzToPeriod(FREQ_Db)
#define NOTE_D  hzToPeriod(FREQ_D)
#define NOTE_Eb hzToPeriod(FREQ_Eb)
#define NOTE_E  hzToPeriod(FREQ_E)
#define NOTE_F  hzToPeriod(FREQ_F)
#define NOTE_Gb hzToPeriod(FREQ_Gb)
#define NOTE_G  hzToPeriod(FREQ_G)
#define NOTE_Ab hzToPeriod(FREQ_Ab)
#define NOTE_A  hzToPeriod(FREQ_A)
#define NOTE_Bb hzToPeriod(FREQ_Bb)
#define NOTE_B  hzToPeriod(FREQ_B)

#define sC(X) (NOTE_C/X)
#define sDb(X) (NOTE_Db/X)
#define sD(X) (NOTE_D/X)
#define sEb(X) (NOTE_Eb/X)
#define sE(X) (NOTE_E/X)
#define sF(X) (NOTE_F/X)
#define sGb(X) (NOTE_Gb/X)
#define sG(X) (NOTE_G/X)
#define sAb(X) (NOTE_Ab/X)
#define sA(X) (NOTE_A/X)
#define sBb(X) (NOTE_Bb/X)
#define sB(X) (NOTE_B/X)

#define SONG_SIZE 50


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

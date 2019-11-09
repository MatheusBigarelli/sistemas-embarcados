#ifndef __DISPLAY__
#define __DISPLAY__

#include <math.h>

#include "util.h"

#include "grlib/grlib.h"

#include "cfaf128x128x16.h"


#define LINE_FREQUENCY 1
#define LINE_AMPLITUDE 2
#define LINE_BASE 30
#define LINE_OFFSET(X) (LINE_BASE + 24*(X-1))


// Definimos um quadrado de 40x40
// para imprimir as ondas
#define WAVE_DISPLAY_X_START 70
#define WAVE_DISPLAY_X_END 110
#define WAVE_DISPLAY_WIDTH (WAVE_DISPLAY_X_END - WAVE_DISPLAY_X_START)

#define WAVE_DISPLAY_Y_START 40
#define WAVE_DISPLAY_Y_END 80
#define WAVE_DISPLAY_HEIGHT (WAVE_DISPLAY_Y_END - WAVE_DISPLAY_Y_START)

extern void displayInit(tContext *sContext);
extern void printHeader(tContext *sContext);
extern void printFrequency(tContext *sContext, float frequency);
extern void printAmplitude(tContext* sContext, float frequency);

extern void floatToString(float value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros, uint8_t precision);
extern void intToString(int64_t value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros);


extern void drawWaveform(tContext* sContext, uint8_t waveform);
extern void drawSine(tContext *sContext);
extern void drawSine(tContext *sContext);
extern void drawTriangle(tContext *sContext);
extern void drawTrapeze(tContext *sContext);
extern void drawSquare(tContext *sContext);
extern void drawSaw(tContext *sContext);
extern void plot(tContext *sContext, uint8_t x[WAVE_DISPLAY_WIDTH], uint8_t y[WAVE_DISPLAY_HEIGHT]);

#endif

#ifndef __TITLE__H
#define __TITLE__H

#include "cmsis_os.h"
#include "TM4C129.h" // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"

#define TITLE_WIDTH 39
#define TITLE_HEIGHT 9

#define TITLE_PIXELS (TITLE_WIDTH*TITLE_HEIGHT)

void drawTitle(void);
void Title(void);

#endif
#ifndef __PAINEL__H
#define __PAINEL__H

#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"

#include "cfaf128x128x16.h"

#include "ladder.h"
#include "floor.h"

#define TITLE_SIG_BASE 0x00
#define FLOOR_SIG_BASE 0x01 << 24

#endif
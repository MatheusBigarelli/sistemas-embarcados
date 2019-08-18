//=======================================================================================
/**
 * Authors:
 *  - Matheus Bigarelli
 *  - Victor Belinello
 * 
 *  Task 1 for Embedded Systems course from UTFPR - Curitiba.
*/
//=======================================================================================

#ifndef __DISPLAY__
#define __DISPLAY__

#include "cfaf128x128x16.h"

//=======================================================================================
// Function definitions
void displayInit();
void displayImage();
void invertColors();
void switchImages();

#endif
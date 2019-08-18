//=======================================================================================
/**
 * Authors:
 *  - Matheus Bigarelli
 *  - Victor Belinello
 * 
 *  Task 1 for Embedded Systems course from UTFPR - Curitiba.
*/
//=======================================================================================
#include <stdint.h>

#include "tm4c1294ncpdt.h";

//---------------------------------------------------------------------------------------
// Function definitions
void initialize();
void wait(float time);
void imageResize();

extern void imageResizeASM(uint32_t direction);

//---------------------------------------------------------------------------------------

// Image is char[row][column][pixelColor].
// pixelColor is one of RGB.
typedef Image char ***;

// Global variables
Image images[2];
Image* current_image;

//=======================================================================================

// Main function - Initializes GPIOs
int main()
{
    initializeComponents();

    while (1)
    {
        displayImage();
        wait(0.5);
        imageResize();
    }

    return 0;
}


void initializeComponents()
{
    PLLInit();
    GPIOInit();
    displayInit();
}



void displayImage()
{

}


void wait(uint32_t time)
{

}


/**
 * Function to take care of image resizing.
 * It will not take any parameters, so it will keep a static state variable
 * to track the size of the image that is beeing displayed. 
*/
void imageResize()
{
    // Power of 2 that is currently multiplying the size of the image.
    // Min: -5 (32)
    // Max: 5 (1/32)
    static int power = 0;
    // Direction that the image is going: expanding (0) or shrinking (1).
    static uint32_t direction = 0;

    // Image is getting bigger.
    if (!direction)
    {
        if (power < 5)
        {
            imageResizeASM(direction);
        }
        else
        {
            power = 4;
            direction = 1;
        }

        power++;
    }

    else
    {
        
        power--;
    }
    

}


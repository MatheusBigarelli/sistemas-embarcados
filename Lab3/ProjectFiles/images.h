#ifndef __IMAGES__H
#define __IMAGES__H

#include <stdint.h>
// As imagens estao "normalizadas" para economizar espaco
// O que esta armazenado sao os codigos das cores, por exemplo 7 indica a cor do chapeu do eddie, enquanto 8 indica a cor do seu corpo
// Assim podemos usar 3bits para representar a cor, como o menor valor � 8bits em C usamos isso.


/*EDDIE IMAGES*/

#define EDDIE_HAT_HEIGHT 2
#define EDDIE_HAT_WIDTH 10
const uint8_t eddie_hat[] = {
0,0,0,7,7,7,7,0,0,0,
0,7,7,7,7,7,7,7,7,0,};
#define EDDIE_BODY1_HEIGHT 3
#define EDDIE_BODY1_WIDTH 10
const uint8_t eddie_body1[] = {
0,0,0,8,8,8,8,0,0,0,
0,0,0,8,8,8,8,8,8,0,
0,0,0,8,8,8,8,0,0,0,
};
#define EDDIE_BODY2_HEIGHT 3
#define EDDIE_BODY2_WIDTH 10
const uint8_t eddie_body2[] = {
0,0,0,8,8,8,8,0,0,0,
0,0,0,8,8,8,8,0,0,0,
0,0,0,8,8,8,8,8,8,8};
#define EDDIE_SHIRT_HEIGHT 8
#define EDDIE_SHIRT_WIDTH 10
const uint8_t eddie_shirt[] = {
0,0,6,6,6,6,6,6,0,0,
6,6,6,6,0,0,6,6,6,6,
6,6,6,6,0,0,6,6,6,6,
6,6,6,6,0,0,6,6,6,6,
6,6,6,6,0,0,0,0,0,0,
6,6,6,6,6,0,0,0,0,0,
6,6,6,6,6,6,6,6,6,6,
0,0,6,6,6,6,6,6,0,0};
#define EDDIE_HEIGHT 16

/*SNEAKER IMAGES*/
#define  HEAD_HEIGHT 4
#define  HEAD_WIDTH 11
#define HEAD_NUMBER_PIXELS HEAD_HEIGHT*HEAD_WIDTH
const uint8_t head[] = {
0,0,5,5,5,5,5,5,5,0,0,
0,5,5,0,0,5,0,0,5,5,0,
5,5,5,0,0,5,0,0,5,5,5,
5,5,5,5,5,5,5,5,5,5,5};

#define LEGS_HEIGHT 3
#define LEGS_WIDTH 11
#define LEGS_NUMBER_PIXELS LEGS_HEIGHT*LEGS_WIDTH
const uint8_t legs[] = {
0,0,0,4,0,0,0,4,0,0,0,
0,0,0,4,0,0,0,4,4,4,4,
4,4,4,4,0,0,0,0,0,0,0};

#define LEGS_EXTRA_HEIGHT 1
#define LEGS_EXTRA_WIDTH 11
#define LEGS_EXTRA_NUMBER_PIXELS LEGS_EXTRA_HEIGHT*LEGS_EXTRA_WIDTH
const uint8_t legsExtra[] = {
0,0,0,4,0,0,0,4,0,0,0
};


/*ITEM IMAGE*/
#define ITEM_HEIGHT 5
#define ITEM_WIDTH 7
#define ITEM_NUMBER_PIXELS ITEM_HEIGHT*ITEM_WIDTH

const uint8_t item[] = {
0,3,3,0,3,3,0,
3,3,3,3,3,3,3,
0,3,3,3,3,3,0,
0,0,3,3,3,0,0,
0,0,0,3,0,0,0
}; 


/*LADDER IMAGE*/
#define LADDER_HEIGHT 21
#define LADDER_WIDTH 22
#define LADDER_NUMBER_PIXELS LADDER_HEIGHT*LADDER_WIDTH
const uint8_t ladder[] = {
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1};


/*FLOOR IMAGE*/
#define FLOOR_WIDTH 128
#define FLOOR_HEIGHT 3
#define FLOOR_NUMBER_PIXELS FLOOR_WIDTH*FLOOR_HEIGHT
const uint8_t floor_[] = {
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,      
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,      
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

#endif
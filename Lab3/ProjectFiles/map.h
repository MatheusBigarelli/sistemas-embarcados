#ifndef __MAP__H
#define __MAP__H

#define MAP_HEIGHT 128
#define MAP_WIDTH 128


#define PRIORITY_EMPTY 0
#define PRIORITY_BACKGROUND 1
#define PRIORITY_EDDIE 2
#define PRIORITY_ITEM 3
#define PRIORITY_SNEAKER 4



// buffer com 2 niveis de profundidade, guarda o pixel atual e o anterior
unsigned char buffer[2][MAP_HEIGHT][MAP_WIDTH]; 

#endif
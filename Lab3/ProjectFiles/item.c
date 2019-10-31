#include "draw.h"

extern osMutexId mid_displayMutex;

extern uint8_t item[];
extern uint8_t zero[];
extern uint8_t one[];
extern uint8_t two[];
extern uint8_t three[];
extern uint8_t four[];
extern uint8_t five[];
extern uint8_t six[];
extern uint8_t seven[];
extern uint8_t eight[];
extern uint8_t nine[];

extern uint32_t palette[9];

extern char buffer[2][MAP_HEIGHT][MAP_WIDTH];

extern Image item1;
extern Image item2;
Image* itemBeingCleared;
int previousScoreX,previousScoreY;
extern Image ladderImage;
bool shouldClearPreviousScore = false;

extern uint16_t currentScore;
extern uint16_t totalScore;
extern bool clearBoss;

void clearPreviousScore()
{
	int i,j;
	ColorIndex colorOnPreviousFrame;
	if(shouldClearPreviousScore == false)
	{
		return;
	}
	
	for (i = 0; i < ITEM_SCORE_HEIGHT; i++)
	{
		for (j = 0; j < ITEM_SCORE_WIDTH*2; j++)
		{
				colorOnPreviousFrame = buffer[1][i + previousScoreY][j + previousScoreX];
				GrContextForegroundSet(&sContext, palette[colorOnPreviousFrame]);
				GrPixelDraw(&sContext, previousScoreX + j, i + previousScoreY);
				buffer[0][i + previousScoreY][j + previousScoreX] = colorOnPreviousFrame;
		}
	}
	currentScore+= 10;
	if(currentScore == 50)
	{
		currentScore = 50;
	}
	totalScore += currentScore;
	clearBoss = true;
	shouldClearPreviousScore = false;
}
void replaceItem(void const* arg) // Callback do timer_item
{
	int k;
	uint16_t otherAreaOffset;
	uint16_t thisAreaOffset = itemBeingCleared->areaOffset;
	
	
	
<<<<<<< HEAD
	if (item.last_x < item.x)
	{
		for (i = 0; i < ITEM_HEIGHT; i++)
		{
			GrPixelDraw(&sContext,(item.last_x)%128,(i+item.last_y)%128);
=======
	previousScoreY = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT) * thisAreaOffset;
	previousScoreX = itemBeingCleared->x;
	shouldClearPreviousScore = true;
	itemBeingCleared->x = 0;
	if(item1.areaOffset == thisAreaOffset) // This eh o item1
	{
		otherAreaOffset = item2.areaOffset;
	}
	else // This eh o item2
	{
		otherAreaOffset = item1.areaOffset;
	}
	for (k = -NUMBER_OF_AREAS + 1; k < NUMBER_OF_AREAS; k++)
	{
		if(k == 0) continue;
		if(thisAreaOffset + k != otherAreaOffset && thisAreaOffset + k < NUMBER_OF_AREAS && thisAreaOffset + k >= 0)
		{
			itemBeingCleared->areaOffset += k;
			break;
>>>>>>> biga
		}
	}	
	itemBeingCleared->collected = false;	
	itemBeingCleared->isMoving = true;
	itemBeingCleared->needsUpdate = true;
}

extern osTimerId timer_item;
void collectItem(Image* itemCollected)
{
	itemCollected->needsUpdate = false;
	itemCollected->isMoving = false;
	itemCollected->collected = true;
	if(itemCollected->dirX == RIGHT)
	{
		itemCollected->x--;
		clear(*itemCollected);
		itemCollected->x++;
	}
	else
	{
<<<<<<< HEAD
		for (i = 0; i < ITEM_HEIGHT; i++)
		{
			GrPixelDraw(&sContext,((ITEM_WIDTH-1)+item.last_x)%128,(i+item.last_y)%128);
=======
		itemCollected->x++;
		clear(*itemCollected);
		itemCollected->x--;
	}
	itemBeingCleared = itemCollected;
	osTimerStart(timer_item,3000*5);
}
void updateItens()
{
	if(item1.x == 0)
	{
		if(item1.dirX == RIGHT)
		{
			item1.x--;
			clear(item1);
			item1.x++;
		}
		else
		{
			item1.x++;
			clear(item1);
			item1.x--;
>>>>>>> biga
		}
		
		item1.x = 127;
	}
	if(item1.x == 128)
	{
		item1.x = 0;
	}
	if(item1.dirX == RIGHT)
	{
		item1.x++;
	}
	if(item1.dirX == LEFT)
	{
		item1.x--;
	}
	if(item2.x == 0)
	{
		if(item2.dirX == RIGHT)
		{
			item2.x--;
			clear(item2);
			item1.x++;
		}
		else
		{
			item2.x++;
			clear(item2);
			item1.x--;
		}
		
		item2.x = 127;
	}
	if(item2.x == 128)
	{
		item2.x = 0;
	}
	if(item2.dirX == RIGHT)
	{
		item2.x++;
	}
	if(item2.dirX == LEFT)
	{
		item2.x--;
	}
<<<<<<< HEAD
	
    for (i = 0; i < ITEM_HEIGHT; i++)
    {
        for (j = 0; j < ITEM_WIDTH; j++)
        {
			foreground = itemOneChannel[i*ITEM_WIDTH + j];
			if (foreground > 5)
			{
				GrContextForegroundSet(&sContext, foreground);
				GrPixelDraw(&sContext,(j+item.x)%128,(i+item.y)%128);
			}
        }
    }
}


void ItemThread(void const *args)
{
	uint8_t i, j;
	int16_t ds;
	for (i = 0; i < NUM_ITENS; i++)
	{
		itens[i].x = rand() % (128-ITEM_WIDTH);
		itens[i].y = ITEM_Y(FLOOR(i+2));
		itens[i].speed = rand()%3 - 1;
		itens[i].last_x = itens[i].x;
		itens[i].last_y = itens[i].y;
		itens[i].glow_state[0] = 0;
		itens[i].glow_state[1] = 40;
		itens[i].glow_state[2] = 80;
	}
	
	while(1)
	{
		for (i = 0; i < NUM_ITENS; i++)
		{
			osMutexWait(context_mutex, osWaitForever);
			drawItem(itens[i]);
			osMutexRelease(context_mutex);
	
			itens[i].last_x = itens[i].x;
			itens[i].last_y = itens[i].y;
			
			itens[i].x += itens[i].speed;	
			if (itens[i].x > 128 && itens[i].speed == 1)
				itens[i].x = 0;
			if (itens[i].x < 0 && itens[i].speed == -1)
				itens[i].x = 128;
			
			for (j = 0; j < NUM_CHANNELS; j++)
			{
				if (itens[i].glow_state[j] > 120)
					ds = -2;
				if (itens[i].glow_state[j] < 2)
					ds = 2;
				
				itens[i].glow_state[j] += ds;
			}
		}
=======
}

uint32_t currentColor = 0x00C97ABB;
uint8_t* numbers[9] ={one,two,three,four,five,six,seven,eight,nine};

void drawItem(Image* img)
{
	int i, j = 0, itemTopOffset;
	
	itemTopOffset = (127 - FLOOR_HEIGHT - LADDER_HEIGHT) - (LADDER_HEIGHT + FLOOR_HEIGHT) * (img->areaOffset);
	palette[ITEM] = currentColor;
	img->colorIndex = ITEM;
	if(img->collected)
	{
		if(img->dirX == RIGHT)
		{
			img->x--;
			clear(*img);
			img->x++;
		}
		else
		{
			img->x++;
			clear(*img);
			img->x--;
		}
		
		img->width = ITEM_SCORE_WIDTH;
		img->height = ITEM_SCORE_HEIGHT;
		img->y = itemTopOffset;
		img->data = numbers[currentScore/10];
		draw(*img);
		img->x += ITEM_SCORE_WIDTH;
		img->data = zero;
		draw(*img);
		img->x -= ITEM_SCORE_WIDTH;
		
	}
	else
	{
		img->data = item;
		img->width = ITEM_WIDTH;
		img->height = ITEM_HEIGHT;
		img->y = itemTopOffset;
		draw(*img);
	}
	
	currentColor += 1;
	if(currentColor == 0x00FFFFFF)
	{
		currentColor = 0x00C97ABB;
>>>>>>> biga
	}
}


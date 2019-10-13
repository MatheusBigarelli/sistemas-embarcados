#include "util.h"
#include "draw.h"
#include "joy.h"



void flipVert(uint8_t  image[], uint16_t rows, uint16_t columns)
{
	int i=0,j =0;
	uint8_t temp;
	for(i=0;i<rows;i++)
	{
		for(j=0;j<columns/2;j++)
		{
			temp = image[i*columns + j];
			image[i*columns + j] = image[i*columns + columns - 1 - j];
			image[i*columns + columns - 1 - j] = temp;
		}
	}
}

void intToString(int64_t value, char * pBuf, uint32_t len, uint32_t base, uint8_t zeros){
	static const char* pAscii = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	bool n = false;
	int pos = 0, d = 0;
	int64_t tmpValue = value;

	// the buffer must not be null and at least have a length of 2 to handle one
	// digit and null-terminator
	if (pBuf == NULL || len < 2)
			return;

	// a valid base cannot be less than 2 or larger than 36
	// a base value of 2 means binary representation. A value of 1 would mean only zeros
	// a base larger than 36 can only be used if a larger alphabet were used.
	if (base < 2 || base > 36)
			return;

	if (zeros > len)
		return;
	
	// negative value
	if (value < 0)
	{
			tmpValue = -tmpValue;
			value    = -value;
			pBuf[pos++] = '-';
			n = true;
	}

	// calculate the required length of the buffer
	do {
			pos++;
			tmpValue /= base;
	} while(tmpValue > 0);


	if (pos > len)
			// the len parameter is invalid.
			return;

	if(zeros > pos){
		pBuf[zeros] = '\0';
		do{
			pBuf[d++ + (n ? 1 : 0)] = pAscii[0]; 
		}
		while(zeros > d + pos);
	}
	else
		pBuf[pos] = '\0';

	pos += d;
	do {
			pBuf[--pos] = pAscii[value % base];
			value /= base;
	} while(value > 0);
}


uint16_t notes[2][14] = {
    {sAb(1), sA(1), sBb(1), sB(1), 0, sC(1), sDb(1), sD(1), sEb(1), sE(1), 0, sF(1), sGb(1), sG(1)},
    {sAb(2), sA(2), sBb(2), sB(2), 0, sC(2), sDb(2), sD(2), sEb(2), sE(2), 0, sF(2), sGb(2), sG(2)},
};

uint16_t getNote(uint8_t *ch)
{
    uint8_t scale = 0;
    if (ch[0] == '^')
        scale = 1;


    if (ch[1] >= 'A' && ch[1] <= 'G')
    {
        if (ch[2] == 'b')
            return notes[scale][2 * (ch[1] - 'A')];
        else if (ch[2] == 'n')
            return notes[scale][2 * (ch[1] - 'A')+1];
    }

    return 0;
}


uint32_t getPause(uint8_t ch)
{
    switch (ch) {
    case '+':
        return 0;
    case ',':
        return 500;
    case '.':
        return 1000;
    case '_':
        return 2000;
    default:
        return 5;
    }
}

void playNote(uint16_t note, uint32_t dur)
{
	buzzer_per_set(note);
	buzzer_write(true);
	osDelay(dur);
}

void playSong(uint8_t *song) {
    uint32_t note;
    uint32_t dur;
    uint32_t pause;
	
	dur = 1000;

    
     // A song is a collection of tones where each tone is
     // a note, duration and pause, e.g.
     // "E2,F4,"
     

    while(*song != '\0') {
        note = getNote(song);
		song += 3;
		
        pause = getPause(*song++);

        playNote(note, dur);
		
		buzzer_write(false);
		osDelay(pause);
    }
}

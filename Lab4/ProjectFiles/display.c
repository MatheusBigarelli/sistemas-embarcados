#include "display.h"

void Display(void const* args)
{
    //To print on the screen
    tContext sContext;

    osEvent event;
    SignalConfig_t* sigConfigMail;
	uint32_t lastTick = osKernelSysTick();
    
    WAVEFORMS waveform = TRIANGULAR;
    float frequency = 1.52, amplitude = 3.3;
	
    displayInit(&sContext);
    printHeader(&sContext);
    printFrequency(&sContext, frequency);
    printAmplitude(&sContext, amplitude);
    drawWaveform(&sContext, waveform);

    while (true)
    {
        event = osMailGet(qidDisplayMailQueue, osWaitForever);
        if (event.status == osEventMail)
        {
            sigConfigMail = event.value.p;
            if (sigConfigMail != NULL)
            {
                if (sigConfigMail->changedParameter == WAVEFORM)
                {
                    waveform = updateWaveformDisplay(&sContext, waveform, sigConfigMail->waveform);
                }

                if (sigConfigMail->changedParameter == FREQUENCY)
                {
                    frequency = updateFrequencyDisplay(&sContext, frequency, sigConfigMail->frequency);
                }

                if (sigConfigMail->changedParameter == AMPLITUDE)
                {
                    amplitude = updateAmplitudeDisplay(&sContext, amplitude, sigConfigMail->amplitude);
                }

                osMailFree(qidDisplayMailQueue, sigConfigMail);
            }
        }
		
		tickCounter(&lastTick,"Display");
    }
}

void displayInit(tContext* sContext)
{
    GrContextInit(sContext, &g_sCfaf128x128x16);

    GrFlush(sContext);
    GrContextFontSet(sContext, g_psFontFixed6x8);

    GrContextForegroundSet(sContext, ClrWhite);
    GrContextBackgroundSet(sContext, ClrBlack);
}

void printHeader(tContext* sContext)
{
    uint8_t i;
    char headerText[] = "Gerador de funcao";
    uint8_t lenHeaderText = strlen(headerText);
    uint32_t lineOffset = (128 - lenHeaderText * sContext->psFont->ui8MaxWidth) / 2;

    GrStringDrawCentered(sContext, headerText, lenHeaderText, 64, 10, true);

    for (i = lineOffset - 5; i < 128 - (lineOffset - 5); i++)
        GrPixelDraw(sContext, i, 20);
}

void printFrequency(tContext* sContext, float frequency)
{
    char freqText[4] = "Freq";
    char freqValue[6];
    uint8_t sidebarOffset = 10;

    floatToString(frequency, freqValue, 6, 10, true, 2);
    GrStringDraw(sContext, freqText, 4, sidebarOffset, LINE_OFFSET(LINE_FREQUENCY), true);
    GrStringDraw(sContext, freqValue, 6, sidebarOffset + 6, LINE_OFFSET(LINE_FREQUENCY)+10, true);
}

void printAmplitude(tContext *sContext, float amplitude)
{
    char amplitudeText[4] = "Ampt";
    char amplitudeValue[6];
    uint8_t sidebarOffset = 10;

    floatToString(amplitude, amplitudeValue, 6, 10, true, 2);
    GrStringDraw(sContext, amplitudeText, 4, sidebarOffset, LINE_OFFSET(LINE_AMPLITUDE), true);
    GrStringDraw(sContext, amplitudeValue, 6, sidebarOffset + 6, LINE_OFFSET(LINE_AMPLITUDE)+10, true);
}

void floatToString(float value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros, uint8_t precision)
{
    static const char *pAscii = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    uint8_t start = 0xFF;
    if (len < 2)
        return;

    if (base < 2 || base > 36)
        return;

    if (zeros + precision + 1 > len)
        return;

    intToString((int64_t)value, pBuf, len, base, zeros);
    while (pBuf[++start] != '\0' && start < len)
        ;

    if (start + precision + 1 > len)
        return;

    pBuf[start + precision + 1] = '\0';

    if (value < 0)
        value = -value;
    pBuf[start++] = '.';
    while (precision-- > 0)
    {
        value -= (uint32_t)value;
        value *= (float)base;
        pBuf[start++] = pAscii[(uint32_t)value];
    }
}

void intToString(int64_t value, char *pBuf, uint32_t len, uint32_t base, uint8_t zeros)
{
    static const char *pAscii = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
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
        value = -value;
        pBuf[pos++] = '-';
        n = true;
    }

    // calculate the required length of the buffer
    do
    {
        pos++;
        tmpValue /= base;
    } while (tmpValue > 0);

    if (pos > len)
        // the len parameter is invalid.
        return;

    if (zeros > pos)
    {
        pBuf[zeros] = '\0';
        do
        {
            pBuf[d++ + (n ? 1 : 0)] = pAscii[0];
        } while (zeros > d + pos);
    }
    else
        pBuf[pos] = '\0';

    pos += d;
    do
    {
        pBuf[--pos] = pAscii[value % base];
        value /= base;
    } while (value > 0);
}

void drawWaveform(tContext *sContext, WAVEFORMS waveform)
{

    switch (waveform)
    {
        case SINUSOIDAL:
            drawSine(sContext);
            break;
        
        case TRIANGULAR:
            drawTriangle(sContext);
            break;

        case TRAPEZOIDAL:
            drawTrapeze(sContext);
            break;

        case SQUARE:
            drawSquare(sContext);
            break;
        
        case SAWTOOTH:
            drawSaw(sContext);
    }
}

void drawSine (tContext* sContext)
{
    uint8_t x[WAVE_DISPLAY_WIDTH], y[WAVE_DISPLAY_HEIGHT];
    uint8_t i;
    double xNorm;
    for (i = 0; i < WAVE_DISPLAY_WIDTH; i++)
    {
        x[i] = WAVE_DISPLAY_X_START + i;
    }
    for (i = 0; i < WAVE_DISPLAY_HEIGHT; i++)
    {
        xNorm = (double)(i) / (double)WAVE_DISPLAY_WIDTH;
        y[i] = (uint8_t)(WAVE_DISPLAY_Y_START + 20 - 20*sin(2 * 3.1415 * xNorm));
    }
    plot(sContext, x, y);
}

void drawTriangle (tContext* sContext)
{
    uint8_t x[WAVE_DISPLAY_WIDTH], y[WAVE_DISPLAY_HEIGHT];
    uint8_t i;
    double xNorm;
    for (i = 0; i < WAVE_DISPLAY_WIDTH; i++)
    {
        x[i] = WAVE_DISPLAY_X_START + i;
    }
    for (i = 0; i < WAVE_DISPLAY_HEIGHT; i++)
    {
        if (i < WAVE_DISPLAY_HEIGHT/4)
            // Subindo do ponto médio para o pico da onda
            y[i] = WAVE_DISPLAY_Y_START+WAVE_DISPLAY_HEIGHT/2 - 2*i;
        else if (i < WAVE_DISPLAY_HEIGHT*3/4)
            // Descendo da crista ao vale
            y[i] = WAVE_DISPLAY_Y_START + 2*(i-WAVE_DISPLAY_HEIGHT/4.0);
        else
            // Terminando o período da onda
            y[i] = WAVE_DISPLAY_Y_START+WAVE_DISPLAY_HEIGHT - 2*(i-WAVE_DISPLAY_HEIGHT*3.0/4.0);
    }
    plot(sContext, x, y);
}

void drawTrapeze (tContext* sContext)
{
    uint8_t x[WAVE_DISPLAY_WIDTH], y[WAVE_DISPLAY_HEIGHT];
    uint8_t i;
    double xNorm;
    for (i = 0; i < WAVE_DISPLAY_WIDTH; i++)
    {
        x[i] = WAVE_DISPLAY_X_START + i;
    }
    for (i = 0; i < WAVE_DISPLAY_HEIGHT; i++)
    {
        if (i < WAVE_DISPLAY_HEIGHT / 6.0)
            // Subindo do ponto médio para o pico da onda
            y[i] = WAVE_DISPLAY_Y_START + WAVE_DISPLAY_HEIGHT / 2 - 3*i;
        else if (i < WAVE_DISPLAY_HEIGHT * 2.0/6.0)
            // Pico do tronco
            y[i] = y[i-1];
        else if (i < WAVE_DISPLAY_HEIGHT * 4.0/6.0)
            // Descendo até o vale
            y[i] = WAVE_DISPLAY_Y_START + 3 * (i - WAVE_DISPLAY_HEIGHT * 2.0/6.0);
        else if (i < WAVE_DISPLAY_HEIGHT * 5.0/6.0)
            // Segundo tronco
            y[i] = y[i-1];
        else
            // Terminando o período da onda
            y[i] = WAVE_DISPLAY_Y_START + WAVE_DISPLAY_HEIGHT - 3 * (i - WAVE_DISPLAY_HEIGHT * 5.0/6.0);
    }

    plot(sContext, x, y);
}

void drawSquare (tContext* sContext)
{
    uint8_t x[WAVE_DISPLAY_WIDTH], y[WAVE_DISPLAY_HEIGHT];
    uint8_t i;
    double xNorm;
    for (i = 0; i < WAVE_DISPLAY_WIDTH; i++)
    {
        x[i] = WAVE_DISPLAY_X_START + i;
    }
    for (i = 0; i < WAVE_DISPLAY_HEIGHT; i++)
    {
        if (i < WAVE_DISPLAY_HEIGHT / 2)
            y[i] = WAVE_DISPLAY_Y_START;
        else
            y[i] = WAVE_DISPLAY_Y_START + WAVE_DISPLAY_HEIGHT/2;
    }
    plot(sContext, x, y);
    // Complementando o plot na descontinuidade
    for (i = 0; i < WAVE_DISPLAY_HEIGHT; i++)
    {
        if (i < WAVE_DISPLAY_HEIGHT / 2)
            GrPixelDraw(sContext, x[0], WAVE_DISPLAY_Y_START+WAVE_DISPLAY_HEIGHT/2 - i);
        else
            GrPixelDraw(sContext, x[WAVE_DISPLAY_HEIGHT/2], WAVE_DISPLAY_Y_START + (i-WAVE_DISPLAY_HEIGHT/2));
    }
}

void drawSaw (tContext* sContext)
{
    uint8_t x[WAVE_DISPLAY_WIDTH], y[WAVE_DISPLAY_HEIGHT];
    uint8_t i;
    double xNorm;
    for (i = 0; i < WAVE_DISPLAY_WIDTH; i++)
    {
        x[i] = WAVE_DISPLAY_X_START + i;
    }
    for (i = 0; i < WAVE_DISPLAY_HEIGHT; i++)
    {
        y[i] = WAVE_DISPLAY_Y_START + WAVE_DISPLAY_HEIGHT/2 - i/2;
    }
    plot(sContext, x, y);
    // Complementando o plot na descontinuidade
    for (i = 0; i < WAVE_DISPLAY_HEIGHT/2; i++)
    {
        GrPixelDraw(sContext, x[WAVE_DISPLAY_WIDTH-1], WAVE_DISPLAY_Y_START+1 + i);
    }
}

void plot(tContext* sContext, uint8_t x[WAVE_DISPLAY_WIDTH], uint8_t y[WAVE_DISPLAY_HEIGHT])
{
    uint8_t i;
    for (i = 0; i < WAVE_DISPLAY_HEIGHT; i++)
        GrPixelDraw(sContext, x[i], y[i]);
}

void clearWaveform(tContext* sContext, WAVEFORMS waveform)
{
    GrContextForegroundSet(sContext, ClrBlack);
    drawWaveform(sContext, waveform);
    GrContextForegroundSet(sContext, ClrWhite);
}

WAVEFORMS updateWaveformDisplay(tContext* sContext, WAVEFORMS oldWaveform, WAVEFORMS newWaveform)
{
    // Ondas válidas
    if (SINUSOIDAL <= newWaveform && newWaveform <= SAWTOOTH)
    {
        clearWaveform(sContext, oldWaveform);
        drawWaveform(sContext, newWaveform);
        return newWaveform;
    }
    
    // Número dado não é uma forma de onda válida.
    return oldWaveform;
}

float updateFrequencyDisplay(tContext* sContext, float oldFrequency, float newFrequency)
{
    // Frequências válidas
    if (0 < newFrequency && newFrequency <= 200)
    {
        clearFrequency(sContext, oldFrequency);
        printFrequency(sContext, newFrequency);
        return newFrequency;
    }

    return oldFrequency;
}

void clearFrequency(tContext* sContext, float frequency)
{
    GrContextForegroundSet(sContext, ClrBlack);
    printFrequency(sContext, frequency);
    GrContextForegroundSet(sContext, ClrWhite);
}

float updateAmplitudeDisplay(tContext* sContext, float oldAmplitude, float newAmplitude)
{
    // Frequências válidas
    if (0 < newAmplitude && newAmplitude <= 3.3)
    {
        clearAmplitude(sContext, oldAmplitude);
        printAmplitude(sContext, newAmplitude);
        return newAmplitude;
    }

    return oldAmplitude;
}

void clearAmplitude(tContext* sContext, float amplitude)
{
    GrContextForegroundSet(sContext, ClrBlack);
    printAmplitude(sContext, amplitude);
    GrContextForegroundSet(sContext, ClrWhite);
}

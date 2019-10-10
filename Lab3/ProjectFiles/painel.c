#include "painel.h"

extern osMutexId context_mutex;
extern tContext sContext;

extern void Title(void);
extern void LadderFunc(void);
extern void Floor(void);

void PainelDeInstrumentos(void const *args)
{
	Title();
	while (1)
	{
		LadderFunc();	
		Floor();
	}
}

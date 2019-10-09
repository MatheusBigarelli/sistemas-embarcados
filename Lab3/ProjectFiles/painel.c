#include "painel.h"

extern osMutexId context_mutex;
extern tContext sContext;

extern void Name(void);
extern void Ladder(void);
extern void Floor(void);

void PainelDeInstrumentos(void const *args)
{
	Name();
	while (1)
	{
		Ladder();	
		Floor();
	}
}

/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 T O C                                     */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Struktur toc dient dazu, bei einem Koroutinenwechsel die Werte der    */
/* nicht-fluechtigen Register zu sichern. toc_settle bereitet sie            */
/* entsprechend vor.                                                         */
/*****************************************************************************/

#include <stdint.h>

#include "machine/toc.h"

#include "thread/coroutine.h"

extern "C" {

void setmem(void *p, unsigned l, char v)
{
	char *s = (char *) p;

	while(l--) { *s++ = v; }
}

// TOC_SETTLE: bereitet den Kontext der Koroutine fuer den ersten Aufruf vor.
void toc_settle(toc_t *regs, void *tos, void *self)
{
	setmem(regs, sizeof(toc_t), 0);

	uint64_t *stack = (uint64_t *) tos;

	*--stack = (uint64_t) self;
	*--stack = (uint64_t) &oostubs::Coroutine::kickoff;

	regs->rsp = stack;
}

}


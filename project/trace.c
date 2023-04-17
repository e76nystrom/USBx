#include "stm32h743xx.h"
#include "trace.h"

//#define TRACE

#if defined(TRACE)

#include <stdio.h>
#include <string.h>

#if defined(USB)
static unsigned int sysClock;
#endif  /* USB */

T_TRC_QUE trcQue;

void trcInit(void)
{
 memset((void *) &trcQue, 0, sizeof(trcQue));

#if defined(USB)
 sysClock = HAL_RCC_GetSysClockFreq();
#endif  /* USB */
}

static unsigned int lastCycleCtr;
extern unsigned int sysClock;
static unsigned int trcCtr;

void trcDisplay(void)
{
 char pBuf[120];
 while (trcQue.count > 0) {
  P_TRC_MSG p = &trcQue.data[trcQue.emp];

  snprintf(pBuf, sizeof(pBuf), "%5d %3d %4d ", trcCtr, trcQue.count, trcQue.emp);
  putstr(pBuf);
  switch(p->type)
  {
   case TRC_RX:
   printf("rx %08x\n", (unsigned int) p->rx.d0);
   break;

   case TRC_TRC:
    snprintf(pBuf, sizeof(pBuf), "%-20s %4d %-20s\n",
             p->trc.file, p->trc.line, p->trc.func);
    putstr(pBuf);
   break;

   case TRC_TRC1:
    snprintf(pBuf, sizeof(pBuf), "%-20s %4d %-20s %4d\n",
             p->trc.file, p->trc.line, p->trc.func, p->trc.val);
    putstr(pBuf);
    break;

   case TRC_TRC2:
    snprintf(pBuf, sizeof(pBuf), "%-20s %4d %-20s %4d %4d\n",
             p->trc.file, p->trc.line, p->trc.func,
             p->trc.val1, p->trc.val2);
    putstr(pBuf);
    break;

   case TRC_ISR:
   {
    unsigned int ctr = p->isr.cycleCtr;
    unsigned int delta = ctr - lastCycleCtr;
    double t = (double) delta / sysClock;
    double us = t * 1000000;
    snprintf(pBuf, sizeof(pBuf), "isr %d %4d %8.1f us\n",
	     p->isr.flag, p->isr.isrCount, us);
    lastCycleCtr = ctr;
    putstr(pBuf);
    if (p->isr.flag)
     putstr("\n");
   }
    break;

   default:
    break;
 }

  trcCtr += 1;
  trcQue.emp += 1;
  if (trcQue.emp >= MAX_TRC_MSG)
   trcQue.emp = 0;
  __disable_irq();
  trcQue.count -= 1;
  __enable_irq();
 }
}

void printFunc(const char *file, int line, const char * func)
{
 char pBuf[120];
 snprintf(pBuf, sizeof(pBuf), "%-20s %4d %-20s\n", file, line, func);
 putstr(pBuf);
}

void trcTrc(const char *file, int line, const char *func)
{
 P_TRC_MSG p = &trcQue.data[trcQue.fil];
 p->type = TRC_TRC;
 p->trc.file = file;
 p->trc.line = line
     ;
 p->trc.func = func;
 p->trc.start = isrStart;
 p->trc.end = isrEnd;
 trcQue.fil += 1;
 if (trcQue.fil >= MAX_TRC_MSG)
  trcQue.fil = 0;
 trcQue.count += 1;
}

void trcTrc1(const char *file, uint16_t line,
             const char *func, uint16_t val)
{
 P_TRC_MSG p = &trcQue.data[trcQue.fil];
 p->type = TRC_TRC1;
 p->trc.file = file;
 p->trc.line = line;
 p->trc.func = func;
 p->trc.val = val;
 trcQue.fil += 1;
 if (trcQue.fil >= MAX_TRC_MSG)
  trcQue.fil = 0;
 trcQue.count += 1;
}

void trcTrc2(const char *file, uint16_t line,
             const char *func, uint16_t val1, uint16_t val2)
{
 P_TRC_MSG p = &trcQue.data[trcQue.fil];
 p->type = TRC_TRC2;
 p->trc.file = file;
 p->trc.line = line;
 p->trc.func = func;
 p->trc.val1 = val1;
 p->trc.val2 = val2;
 trcQue.fil += 1;
 if (trcQue.fil >= MAX_TRC_MSG)
  trcQue.fil = 0;
 trcQue.count += 1;
}

void trcISR(int flag, int count)
{
 P_TRC_MSG p = &trcQue.data[trcQue.fil];
 p->type = TRC_ISR;
 p->isr.flag = flag;
 p->isr.isrCount = count;
 p->isr.cycleCtr = DWT->CYCCNT;
 trcQue.fil += 1;
 if (trcQue.fil >= MAX_TRC_MSG)
  trcQue.fil = 0;
 trcQue.count += 1;
}

#else   /* TRACE */

inline void trcInit(void) {}
inline void trcDisplay(void) {}
inline void printFunc(const char *file  __attribute__((unused)),
		      int line  __attribute__((unused)),
		      const char *func __attribute__((unused))) {}
inline void trcTrc(const char *file  __attribute__((unused)),
		   int line  __attribute__((unused)),
		   const char *func __attribute__((unused))) {}
inline void trcTrc1(const char *file  __attribute__((unused)),
		    uint16_t line  __attribute__((unused)),
		    const char *func  __attribute__((unused)),
		    uint16_t val __attribute__((unused))) {}
inline void trcTrc2(const char *file  __attribute__((unused)),
		    uint16_t line  __attribute__((unused)),
		    const char *func  __attribute__((unused)),
		    uint16_t val1  __attribute__((unused)),
		    uint16_t val2 __attribute__((unused))) {}
inline void trcISR(int flag  __attribute__((unused)),
		   int count __attribute__((unused))) {}

#endif  /* TRACE */

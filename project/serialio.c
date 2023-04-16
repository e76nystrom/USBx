#include "stm32h7xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "include/serialio.h"
#define DBGPORT USART3
/*
void putx(char c);
void putstr(const char *p);
void sndhex(unsigned char *p, int size);
void newline(void);
void flushBuf(void);
void dumpBuf(unsigned long *p, unsigned int len);
*/

#define CHAR_BUF_SIZE 64
typedef struct sCharBuf
{
 int fil;
 int emp;
 int count;
 unsigned int overflow;
 char buf[CHAR_BUF_SIZE];
} T_CHAR_BUF, *P_CHAR_BUF;

T_CHAR_BUF charBuf;

inline uint32_t dbgTxEmpty(void)
{
 return(DBGPORT->ISR & USART_ISR_TXE_TXFNF);
}
inline void dbgTxSend(char ch)
{
 DBGPORT->TDR = ch;
}

void newline(void)
{
 putx('\n');
 putx('\r');
}

void flushBuf(void)
{}

void putx(char c)
{
 while (dbgTxEmpty() == 0)
  ;
 dbgTxSend(c);
}

void putstr(const char *p)
{
 while (true)
 {
  char ch = *p++;
  if (ch == 0)
   break;
  putx(ch);
  if (ch == '\n')
   putx('\r');
 }
}

void initCharBuf(void)
{
 charBuf.fil = 0;
 charBuf.emp = 0;
 charBuf.count = 0;
 charBuf.overflow = 0;
}

void putBufChar(char ch)
{
 uint32_t priMask = __get_PRIMASK();
 __disable_irq();
 if (charBuf.count < CHAR_BUF_SIZE)
 {
  charBuf.count++;
  if (priMask == 0)
   __enable_irq();
  charBuf.buf[charBuf.fil] = ch;
  charBuf.fil++;
  if (charBuf.fil >= CHAR_BUF_SIZE)
   charBuf.fil = 0;
 }
 else
 {
  if (priMask == 0)
   __enable_irq();
  charBuf.overflow++;
 }
}

void putBufStr(const char *s)
{
 char ch;
 while ((ch = *s++) != 0)
 {
  putBufChar(ch);
 }
}

void pollBufChar(void)
{
 __disable_irq();
 if (charBuf.count > 0)
 {
  --charBuf.count;
  __enable_irq();
  char ch = charBuf.buf[charBuf.emp];
  charBuf.emp++;
  if (charBuf.emp >= CHAR_BUF_SIZE)
   charBuf.emp = 0;
  putx(ch);
 }
 else
 {
  __enable_irq();
  charBuf.overflow++;
 }
}

void sndhex(unsigned char *p, int size)
{
 char tmp;
 char ch;

 p += size;
 while (size != 0)
 {
  --size;
  p--;
  tmp = *p;
  ch = tmp;
  ch >>= 4;
  ch &= 0xf;
  if (ch < 10)
   ch += '0';
  else
   ch += 'a' - 10;
//  while (utxbf1);
//  u1txreg = ch;
  putx(ch);

  tmp &= 0xf;
  if (tmp < 10)
   tmp += '0';
  else
   tmp += 'a' - 10;
//  while (utxbf1);
//  u1txreg = tmp;
  putx(tmp);
 }
}

void dumpBuf(unsigned long *p, unsigned int len)
{
#define MAX_COL 4
 char col = 0;
 for (unsigned int i = 0; i < len; i += 4)
 {
  if (col == 0)			/* if column 0 */
  {
   printf("%08x  ", (unsigned int) p);
  }
  unsigned int val = *p++;
  printf("%08x ", val);		/* output value */
  col += 1;			/* count a column */
  if (col == MAX_COL)		/* if at end of line */
  {
   col = 0;			/* reset column counter */
   printf("\n");
  }
 }
 if (col != 0)
  printf("\n");
}

_ssize_t _write(int fd  __attribute__((unused)), const char* buf, _ssize_t nbyte)
{
 _ssize_t count = nbyte;

  while (--count >= 0)
  {
   char ch = *buf++;
   putx(ch);
   if (ch == '\n')
    putx('\r');
  }

 return(nbyte);
}

#define DWT_CTRL_CycCntEna DWT_CTRL_CYCCNTENA_Msk
inline void resetCnt()
{
 DWT->CTRL &= ~DWT_CTRL_CycCntEna; // disable the counter
 DWT->CYCCNT = 0;		// reset the counter
}

inline void startCnt()
{
 DWT->CTRL |= DWT_CTRL_CycCntEna; // enable the counter
}

inline void stopCnt()
{
 DWT->CTRL &= ~DWT_CTRL_CycCntEna; // disable the counter
}

inline unsigned int getCycles()
{
 return DWT->CYCCNT;
}

//static unsigned int lastCycleCtr;
//extern unsigned int sysClock;
//static unsigned int trcCtr;
//
//void trcDisplay(void)
//{
// char pBuf[120];
// while (trcQue.count > 0) {
//  P_TRC_MSG p = &trcQue.data[trcQue.emp];
//
//  snprintf(pBuf, sizeof(pBuf), "%5d %3d %4d ", trcCtr, trcQue.count, trcQue.emp);
//  putstr(pBuf);
//  switch(p->type)
//  {
//   case TRC_RX:
//   printf("rx %08x\n", (unsigned int) p->rx.d0);
//   break;
//
//   case TRC_TRC:
//    snprintf(pBuf, sizeof(pBuf), "%-20s %4d %-20s\n",
//             p->trc.file, p->trc.line, p->trc.func);
//    putstr(pBuf);
//   break;
//
//   case TRC_TRC1:
//    snprintf(pBuf, sizeof(pBuf), "%-20s %4d %-20s %4d\n",
//             p->trc.file, p->trc.line, p->trc.func, p->trc.val);
//    putstr(pBuf);
//    break;
//
//   case TRC_TRC2:
//    snprintf(pBuf, sizeof(pBuf), "%-20s %4d %-20s %4d %4d\n",
//             p->trc.file, p->trc.line, p->trc.func,
//             p->trc.val1, p->trc.val2);
//    putstr(pBuf);
//    break;
//
//   case TRC_ISR:
//   {
//    unsigned int ctr = p->isr.cycleCtr;
//    unsigned int delta = ctr - lastCycleCtr;
//    double t = (double) delta / sysClock;
//    double us = t * 1000000;
//    snprintf(pBuf, sizeof(pBuf), "isr %d %4d %8.1f us\n",
//	     p->isr.flag, p->isr.isrCount, us);
//    lastCycleCtr = ctr;
//    putstr(pBuf);
//    if (p->isr.flag)
//     putstr("\n");
//   }
//    break;
//
//   default:
//    break;
// }
//
//  trcCtr += 1;
//  trcQue.emp += 1;
//  if (trcQue.emp >= MAX_TRC_MSG)
//   trcQue.emp = 0;
//  __disable_irq();
//  trcQue.count -= 1;
//  __enable_irq();
// }
//}
//
//void trcTrc(const char *file, int line, const char *func)
//{
// P_TRC_MSG p = &trcQue.data[trcQue.fil];
// p->type = TRC_TRC;
// p->trc.file = file;
// p->trc.line = line
//     ;
// p->trc.func = func;
// p->trc.start = isrStart;
// p->trc.end = isrEnd;
// trcQue.fil += 1;
// if (trcQue.fil >= MAX_TRC_MSG)
//  trcQue.fil = 0;
// trcQue.count += 1;
//}
//
//void trcTrc1(const char *file, uint16_t line,
//             const char *func, uint16_t val)
//{
// P_TRC_MSG p = &trcQue.data[trcQue.fil];
// p->type = TRC_TRC1;
// p->trc.file = file;
// p->trc.line = line;
// p->trc.func = func;
// p->trc.val = val;
// trcQue.fil += 1;
// if (trcQue.fil >= MAX_TRC_MSG)
//  trcQue.fil = 0;
// trcQue.count += 1;
//}
//
//void trcTrc2(const char *file, uint16_t line,
//             const char *func, uint16_t val1, uint16_t val2)
//{
// P_TRC_MSG p = &trcQue.data[trcQue.fil];
// p->type = TRC_TRC2;
// p->trc.file = file;
// p->trc.line = line;
// p->trc.func = func;
// p->trc.val1 = val1;
// p->trc.val2 = val2;
// trcQue.fil += 1;
// if (trcQue.fil >= MAX_TRC_MSG)
//  trcQue.fil = 0;
// trcQue.count += 1;
//}
//
//void trcISR(int flag, int count)
//{
// P_TRC_MSG p = &trcQue.data[trcQue.fil];
// p->type = TRC_ISR;
// p->isr.flag = flag;
// p->isr.isrCount = count;
// p->isr.cycleCtr = getCycles();
// trcQue.fil += 1;
// if (trcQue.fil >= MAX_TRC_MSG)
//  trcQue.fil = 0;
// trcQue.count += 1;
//}

#if 0
void test()
{
 T_TRC_MSG x;
 uint32_t a = x.data[0];
 int b = x.type;
 b = (int) x.rx.d0;
 b = x.tx.x0;
 int tmp = (int) 'abcd';
 printf("%c\n", tmp);
}
#endif

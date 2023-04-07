#include "stm32h7xx_hal.h"
#include <stdbool.h>
#include <stdio.h>

#include "serialio.h"
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

T_TRC_QUE trcQue;

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
 __disable_irq();
 if (charBuf.count < CHAR_BUF_SIZE)
 {
  charBuf.count++;
  __enable_irq();
  charBuf.buf[charBuf.fil] = ch;
  charBuf.fil++;
  if (charBuf.fil >= CHAR_BUF_SIZE)
   charBuf.fil = 0;
 }
 else
 {
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

void printFunc(const char *file, int line, const char * func)
{
 char pBuf[120];
 snprintf(pBuf, sizeof(pBuf), "%-72s %4d %-20s\n", file, line, func);
 putstr(pBuf);
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

void trcDisplay(void)
{
 while (trcQue.count > 0) {
  P_TRC_MSG p = &trcQue.data[trcQue.emp];
  trcQue.emp += 1;
  trcQue.emp &= ~(MAX_TRC_MSG - 1);
  int type = p->type;
  if (type == TRC_RX) {
   printf("rx %08x\n", (unsigned int) p->rx.d0);
  }
  trcQue.count -= 1;
 }
}

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
//
// Created by Eric on 4/3/2023.
//
#include "main.h"

#ifndef USBTEST2_SERIALIO_H
#define USBTEST2_SERIALIO_H

uint32_t dbgTxEmpty(void);
void dbgTxSend(char ch);

void putx(char c);
void putstr(const char *p);
void sndhex(unsigned char *p, int size);
void newline(void);
void flushBuf(void);
void dumpBuf(unsigned long *p, unsigned int len);

void initCharBuf(void);
void putBufChar(char ch);
void putBufStr(const char *s);
void pollBufChar(void);

void trcDisplay(void);

void printFunc(const char *file, int line, const char * func);

//#define PRINT_FUNC() printf("%-72s %4d %-20s\n", __FILE__, __LINE__, __func__)
#define PRINT_FUNC() printFunc(__FILE_NAME__, __LINE__, __func__)

struct s_generic
{
 uint32_t data[4];
};

typedef struct s_rx
{
 uint32_t d0, d1, d2;
} T_RX;

typedef struct s_tx
{
 int x0, x1, x2;
} T_TX;

typedef struct s_trc
{
 const char *file;
 uint16_t line;
 uint16_t val;
 const char *func;
 uint16_t start;
 uint16_t end;
 uint16_t val1;
 uint16_t val2;
} T_TRC, *P_TRC;

typedef struct s_isr
{
 unsigned int cycleCtr;
 int isrCount;
 int flag;
} T_ISR, *P_ISR;

typedef union
{
 uint32_t data[4];
 struct
 {
  int type;
  union
  {
   T_RX rx;
   T_TX tx;
   T_TRC trc;
      T_ISR isr;
  };
 };
} T_TRC_MSG, *P_TRC_MSG;

#define MAX_TRC_MSG 1024

enum {
 TRC_NONE, TRC_TRC, TRC_TRC1, TRC_TRC2, TRC_RX, TRC_ISR
};

typedef struct s_TrcQue
{
 int count;
 int fil;
 int emp;
 T_TRC_MSG data[MAX_TRC_MSG];
} T_TRC_QUE, *P_TRC_QUE;

extern T_TRC_QUE trcQue;

void trcTrc(const char *file, int line, const char *func);
void trcTrc1(const char *file, uint16_t line, const char *func, uint16_t val);
void trcTrc2(const char *file, uint16_t line,
             const char *func, uint16_t val1, uint16_t val2);
void trcISR(int flag, int count);

inline void trcRx(uint32_t val1  __attribute__((unused)))
{
 P_TRC_MSG p = &trcQue.data[trcQue.fil];
 p->type = TRC_RX;
 p->rx.d0 = val1;
 trcQue.fil += 1;
 trcQue.fil &= ~(MAX_TRC_MSG - 1);
 trcQue.count += 1;
}

void trcInit(void);

inline void dbg0Set() {Dbg0_GPIO_Port->BSRR = Dbg0_Pin;}
inline void dbg0Clr() {Dbg0_GPIO_Port->BSRR = (Dbg0_Pin << 16);}

inline void dbg2Set() {Dbg2_GPIO_Port->BSRR = Dbg2_Pin;}
inline void dbg2Clr() {Dbg2_GPIO_Port->BSRR = (Dbg2_Pin << 16);}

extern int isrStart;
extern int isrEnd;

void resetCnt();
void startCnt();
void stopCnt();
unsigned int getCycles();

#endif // USBTEST2_SERIALIO_H

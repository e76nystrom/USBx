//
// Created by Eric on 4/15/2023.
//

#ifndef USBX_TRACE_H
#define USBX_TRACE_H

#include "main.h"
#if !defined(USB)
#include "serialio.h"
#else
void putstr(const char *p);
#endif  /* USB */

void trcInit(void);
void trcDisplay(void);
void printFunc(const char *file, int line, const char * func);
void trcTrc(const char *file, int line, const char *func);
void trcTrc1(const char *file, uint16_t line, const char *func, uint16_t val);
void trcTrc2(const char *file, uint16_t line,
             const char *func, uint16_t val1, uint16_t val2);
void trcISR(int flag, int count);

//#define PRINT_FUNC() printf("%-72s %4d %-20s\n", __FILE__, __LINE__, __func__)
#define PRINT_FUNC() printFunc(file, __LINE__, __func__)

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

inline void trcRx(uint32_t val1  __attribute__((unused)))
{
 P_TRC_MSG p = &trcQue.data[trcQue.fil];
 p->type = TRC_RX;
 p->rx.d0 = val1;
 trcQue.fil += 1;
 trcQue.fil &= ~(MAX_TRC_MSG - 1);
 trcQue.count += 1;
}

#if !defined(USB)
inline void dbg0Set() {Dbg0_GPIO_Port->BSRR = Dbg0_Pin;}
inline void dbg0Clr() {Dbg0_GPIO_Port->BSRR = (Dbg0_Pin << 16);}

inline void dbg2Set() {Dbg2_GPIO_Port->BSRR = Dbg2_Pin;}
inline void dbg2Clr() {Dbg2_GPIO_Port->BSRR = (Dbg2_Pin << 16);}
#endif  /* USB */

extern int isrStart;
extern int isrEnd;

#endif //USBX_TRACE_H

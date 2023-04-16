//
// Created by Eric on 4/3/2023.
//

#ifndef USBTEST2_SERIALIO_H
#define USBTEST2_SERIALIO_H

#include <stdint.h>

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

void resetCnt();
void startCnt();
void stopCnt();
unsigned int getCycles();

#endif // USBTEST2_SERIALIO_H

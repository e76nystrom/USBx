/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "bsp/board.h"
#include "tusb.h"
#include <stdio.h>

#if defined(LATHE_USB)
#include "tusb_config.h"

#include "include/serialio.h"
#include "stm32h7xx_hal.h"
#include "board.h"
#include "include/stm32Info.h"
#include "trace.h"

static const char *file = __FILE_NAME__;

extern char _sbss;
extern char _ebss;
extern char _sdata;
extern char _edata;
extern char _estack;
extern char _end;

//_ssize_t write(int fd  __attribute__((unused)), const char* buf, _ssize_t nbyte);
//void Error_Handler(void);
unsigned int getSP(void);
#endif

//------------- prototypes -------------//
static void cdc_task(void);
/*------------- MAIN -------------*/
#if defined(LATHE_USB)

#include <unistd.h>

//#define FIX_SYS
#if defined(FIX_SYS)
extern void fixSys(void);
#endif

int isrStart;
int isrEnd;

#if !defined DWT_LSR_Present_Msk
#define DWT_LSR_Present_Msk ITM_LSR_Present_Msk
#endif

#if !defined DWT_LSR_Access_Msk
#define DWT_LSR_Access_Msk ITM_LSR_Access_Msk
#endif

#define CoreDebug_DEMCR_TrcEna CoreDebug_DEMCR_TRCENA_Msk
#define DWT_LAR_KEY 0xC5ACCE55

void dwtAccessEnable(unsigned ena)
{
 uint32_t lsr = DWT->LSR;

 printf("lsr %08x\n", (unsigned int) lsr);

 CoreDebug->DEMCR |= CoreDebug_DEMCR_TrcEna;
 if ((lsr & DWT_LSR_Present_Msk) != 0)
 {
  if (ena)
  {
   if ((lsr & DWT_LSR_Access_Msk) != 0) //locked: access need unlock
   {
    DWT->LAR = DWT_LAR_KEY;
   }
  }
  else
  {
   if ((lsr & DWT_LSR_Access_Msk) == 0) //unlocked
   {
    DWT->LAR = 0;
   }
  }
 }
}

unsigned int sysClock;
#endif

int tinyMain(void)

#if defined(LATHE_USB)
{
 GPIO_InitTypeDef GPIO_InitStruct = {0};
 isrStart = 0;
 isrEnd = 0;
#if defined(FIX_SYS)
 fixSys();
#endif

 trcInit();
 board_init();
 putstr("start\n");
 /*Configure GPIO pin : PtPin */
 GPIO_InitStruct.Pin = Dbg2_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(Dbg2_GPIO_Port, &GPIO_InitStruct);

 /*Configure GPIO pin : PtPin */
 GPIO_InitStruct.Pin = Dbg0_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(Dbg0_GPIO_Port, &GPIO_InitStruct);
 gpioInfo(Dbg0_GPIO_Port);
 gpioInfo(Dbg2_GPIO_Port);

 printf("DWT_CTRL %x\n", (unsigned int) DWT->CTRL);
 dwtAccessEnable(1);
 resetCnt();
 startCnt();
 stopCnt();
 printf("cycles %u\n", getCycles());

 char buf[] = "_write\n";
 write(0, buf, sizeof(buf));

 unsigned int bss = (unsigned int) (&_ebss - &_sbss);
 unsigned int data = (unsigned int) (&_edata- &_sdata);
 unsigned int sp = getSP();
 unsigned int stack = (unsigned int) &_estack;//&__stack;
 unsigned int used = stack - sp;
// unsigned int stackLimit = (unsigned int) &__Main_Stack_Limit;
 putstr("sp ");
 sndhex((unsigned char *) &sp, sizeof(sp));
 putstr(" stack ");
 sndhex((unsigned char *) &stack, sizeof(stack));
 putstr(" used ");
 sndhex((unsigned char *) &used, sizeof(used));
// putstr(" stackLimit ");
// sndhex((unsigned char *) &stackLimit, sizeof(stackLimit));
 putstr("\n");
 printf("data %u bss %u total %u\n", data, bss, data + bss);
 printf("stack %08x sp %08x\n",
        (unsigned int) &_estack, getSP());
 sysClock = HAL_RCC_GetSysClockFreq();
 unsigned int clockFreq = HAL_RCC_GetHCLKFreq();
 unsigned int FCY = HAL_RCC_GetPCLK2Freq() * 2;
  printf("sys clock %u clock frequency %u FCY %u\n",
         sysClock, clockFreq, FCY);

// RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;

 printf("SYSCFG->PWRCR %08x RCC->APB4ENR %08x PWR->D3CR %08x\n",
        (unsigned int) SYSCFG->PWRCR, (unsigned int) RCC->APB4ENR,
        (unsigned int) PWR->D3CR);

 printf("RCC->D2CCIP2R %x\n", (unsigned int)
        (RCC->D2CCIP2R & RCC_D2CCIP2R_USBSEL) >> RCC_D2CCIP2R_USBSEL_Pos);

// SYSCFG->PWRCR = SYSCFG_PWRCR_ODEN;
//
// printf("SYSCFG->PWRCR %08x\n", (unsigned int) SYSCFG->PWRCR);

 PRINT_FUNC();
  initCharBuf();
 printf("testing\n");

 resetCnt();
 startCnt();
 printf("cycles %u\n", getCycles());
#else
 int main(void)
 {
  board_init();
#endif

// init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);
  
#if defined(LATHE_USB)
 uint32_t t0 = HAL_GetTick();
#endif

  while (1)
  {
    tud_task(); // tinyusb device task
    cdc_task();
    
#if defined(LATHE_USB)
    pollBufChar();
    trcDisplay();

   uint32_t t = HAL_GetTick();
   if ((t - t0) > 500) {
    t0 = t;
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
#endif
   }
  }
}

// echo to either Serial0 or Serial1
// with Serial0 as all lower case, Serial1 as all upper case
static void echo_serial_port(uint8_t itf, uint8_t buf[], uint32_t count)
{
  uint8_t const case_diff = 'a' - 'A';

  for(uint32_t i=0; i<count; i++)
  {
    if (itf == 0)
    {
      // echo back 1st port as lower case
      if (isupper(buf[i])) buf[i] += case_diff;
    }
    else
    {
      // echo back 2nd port as upper case
      if (islower(buf[i])) buf[i] -= case_diff;
    }

    tud_cdc_n_write_char(itf, buf[i]);
  }
  tud_cdc_n_write_flush(itf);
}

#if defined(LATHE_USB)
void prtBuf(unsigned char *p, int size)
{
 char col;

 col = 16;			/* number of columns */
 while (size != 0)		/* while not done */
 {
  --size;			/* count off data sent */
  if (col == 16)		/* if column 0 */
  {
   printf("%8x  ", (unsigned int) p); /* output address */
  }
  printf("%2x", *p++);		/* output value */
  --col;			/* count off a column */
  if (col)			/* if not end of line */
  {
//   if ((col & 1) == 0)		/* if even column */
   printf(" ");		/* output a space */
  }
  else				/* if end of line */
  {
   col = 16;			/* reset column counter */
   if (size != 0)		/* if not done */
    printf("\n");
  }
 }
 if (col != 0)
  printf("\n");
}
#include "class/vendor/vendor_device.h"
#endif
//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
static void cdc_task(void)
{
  uint8_t itf;

#if defined(LATHE_CPP)
#if CFG_TUD_VENDOR != 0
  if (tud_vendor_n_available(0))
  {
   uint8_t buf[64];

   int count = (int) tud_vendor_n_read(0, buf, sizeof(buf));

   prtbuf(buf, count);

   count = (int) tud_vendor_write_now(0, buf, count);
   printf("write %d\n", count);
  }
#endif  /* CFG_TUD_VENDOR */
#endif
  for (itf = 0; itf < CFG_TUD_CDC; itf++)
  {
    // connected() check for DTR bit
    // Most but not all terminal client set this when making connection
    // if ( tud_cdc_n_connected(itf) )
    {
      if ( tud_cdc_n_available(itf) )
      {
        uint8_t buf[64];

        uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

        // echo back to both serial ports
        echo_serial_port(0, buf, count);
        echo_serial_port(1, buf, count);
      }
    }
  }
}

#if defined(LATHE_USB)
void errHandler(void)
{
 __disable_irq();
 while (1)
 {
 }
}

#if 0
void Error_Handler(void)
{
 /* USER CODE BEGIN Error_Handler_Debug */
 /* User can add his own implementation to report the HAL error return state */
 __disable_irq();
 while (1)
 {
 }
 /* USER CODE END Error_Handler_Debug */
}
#endif
#endif

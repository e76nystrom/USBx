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

#include "serialio.h"
#include "stm32h7xx_hal.h"
#include "board.h"

#if defined(CLION)
#define __bss_start__ _sbss
#define __bss_end__ _ebss
#define __data_start__ _sdata
#define __data_end__ _edata
#define __stack _estack
#define __Main_Stack_Limit _end
#endif	/* CLION */
extern char __bss_start__;
extern char __bss_end__;
extern char __data_start__;
extern char __data_end__;
extern char __stack;
extern char __Main_Stack_Limit;

void stub(void);
_ssize_t write(int fd  __attribute__((unused)), const char* buf, _ssize_t nbyte);
unsigned int getSP(void);

//------------- prototypes -------------//
static void cdc_task(void);
/*------------- MAIN -------------*/
extern unsigned int _estack;

int main(void)
{
 stub();
 board_init();
 char buf[] = "_write\n";
 write(0, buf, sizeof(buf));
 putstr("start\n");

 //unsigned int bss = (unsigned int) (&__bss_end__ - &__bss_start__);
 //unsigned int data = (unsigned int) (&__data_end__ - &__data_start__);
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
// printf("data %u bss %u total %u\n", data, bss, data + bss);
// printf("stack %08x stackLimit %08x sp %08x\n",
//        (unsigned int) &__stack, (unsigned int) &__Main_Stack_Limit,
//        getSP());
// unsigned int sysClock = HAL_RCC_GetSysClockFreq();
// unsigned int clockFreq = HAL_RCC_GetHCLKFreq();
// unsigned int FCY = HAL_RCC_GetPCLK2Freq() * 2;
//  printf("sys clock %u clock frequency %u FCY %u\n",
//         sysClock, clockFreq, FCY);
// printf("sysTick load %d\n", (int) SysTick->LOAD);
 PRINT_FUNC();
 initCharBuf();
  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

 uint32_t t0 = HAL_GetTick();

  while (1)
  {
    tud_task(); // tinyusb device task
    cdc_task();
    pollBufChar();

   uint32_t t = HAL_GetTick();
   if ((t - t0) > 500) {
    t0 = t;
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
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

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
static void cdc_task(void)
{
  uint8_t itf;

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

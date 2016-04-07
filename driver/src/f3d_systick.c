/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Shichao Hu
 * Maintainer: Shichao Hu
 * Created: 3/31/2016
 * Last-Updated: 4/6/2016
 *           By: Shichao Hu
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_user_btn.h>
#include <f3d_uart.h>
#include <queue.h>

volatile int systick_flag = 0;
int i;

void f3d_systick_init(void) {
  // this call would produce generate 100 interrupts per second
  SysTick_Config(SystemCoreClock/100);
}

void SysTick_Handler(void) {

  //SysTick_Handler as the new delay function
  if(user_btn_read()){
    // if user button is pressed, the led circle goes to slow rate (10 interrupts/second)
    SysTick_Config(SystemCoreClock/10);
  }
  else{
    // if not, the led circle has fast rate of 100 interrupts per second
    SysTick_Config(SystemCoreClock/100);
  }

  // my led function
  f3d_led_off(i);
  i = (i + 1) % 9;
  f3d_led_on(i);

  /////////////////////////////////////////////
  if(!queue_empty(&txbuf)){
    flush_uart();
  }

}

/* f3d_systick.c ends here */

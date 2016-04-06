/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Nov 14 07:57:37 2013
 * Last-Updated: 
 *           By: 
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

volatile int systick_flag = 0;
int num;
int i;

f3d_user_btn_init();
f3d_led_init();

void f3d_systick_init(void) {
  // this call would produce generate 100 interrupts per second
  SysTick_Config(SystemCoreClock/100);
}

void SysTick_Handler(void) {
  if(user_btn_read()){
    f3d_led_on(i);
    i++;
    if(i>7){
      i = 0;
    }
  }
  else{
    
  }
}

/* f3d_systick.c ends here */

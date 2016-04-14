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
#include "queue.h"

extern queue_t txbuf;
extern queue_t rxbuf;
volatile int systick_flag = 0;
int counter = 0;
int i=0;

void f3d_systick_init(void) {
	f3d_led_init();
	f3d_user_btn_init();
	SysTick_Config(SystemCoreClock/SYSTICK_INT_SEC);
}


void SysTick_Handler(void) {
	if (!queue_empty(&txbuf)) {
    	flush_uart();
  	}
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) {
		f3d_led_all_off();
        	//while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
        	if(counter == 10){
                        counter = 0;
                        f3d_led_on(i);
                        i++;
                        if(i > 8){
                                i = 0;
				
                        }
                }
                counter++;	
       }
	 else {
		f3d_led_all_off();
                f3d_led_on(i);
                i++;
                if(i > 8){
                        i = 0;
			//f3d_led_all_off();
                }
        }
}

/* f3d_systick.c ends here */

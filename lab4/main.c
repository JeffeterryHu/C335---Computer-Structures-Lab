/*
 * main.c
 *
 * Author: Shichao Hu
 * Date Created: 2/4/2016
 * Last Modified by: Shichao Hu
 * Date Last Modified: 2/8/2016
 * Part of: Lab4
 * Partner: Kyle Dijkstra
 */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h> // include file for the local driver

// Simple looping delay function
void delay(void) {
	int i = 4000000;
  	while (i-- > 0) {
		asm("nop");
	}
}

int main(void) { 
  f3d_user_btn_init(); // initialize button
  f3d_led_init(); //initialize led
  int i = 0;
  while(1){
	while(!user_btn_read()){ // while button is not pushed, led will light in sequence with delay
		if(i>=0&&i<=7){
		f3d_led_off(i-1);
		f3d_led_on(i);
		i++;
		delay();
	}else{ // at the end of sequence, all led lights on and then off, restart cycle
		f3d_led_all_on();
		delay();
		f3d_led_all_off();
		delay();
		i = 0;
	}
	}
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */

/* main.c --- 
 * 
 * Filename: main.c
 * Description: lab 10 main.c
 * Author: Shichao Hu && Yangjun Liu
 * Created: 3/24/2016
 * Last Modified: 3/31/2016

/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <stdio.h>

int main(void) { 

  f3d_delay_init();
  f3d_led_init();
  f3d_user_btn_init();
  
  int led = 9;

  if(user_btn_read()){
    f3d_led_on(led);
    led++;
  }
  else{
    f3d_led_all_on();
  }


}
 
  /********************* Ends Here *************************/


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */

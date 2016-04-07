/* main.c --- 
 * 
 * Filename: main.c
 * Author: Shichao Hu
 * Partner: Broadmore Tung
 * Created: 3/31/2016
 * Last Modified: 4/6/2016

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
#include <f3d_systick.h>
#include <f3d_user_btn.h>
#include <queue.h>

int main(void) {
  f3d_led_init();
  f3d_user_btn_init();
  f3d_systick_init();
  f3d_uart_init();

  while(1){
  putchar(getchar());
  //printf("world\n");
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

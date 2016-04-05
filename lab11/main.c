/* main.c --- 
 * 
 * Filename: main.c
 * Description: lab 10 main.c
 * Author: Shichao Hu && Broadmore Tung
 * Created: 3/31/2016
 * Last Modified: 4/4/2016

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

int main(void) {
  f3d_led_init();
  f3d_user_btn_init();
  f3d_systick_init();
  int i = 0;

  while(1){
    if(!user_btn_read()){ // while button is not pushed, 100 interrupts/second
      SysTick_Config(SystemCoreClock / 100000);
      if(i>=0&&i<=7){
  	f3d_led_off(i-1);
  	f3d_led_on(i);
  	i++;
  	//delay();
  	SysTick_Handler();
      }
      else{
  	f3d_led_all_on();
        //delay();
  	SysTick_Handler();
  	f3d_led_all_off();
        //delay();
  	SysTick_Handler();
  	i = 0;
      }
    }
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

/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author:
 * Created:
 * Last-Updated: 
 *           By: 

 /* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) { 

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  f3d_timer2_init();
  f3d_dac_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_systick_init();
  f3d_lcd_init();

  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  f3d_lcd_fillScreen(WHITE);
  //delay(1000)
  f3d_lcd_fillScreen(BLUE);
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_fillScreen(BLUE);
  f3d_lcd_fillScreen(WHITE);

  uint16_t colors3[25] = { RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED };

 
  nunchuk_t n;

  while (1){
    
    // draw box
    f3d_lcd_setAddrWindow(64, 80, 64+4, 80+4, MADCTLGRAPHICS);
    f3d_lcd_pushColor(colors3, 25); 

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

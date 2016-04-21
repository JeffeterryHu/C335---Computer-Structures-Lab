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
#include <rect.h>
//#include <guy.h>


rect_t rectA,rectB,rectC,rectD,rectE,rectF,rectG,rectH,rectI,rectJ,rectK,rectL;
rect_t rectTree1,rectTree2,rectTree3;

int main(void) {

  ///////////////////////////////////////////
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
  f3d_user_btn_init();

  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  f3d_lcd_fillScreen(CYAN);

  //new fill screen
  /* f3d_lcd_fillScreen(WHITE); */
  /* f3d_lcd_fillScreen(BLUE); */
  /* f3d_lcd_fillScreen(WHITE); */
  /* f3d_lcd_fillScreen(BLUE); */
  /* f3d_lcd_fillScreen(WHITE); */
  ///////////////////////////////////////////

  //tree-1 bottom
  int g,h;
  g = 110;
  h = 155;
  initRect(&rectTree1,g,h,5,5,BLACK);
  //tree-2 mid
  int g2,h2;
  g2 = 105;
  h2 = 145;
  initRect(&rectTree2,g2,h2,15,10,GREEN);
  //tree-3 top
  int g3,h3;
  g3 = 110;
  h3 = 140;
  initRect(&rectTree3,g3,h3,5,5,GREEN);

  //cloud1-bottom
  int a,b;
  a = 64;
  b = 55;
  initRect(&rectI,a,b,20,5,WHITE);

  //cloud1-top
  int a1,b1;
  a1 = 69;
  b1 = 50;
  initRect(&rectJ,a1,b1,10,5,WHITE);

  //cloud2-bottom
  int a2,b2;
  a2 = 100;
  b2 = 35;
  initRect(&rectL,a2,b2,20,5,WHITE);

  //cloud2-top
  int a3,b3;
  a3 = 105;
  b3 = 30;
  initRect(&rectK,a3,b3,10,5,WHITE);

  //left eye
  int x4,y4;
  x4 = 5;
  y4 = 135;
  initRect(&rectG,x4,y4,5,5,BLACK);

  //right eye
  int x5,y5;
  x5 = 15;
  y5 = 135;
  initRect(&rectH,x5,y5,5,5,BLACK);

  //head
  int m1,n1;
  m1 = 5;
  n1 = 130;
  initRect(&rectF,m1,n1,15,15,YELLOW);

  //left hand
  int x2,y2;
  x2 = 0;
  y2 = 145;
  initRect(&rectD,x2,y2,5,5,YELLOW);

  //right hand
  int x3,y3;
  x3 = 20;
  y3 = 145;
  initRect(&rectE,x3,y3,5,5,YELLOW);

  //left foot
  int x,y;
  x = 5;
  y = 160;
  initRect(&rectA,x,y,5,5,BLACK);

  //right foot
  int x1,y1;
  x1 = 15;
  y1 = 160;
  initRect(&rectC,x1,y1,5,5,BLACK);

  //body
  int m,n;
  m = 5;
  n = 145;
  initRect(&rectB,m,n,15,10,RED);

  while (1){

    //int guy();

    while(user_btn_read()){

      for(x = 110;x>0;x--){
	moveRect(&rectTree1,-1,0,CYAN);
	moveRect(&rectTree2,-1,0,CYAN);
	moveRect(&rectTree3,-1,0,CYAN);
	delay(10);
      }

      for(y=160;y>80;y--){
	moveRect(&rectA,0,-1,CYAN);
	moveRect(&rectB,0,-1,CYAN);
	moveRect(&rectC,0,-1,CYAN);
	moveRect(&rectD,0,-1,CYAN);
	moveRect(&rectE,0,-1,CYAN);
	moveRect(&rectF,0,-1,CYAN);
	moveRect(&rectG,0,-1,CYAN);
	moveRect(&rectH,0,-1,CYAN);
	delay(1);
      }
      for(y=80;y<160;y++){
	moveRect(&rectA,0,1,CYAN);
	moveRect(&rectB,0,1,CYAN);
	moveRect(&rectC,0,1,CYAN);
	moveRect(&rectD,0,1,CYAN);
	moveRect(&rectE,0,1,CYAN);
	moveRect(&rectF,0,1,CYAN);
	moveRect(&rectG,0,1,CYAN);
	moveRect(&rectH,0,1,CYAN);
	delay(1);
	// int guy_jump();
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

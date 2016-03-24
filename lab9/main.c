/*
  main.c
 
  Author: Shichao Hu
  Date Created: March 20, 2016
  Last Modified By: Shichao Hu
  Date Last Modified: March 23, 2016
  Assignment: Lab9
*/

#include <stm32f30x.h> 
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_delay.h>
#include <stdio.h>
#include <math.h> //added for trig functions

#define TIMER 20000
#define PI 3.14159265359
#define TILT 1
#define COMPASS 0
#define tilt(A) (asin(A/9.8))

double heading(float *results) {
  return atan2f(results[1], results[0]) * (180 / PI) + 180;
}

/* EXTRA CREDIT */
/* tilt(x,y,z,double*[]) */
/*
void tiltCompensation(double X, double Y, double Z, double * r) {
  double x_heading, y_heading;
  x_heading = X * cos(Y) + Z * sin(Y);
  y_heading = X * sin(X) * sin(Y) + Y * cos(X) - Z * sin(X) * cos(Y);
  r[0] = x_heading;
  r[1] = y_heading;
  heading(r);
}
//*/
/* EXTRA CREDIT END */

int circle[2];

int main(void) {
  f3d_uart_init();
  f3d_lcd_init();
  f3d_delay_init();
  f3d_i2c1_init();
  f3d_user_btn_init();
  f3d_accel_init();
  f3d_mag_init();
 
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  // Set up your inits before you go ahead
  f3d_lcd_fillScreen2(WHITE);  
  circle[0] = 64;
  circle[1] = 80;
  int mode = 0;
  double oA = -1.0;
  float magData[3];
  float accelData[3];
  double ax, ay;
  while(1){
    //mode will change
    if(user_btn_read()) {
      while(user_btn_read());
      mode = ++mode % 2;
      f3d_lcd_fillScreen2(WHITE);
    }    
    switch(mode){
    case COMPASS:
      f3d_mag_read(magData);
      f3d_accel_read(accelData);
      f3d_lcd_drawChar(64, 1, 'N', BLACK, WHITE);
      f3d_lcd_drawChar(122, 80, 'E', BLACK, WHITE);
      f3d_lcd_drawChar(64, 152, 'S', BLACK, WHITE);
      f3d_lcd_drawChar(1, 80, 'W', BLACK, WHITE);
      printf("Heading: %G\n",heading(magData));
      //cleans the line
      if(oA > 0){
	f3d_lcd_drawCompass(oA,WHITE);
      }
      //draws the line
      oA = heading(magData);
      f3d_lcd_drawCompass(oA,BLACK);
      delay(50);
      break;

    case TILT:

      f3d_accel_read(accelData);
      f3d_lcd_setAddrWindow(circle[0]-10,circle[1]-10,circle[0]+10,circle[1]+10,MADCTLGRAPHICS);
      int i;
      uint16_t x[21];
      //assign colors
      for(i = 0; i < 21;i++) x[i]=WHITE;
      //changes colors
      for(i = 0; i < 21;i++) f3d_lcd_pushColor(x,21);
      
      ax = tilt(accelData[0]);
      ay = tilt(accelData[1]);

      printf("ACCEL: %f,%f\n",ax,ay);
      // draw circle
      circle[0] = circle[0]-ay*150;
      circle[1] = circle[1]-ax*150;
      //the below is collision detection
      if(circle[0] > 118) circle[0] = 118;
      if(circle[0] < 10) circle[0] = 10;
      if(circle[1] > 150) circle[1] = 150;
      if(circle[1] < 10) circle[1] = 10;
      f3d_lcd_drawCircle(circle[0], circle[1], 10, BLACK);       
      
      
      //f3d_delay_uS(75);
      delay(33);
      break;
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



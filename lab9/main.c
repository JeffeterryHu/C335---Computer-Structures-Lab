/*
  main.c
 
  Author: Shichao Hu
  Date Created: March 20, 2016
  Last Modified By: Shichao Hu
  Date Last Modified: March 23, 2016
  Assignment: Lab9
*/

#include <stm32f30x.h> 
#include <f3d_led.h>
#include <f3d_gyro.h>
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_delay.h>
#include <stdio.h>
#include <math.h> //added for trig functions
#include <stdlib.h>

#define TIMER 20000
#define PI 3.14159265359
#define TILT 1
#define COMPASS 0
#define GYRO 2
#define tilt(A) (asin(A/9.8))

double heading(float *results) {
  return atan2f(results[1], results[0]) * (180 / PI) + 180;
}

void dada(int i) {
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}


void Draw_Box(uint16_t color, int value, int start_x, int start_y) { //draws box relative to value passed.
  int x;
  int y;
  for(y = start_y; y < start_y + 15; y++) {
    for(x = start_x; x < start_x + value; x++) {
      f3d_lcd_drawPixel(x,y,color);
    }
  }  
}

int Int_Maker(float f) { //turns our abs(axis float) into int between 0-100.
  int counter = 0;
  float v = abs(f);
  float k = 0.0; 
  while(v > k) {
    k += 4.3;
    counter += 1;
    if(counter > 99) {
      return counter;
    }
  }
  return counter;
}

int circle[2];

int main(void) {
  f3d_gyro_init();
  f3d_led_init();
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
  f3d_lcd_fillScreen2(YELLOW);  
  circle[0] = 64;
  circle[1] = 80;
  int mode = 0;
  double oA = -1.0;
  float magData[3];
  float accelData[3];
  double ax, ay;
  float degrees;
  int led;
  while(1){

    
    float axis[3];
    f3d_gyro_getdata(axis); //Fills our axis with our 3 values.
    //
    
    //
    int x_axis = Int_Maker(axis[0]);
    int y_axis = Int_Maker(axis[1] - 1.4); //assigns each axis to Int value.
    int z_axis = Int_Maker(axis[2]);
    //
    char buff1[50];
    char buff2[50]; //create text buffer.
    char buff3[50];
    //
    int reta = sprintf(buff1,"X axis: %4.2f",axis[0]);
    int retb = sprintf(buff2,"Y axis: %4.2f",axis[1]-1.5); //fill said buffer.
    int retc = sprintf(buff3,"Z axis: %4.2f",axis[2]);
    //

    //mode will change
    if(user_btn_read()) {
      while(user_btn_read());
      mode = ++mode % 2;
      f3d_lcd_fillScreen2(YELLOW);
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
	f3d_lcd_drawCompass(oA,YELLOW);
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
      for(i = 0; i < 21;i++) x[i]=YELLOW;
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

    case GYRO:

      f3d_lcd_drawStringH(10,20,buff1,RED,BLACK); //draw string for X
      Draw_Box(RED,x_axis,10,30); //draw box - width by value
      f3d_lcd_drawStringH(10,60,buff2,GREEN,BLACK); //draw string for Y
      Draw_Box(GREEN,y_axis,10,70); //draw box
      f3d_lcd_drawStringH(10,100,buff3,BLUE,BLACK); //draw string for Z
      Draw_Box(BLUE,z_axis,10,110); //draw box
      //dada(500000);
      //
      Draw_Box(BLACK,100,10,30);
      Draw_Box(BLACK,100,10,70); //cleans the boxes - turning all previous pixels off.
      Draw_Box(BLACK,100,10,110);
      //
    

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



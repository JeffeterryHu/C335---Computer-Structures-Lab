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
#include <f3d_nunchuk.h>
#define TIMER 20000
#define PI 3.14159265359
#define COMPASS 0
#define TILT 1
#define GYRO 2
#define NUNCHUK 3
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
  f3d_nunchuk_init();
 
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
  float roll;
  float pitch;
  float nunchuk_pitch;  
  float nunchuk_roll;

  // strings for printing
  unsigned char string_roll[50];
  unsigned char string_pitch[50];
  unsigned char string_heading[50];
  unsigned char string_heading_degrees[50];

  //colors
  uint16_t colors[25] =  {YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW};
  uint16_t colors2[25] = {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};
  uint16_t colors3[25] = { RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED };
  uint16_t x0,y0,x1,y1,Dx0,Dy0,Dx1,Dy1,oldx0,oldx1,oldy0,oldy1;


  while(1){
    float axis[3];
    f3d_gyro_getdata(axis); //Fills our axis with our 3 values.

    // get pitch and roll
    float x_a = accelData[0];
    float y_a = accelData[1];
    float z_a = accelData[2];

    //
    //
    // nunchuk pitch and roll
    nunchuk_t nunchuk;
    f3d_nunchuk_read(&nunchuk);

    float nunchuk_ax = ((float) nunchuk.ax - 500.0)/250.0;
    float nunchuk_ay = ((float) nunchuk.ay - 500.0)/250.0;
    float nunchuk_az = ((float) nunchuk.az - 700.0)/250.0;
    
    unsigned char jx = nunchuk.jx;
    unsigned char c = nunchuk.c;
    unsigned char z = nunchuk.z;

    nunchuk_pitch = (atanf(nunchuk_ax/sqrt(pow(nunchuk_ay,2) + pow(nunchuk_az,2))));
    nunchuk_roll = (atanf(nunchuk_ay/sqrt(pow(nunchuk_ax,2) + pow(nunchuk_az,2))));

    sprintf( string_pitch, "Pitch = %f", nunchuk_pitch);
    sprintf( string_roll, "Roll = %f",   nunchuk_roll); 
    //
    //
    //

    pitch = atanf(x_a/sqrt(pow(y_a,2) + pow(z_a,2)));
    roll  = atanf(y_a/sqrt(pow(x_a,2) + pow(z_a,2)));
 
    sprintf( string_pitch, "Pitch = %f", pitch);
    sprintf( string_roll, "Roll = %f",   roll); 

    /*
    float x_m = magData[0];
    float y_m = magData[1];
    float z_m = magData[2];

    float Xh = x_m * cos(pitch) + z_m * sin(pitch);
    float Yh = x_m * sin(roll)*sin(pitch) + y_m * cos(roll) - z_m * sin(roll) * cos(pitch);
    heading = atan2f(Yh,Xh);
    */

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
   

    //mode will change by pressing userbutton
    if(user_btn_read()) {
      while(user_btn_read());
      mode = ++mode % 4;
      f3d_lcd_fillScreen2(YELLOW);
    }

    /*
    //
    // use nunchuk to control the graphical display
    while(jx >= 0 && jx < 127){
      // joystick left
      while(mode>0 && mode<4){
	mode--;
      }
    }
    while(jx > 127 && jx <= 256){ 
      // joystick right
      while(mode>0 && mode<4){
	mode++;
      }
    }
    while(c){
      while(mode>0 && mode<4){
	mode--;
      }
    }
    while(z){
      while(mode>0 && mode<4){
	mode++;
      }
    }
    */

    if ( jx > 180 || c == 1){
      f3d_lcd_fillScreen(YELLOW);
      delay(100);
      if(mode < 3){
	mode++;
      }
      else mode = 0;
    }
    else if(jx < 60 || z == 1){
      f3d_lcd_fillScreen(YELLOW);
      delay(100);
      if(mode == 0){
	mode = 3;
      }
      else mode --;
    }

    // different displays
    switch(mode){  
    case COMPASS:
      f3d_mag_read(magData);
      f3d_accel_read(accelData);
      f3d_lcd_drawChar(64, 1, 'N', BLACK, YELLOW);
      f3d_lcd_drawChar(122, 80, 'E', BLACK, YELLOW);
      f3d_lcd_drawChar(64, 152, 'S', BLACK, YELLOW);
      f3d_lcd_drawChar(1, 80, 'W', BLACK, YELLOW);
      //printf("Heading: %G\n",heading(accelData));
      //cleans the line
      if(oA > 0){
	f3d_lcd_drawCompass(oA,YELLOW);
      }
      //draws the line
      oA = heading(magData);
      f3d_lcd_drawCompass(oA,BLACK);
      delay(50);
      /*
	degrees = heading * (180/M_PI) + 180;
	led = (int) ((degrees + 22.5) / 45);
	f3d_led_all_off();
	f3d_led_on(led); 
      */
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

      // printf("ACCEL: %f,%f\n",ax,ay);
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

      f3d_lcd_drawStringH(10,20,buff1,RED,YELLOW); //draw string for X
      Draw_Box(RED,x_axis,10,30); //draw box - width by value
      f3d_lcd_drawStringH(10,60,buff2,BLACK,YELLOW); //draw string for Y
      Draw_Box(GREEN,y_axis,10,70); //draw box
      f3d_lcd_drawStringH(10,100,buff3,BLUE,YELLOW); //draw string for Z
      Draw_Box(BLUE,z_axis,10,110); //draw box
      //dada(500000);
      //
      Draw_Box(BLACK,100,10,30);
      Draw_Box(BLACK,100,10,70); //cleans the boxes - turning all previous pixels off.
      Draw_Box(BLACK,100,10,110);
      //
    
    case NUNCHUK:
        
      f3d_lcd_setAddrWindow(64, 80, 64+4, 80+4, MADCTLGRAPHICS);
      f3d_lcd_pushColor(colors3, 25); 


      x0 = 64+(nunchuk_roll*35)   ;
      y0 = 80+(nunchuk_pitch*43)  ;    
      x1 = 64+(nunchuk_roll*35)   +4;
      y1 = 80+(nunchuk_roll*43)   +4;

      f3d_lcd_setAddrWindow(x0, y0, x1, y1, MADCTLGRAPHICS);
      f3d_lcd_pushColor(colors2, 25); 
      printf("Nunchuk: %c, %c, %c\n", jx,c,z);
      delay(50); 

      f3d_lcd_setAddrWindow(x0, y0, x1, y1, MADCTLGRAPHICS);
      f3d_lcd_pushColor(colors, 25); 

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



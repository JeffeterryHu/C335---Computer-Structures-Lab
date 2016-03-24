/* main.c --- 
 * 
 * Filename: main.c
 * Description:

Draw_Box(~) is our method that draws a box - height: 15, and width dependent on the int value passed. as int increases, box grows rightward on our screen.

Int_Maker(~) turns a +/- float into a abs(int) from 0-100. So that the max width of our Draw_Box is only 100 (as to not overflow our guy). Remainder of process is defined step by step on our main guy.

 * Author: Nathan Krummen
 * Partner: Shichao Hu
 * Date Created: 2/25/2016
 * Date Last Modified: 3/3/2016
 * Part of: Lab7
 */


 /* Code: */


#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>
#include <f3d_user_btn.h>
#include <stdlib.h>

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

#define TIMER 20000

int main(void) { 
  f3d_lcd_init();
  f3d_uart_init();
  f3d_user_btn_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_gyro_init();
  f3d_led_init();
  f3d_lcd_fillScreen2(WHITE); 
  //create tiles:
  while(1) {
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
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */

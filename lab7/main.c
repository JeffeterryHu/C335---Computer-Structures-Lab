/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
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


void Write_Titles() {
  f3d_lcd_drawString(20,20,"X Axis: ",BLUE,BLACK);
  f3d_lcd_drawString(20,70,"Y Axis: ",GREEN,BLACK);
  f3d_lcd_drawString(20,120,"Z Axis: ",RED,BLACK);
}

int absvol(float f) {
  if(f< 0.0) {
    f *= -1.0;
  }
  return f;
  }



int Int_Maker(float f) {
  float v = absvol(f);
  float k = 0.0;
  int counter = 0;
  while(v > k) {
    k += 4.0;
    counter += 1;
    if(counter > 100) {
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
  
  int i = 0;
  float axis[3];
  
  
  if(user_btn_read()) { //screen wash.
    //f3d_lcd_fillScreen(BLACK);
    //f3d_lcd_fillScreen(WHITE);
    //f3d_lcd_fillScreen(BLACK);
  }
  
  //create tiles:
  
  
  float x_axis = axis[0];
  float y_axis = axis[1];
  float z_axis = axis[2];

  int size = Int_Maker(x_axis);
  int counter = 0;

  while(1) {
    f3d_gyro_getdata(axis);

    //dada(1000000);
    
    f3d_lcd_fillScreen(BLACK);
    Write_Titles();
    //                  V , X, Y, X,  Y //
    f3d_lcd_OURGUY(BLUE,20,10,30,50+counter,50+counter);
    f3d_lcd_OURGUY(BLUE,20,10,80,50+counter,50+counter);
    f3d_lcd_OURGUY(BLUE,20,10,130,50+counter,50+counter);
    printf("axis: %f\n",axis[0]);
    counter += 1;
    }
}

  
  //CODE IS BELOW
  /*
  float axis[3];
  int button_counter = 0; //ranges from 0-6, reset at 6.
  char sample[] = "SAMPLE";
  while(button_counter == 0) {
  f3d_gyro_getdata(axis); //We've filled axis with x,y,z, values from Gyro
  }
  //f3d_led_all_on();
  //f3d_lcd_drawString(75,50,sample,RED,BLACK);
  f3d_lcd_drawChar(100,100,'b',RED,BLUE);
  */


  //END CODE

 


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */

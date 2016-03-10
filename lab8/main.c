/* main.c ---
*
* Filename: main.c
* Description: main program to build a compass
* Author: Robert Mulligan
* Maintainer: Robert Mulligan
* Created: Thu Feb 7 2016
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <stdio.h>
#include <math.h>
#include <f3d_led.h>


#define TIMER 20000

int main(void) {


  f3d_uart_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  // Set up your inits before you go ahead
  f3d_i2c1_init();
  delay(10); 
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10); 
  f3d_led_init();


  // Setup buffers
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);



  // floats arrays
  float mag_data[3];
  float accel_data[3];

  // strings for printing
  unsigned char string_roll[50];
  unsigned char string_pitch[50];
  unsigned char string_heading[50];
  unsigned char string_heading_degrees[50];

  float roll;
  float pitch;
  float heading;
  float degrees;   
  int led;

  uint16_t colors[25] =  {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};
  uint16_t colors2[25] = {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};
  uint16_t colors3[25] = { RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED };


  f3d_lcd_fillScreen(WHITE);



  uint16_t x0,y0,x1,y1,Dx0,Dy0,Dx1,Dy1,oldx0,oldx1,oldy0,oldy1;


  while(1) {
  
    f3d_accel_read(accel_data); 
    f3d_mag_read(mag_data);

    float x_a = accel_data[0];
    float y_a = accel_data[1];
    float z_a = accel_data[2];
    
    pitch = atanf(x_a/sqrt(pow(y_a,2) + pow(z_a,2)));
    roll  = atanf(y_a/sqrt(pow(x_a,2) + pow(z_a,2)));
 
    sprintf( string_pitch, "Pitch = %f", pitch);
    sprintf( string_roll, "Roll = %f",   roll); 

    // f3d_lcd_drawString(10,60, string_roll, WHITE, BLACK);
    // f3d_lcd_drawString(10,70, string_pitch, WHITE, BLACK);

    float x_m = mag_data[0];
    float y_m = mag_data[1];
    float z_m = mag_data[2];
 
    /* float M_x = (x_m*cos(pitch) + z_m*sin(pitch)); */
    /* float M_y = (x_m*sin(roll)*sin(pitch) + y_m*cos(roll) + z_m*sin(roll)*cos(pitch)); */
    /* float M_z = -(x_m*cos(roll)*sin(pitch) + y_m*sin(roll) + z_m*cos(roll)*cos(pitch)); *
    
    /* if(M_x > 0 && M_y >= 0){ */
    /* heading = atan(M_y/M_x); */
    /* }else if(M_x<0){ */
    /* heading = (M_PI + atan(M_y/M_x)); */
    /* }else if(M_x > 0 && M_y <= 0){ */
    /* heading = (2*M_PI + atan(M_y/M_x)); */
    /* }else if(M_x = 0 && M_y < 0){ */
    /* heading = M_PI/2; */
    /* }else if(M_x = 0 && M_y > 0){ */
    /* heading = M_PI*1.5; */
    /* }else{ */
    /* f3d_lcd_drawString(20,100, "Failure" , WHITE, BLACK); */
    /* } */

    float Xh = x_m * cos(pitch) + z_m * sin(pitch);
    float Yh = x_m * sin(roll)*sin(pitch) + y_m * cos(roll) - z_m * sin(roll) * cos(pitch);

    heading = atan2f(Yh,Xh);

    //Equations 2;
    // page 7 on the LSM 303DLH for tilt compensate 
    //Xh and Yh of equations 2 you can get heading = arctan2(Yh, Xh)*180/M_PI + 
    
    //sprintf(string_heading,"Heading is %f", heading);
    //f3d_lcd_drawString(10, 120, string_heading, BLACK, WHITE);

    degrees = heading * (180/M_PI) + 180;

    led = (int) ((degrees + 22.5) / 45);
    f3d_led_all_off();
    f3d_led_on(led); 

    
    /* if(degrees >= 0 && degrees < 45) { f3d_led_all_off(); f3d_led_on(0); } */
    /* else if(degrees >= 45 && degrees < 90) { f3d_led_all_off(); f3d_led_on(1); } */
    /* else if(degrees >= 90 && degrees < 135) { f3d_led_all_off(); f3d_led_on(2); } */
    /* else if(degrees >= 135 && degrees < 180) { f3d_led_all_off(); f3d_led_on(3); } */
    /* else if(degrees >= 180 && degrees < 225) { f3d_led_all_off(); f3d_led_on(4); } */
    /* else if(degrees >= 225 && degrees < 270 ) { f3d_led_all_off(); f3d_led_on(5); } */
    /* else if(degrees >= 270 && degrees < 315) { f3d_led_all_off(); f3d_led_on(6); } */
    /* else if(degrees >= 315 && degrees < 360 ) { f3d_led_all_off(); f3d_led_on(7); } */



     /* sprintf(string_heading_degrees,"Heading in degrees is %f", (heading * (180 / M_PI)+180));  */
     /* f3d_lcd_drawString(10, 130, string_heading_degrees, BLACK, WHITE);  */

    /* int i, j; */
    /* for(i = 0; i < 10; i++){ */
    /*   for(j = 0; j < 10; j++){ */
    /* 	f3d_lcd_drawPixel(64+i, 80+j, BLUE); */
    /*   } */
    /* } */

    f3d_lcd_setAddrWindow(64, 80, 64+4, 80+4, MADCTLGRAPHICS);
    f3d_lcd_pushColor(colors3, 25); 


    x0 = 64+(roll*35)   ;
    y0 = 80+(pitch*43)  ;    
    x1 = 64+(roll*35)   +4;
    y1 = 80+(roll*43)   +4;

    f3d_lcd_setAddrWindow(x0, y0, x1, y1, MADCTLGRAPHICS);
    f3d_lcd_pushColor(colors2, 25); 

    delay(225); 

    f3d_lcd_setAddrWindow(x0, y0, x1, y1, MADCTLGRAPHICS);
    f3d_lcd_pushColor(colors, 25); 

    /* for(i = 0; i < 5; i++){ */
    /*   for(j = 0; j < 5; j++){ */
    /* 	f3d_lcd_setAddrWindow( */
    /* 	f3d_lcd_drawPixel(64+(roll*25)+i, 80+(25*pitch)+j, RED);  */
    /*   } */
    /* } */




  }
}

/*
  unsigned char string_mag_data[50];
  unsigned char string_accel_data[50];
  
  while(1) {
  

  f3d_accel_read(accel_data); 
  f3d_mag_read(mag_data);
  
  
  sprintf(string_mag_data,"mag_data:\nx = %4f\ny = %4f\nz = %4f", mag_data[0],mag_data[1],mag_data[2]); 
  sprintf(string_accel_data,"Accel_data:\nx = %4f\ny = %4f\nz = %4f", accel_data[0], accel_data[1], accel_data[2]); 
  
  
  f3d_lcd_drawString(10, 40, string_mag_data, WHITE, BLACK); 
  f3d_lcd_drawString(10, 100, string_accel_data, WHITE, BLACK);
  }
*/



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */

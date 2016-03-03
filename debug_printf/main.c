
/* main.c
 *
 * Name: Shichao Hu
 * Partner: Nathan Krummen
 * Date Created: 2/25/2016
 * Date Last Modified: 3/3/2016
 * Part of: Lab7
 */	 

/*

Nathan Krummen
This loops through like 400 printfs per cycle, resulting in probably a 1/3rd of a second delay between lights on/lights off. Printf takes a decent amount of time to process being the point of this assignment me thinks.

 */
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>
#include <stdlib.h>

void inits(void) {
  f3d_led_init();
  f3d_user_btn_init();
}
int i = 0;

void main(void) {
  inits();  
  while(1) {
    f3d_led_all_on();
    int k;
    for(k = 0; k < 100; k++) {
    printf("abc");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    }
    f3d_led_all_off();
    int kk;
    for(kk = 0; kk < 100; kk++) {
    printf("cba");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
    printf("asdfdslf;j;slkfj;as;jfalsjfksladjfsldkf");
  }
  }

}


void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */

  printf("Error - Check Assert_failed");
  while (1);
}

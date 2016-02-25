//main.c for lab6

/* main.c
 * 
 * Author: Shichao Hu
 * Partner: Broadmore Chang Tung
 * Date Created: 2/18/2016
 * Last Modified by: Shichao Hu
 * Date Last Modified: 2/25/2016
 * Part of: Lab6
 */

#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>
#include <stm32f30x.h>

// delay function
void delay(void){
	int i = 800000;
	while(i-- > 0){
		asm("nop");
	}
}

/*
void choice(int i){
	char ch;
  	scanf("%c",&ch);
 	if(ch == 'x'){
    		i= 1;
  	} else if (ch == 'y'){
    		i= 2;
  	} else if (ch == 'z'){
    		i= 3;
  	} else {
    		i = 0;
	}
}
*/

/*
int dpg;

void LightLED(int dpg){
	int min = 0;
	int max = 0;
	int step = 500 / 5;
	
	f3d_led_on(9);
	
}
*/

// main function
int main(void){

  //initializing button
  GPIO_InitTypeDef GPIO_InitStructure;

  int button_state;
  int i=1;
  float Data[3] = {0};
  float eData[3] = {0};
  int example = 0;

  f3d_uart_init();
  f3d_led_init();
  f3d_gyro_init();
  f3d_user_btn_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  printf("Angular Velocity Visualization Application\n");
  printf("Press user button will start the app and change the axises showing.\n");  

  while(1){

  	button_state = user_btn_read();

	if(button_state){
		example++;
		printf("%d\n",example);
		delay();
	}
	

	//char ch;
	//ch = getchar();
	//char ch;
	//scanf("%c",&ch);

	//ch = getchar();

	while(example%4 == 1 /*|| ch == 'x'*/) {
		f3d_gyro_getdata(Data); 
	      	printf("X axis: %f\n", Data[0]);
	      	delay();

	if(Data[0] > 0 && Data[0] < 100){	
		
		if(Data[0] > 0 && Data[0] < 10){
			f3d_led_on(0);
			delay();
			f3d_led_off(0);
		}
		
		else if(Data[0] > 10 && Data[0] < 30){
			f3d_led_on(5);
			f3d_led_on(6);
			delay();
			f3d_led_off(5);
			f3d_led_off(6);
		}

		else{
                        f3d_led_on(4);
			f3d_led_on(5);
			f3d_led_on(6);
			f3d_led_on(7);
			f3d_led_on(0);
                	delay();
			f3d_led_off(0);
			f3d_led_off(4);
			f3d_led_off(5);
			f3d_led_off(6);
			f3d_led_off(7);
		}
	}

	if(Data[0] <= 0 && Data[0] > -100){
		
		if(Data[0] <= 0){
                        f3d_led_on(2);
                	delay();
                        f3d_led_off(2);
                }

                else if(Data[0] < -30){
                        f3d_led_on(1);
                        f3d_led_on(3);
                	delay();
                        f3d_led_off(1);
                        f3d_led_off(3);
                }

                else{
                        f3d_led_on(0);
                        f3d_led_on(1);
                        f3d_led_on(2);
                        f3d_led_on(3);
                        f3d_led_on(4);
                	delay();
                        f3d_led_off(0);
                        f3d_led_off(1);
                        f3d_led_off(2);
                        f3d_led_off(3);
                        f3d_led_off(4);
                }
	}



		if (user_btn_read()) {
			example++;
			//delay();
			f3d_gyro_getdata(Data);
		}	
	}

	while(example%4 == 2 /*|| ch == 'y'*/){
		f3d_gyro_getdata(Data);
		printf("Y axis: %f\n", Data[1]);
		delay();
		
		/*
                if(ch == 'x'){
                	example--;
                }
                else if(ch == 'y'){
                }
                else if(ch == 'z'){
                	example++;
                }
                else{}
		*/

		if(user_btn_read()){
			example++;
			//delay();
			f3d_gyro_getdata(Data);
		}
	}

	while(example%4 == 3 /*|| ch == 'z'*/){
		f3d_gyro_getdata(Data);
		printf("Z axis: %f\n", Data[2]);
		delay();
	
		/*
                if(ch == 'x'){
                        example -= 2;
                }
                else if(ch == 'y'){
			example += 3;
                }
                else if(ch == 'z'){
                }
                else{}
		*/
	
		if(user_btn_read()){
			example++;
			//delay();
			f3d_gyro_getdata(Data);
		}
	}
  }
}
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}

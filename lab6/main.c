//main.c for lab6
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>
#include <stm32f30x.h>

// delay function
void delay(void){
	int i = 1800000;
	while(i-- > 0){
		asm("nop");
	}
}

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
	
	while(example%4 == 1) {
		f3d_gyro_getdata(Data); 
	      	printf("X axis: %f\n", Data[0]);
	      	delay();
		     
		if (user_btn_read()) {
			example++;
			delay();
			f3d_gyro_getdata(Data);
		}
		
		/*
		if(Data[0] > 5){
			f3d_led_on(0);
		}else{
			f3d_led_off(0);
		}

		if(Data[0] >63){
			f3d_led_on(1);
		}else{
			f3d_led_off(1);
		}

		if(Data[0] > 111){
			f3d_led_on(2);
		} else {
			f3d_led_off(2);
		}

		if(Data[0] > 169){
			f3d_led_on(3);
		} else {
			f3d_led_off(3);
		}

		if(Data[0] > 217){
			f3d_led_on(4);
		} else{
			f3d_led_off(4);
		}

		if(Data[0] < -63){
			f3d_led_on(7);
		}else{
			f3d_led_off(7);
		}

		if(Data[0] < -111){
			f3d_led_on(6);
		}else{
			f3d_led_off(6);
		}

		if(Data[0] < -169){
			f3d_led_on(5);
		}else{
			f3d_led_off(5);
		}

		*/
	}

	while(example%4 == 2){
		f3d_gyro_getdata(Data);
		printf("Y axis: %f\n", Data[1]);
		delay();
		
		if(user_btn_read()){
			example++;
			delay();
			f3d_gyro_getdata(Data);
		}
	}

	while(example%4 == 3){
		f3d_gyro_getdata(Data);
		printf("Z axis: %f\n", Data[2]);
		delay();
		
		if(user_btn_read()){
			example++;
			delay();
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

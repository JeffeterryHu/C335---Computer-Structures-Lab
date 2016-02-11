/**********************************
 *f3d_user_btn.c 
 *contains the init and read functions for the User Button
 *********************************/

/*
 * f3d_led.c
 *
 * Author: Shichao Hu
 * Date Created: 2/4/2016
 * Last Modified by: Shichao Hu
 * Date Last Modified: 2/9/2016
 * Part of: Lab4
 * Partner: Kyle Dijkstra
 */


#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>


/*Initialization of the UserButton*/
void f3d_user_btn_init(void){
	
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
  	//GPIO_StructInit(&GPIO_InitStructure);
  	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	//GPIO_Init(GPIOE, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*reads the User Button*/
int user_btn_read(void){
       	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);  
}

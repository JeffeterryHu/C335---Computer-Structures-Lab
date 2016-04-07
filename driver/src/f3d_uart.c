/************************** 
 *f3d_uart.c 
 *contains the initialization basic i/o functions for UART
****************************/ 

/*
 * f3d_uart.c
 *
 * Created on: 02/11/2016
 * Author: Shichao Hu
 * Partner: Bradmore Tung
 * Last Modified: 4/5/2016
 * Part of: Lab11
 *
 */

/* Code: */

#include <stm32f30x.h>
#include <f3d_uart.h>
#include <queue.h>
#include <string.h>


//the initialization function to call
void f3d_uart_init(void) {

  /////////////////////////////////// NEW CODE //////////////////////////////////////////////
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC , &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1 ,&USART_InitStructure);
  USART_Cmd(USART1 , ENABLE);

  // Initialize the rx and tx queues
  init_queue(&rxbuf);
  init_queue(&txbuf);

  // Setup the NVIC priority and subpriority
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // Enable the RX interrupt
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  ///////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////OLD CODE//////////////////////////////////////////
  /* Initialize the clocks */
  /* RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); */
  /* RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); */
  

  /* /\* Initialize the RX and TX Pins *\/ */
  /* GPIO_InitTypeDef GPIO_InitStructure; */
  /* GPIO_StructInit(&GPIO_InitStructure); */
  /* GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; */
  /* GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; */
  /* GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; */
  /* GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; */
  /* GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; */
  /* GPIO_Init(GPIOC,&GPIO_InitStructure); */

  /* GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7); */
  /* GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7); */
  
  /* /\* UART Initialization *\/ */
  /* USART_InitTypeDef USART_InitStructure; */
  /* USART_StructInit(&USART_InitStructure); */
  /* USART_InitStructure.USART_BaudRate = 9600; */
  /* USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; */
  /* USART_Init(USART1 ,&USART_InitStructure); */
  /* USART_Cmd(USART1 , ENABLE); */
  ///////////////////////////////////////////////////////////////////////////////////////////

}

int putchar (int c){
  if(c != 0){
    enqueue(&txbuf, c);
  }
  return c;
}

/* int putchar (int c) */
/* { */
/*   while (! enqueue (& UART1_TXq , c)); */
/*   if (! TxPrimed ) { */
/*     TxPrimed = 1; */
/*     USART_ITConfig (USART1 , USART_IT_TXE , ENABLE ); */
/*   } */
/* } */

/////////////////////// OLD putchar ///////////////////////
/* //sends a character */
/* int putchar(int c) { */
/*   while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == (uint16_t)RESET); */
/*   USART_SendData(USART1, c); */
/*   return 0; */
/* } */


int getchar (void){
  int result = 0;
  while(!queue_empty(&rxbuf)){
    result = dequeue(&rxbuf);
  }
    return result;
}

/* int getchar (void) */
/* { */
/*   uint8_t data; */
/*   while (! dequeue (& UART1_RXq , &data)); */
/*   return data; */
/* } */

///////////////////// OLD getchar/////////////////////////
/* //gets a character */
/* int getchar(void) { */
/*   while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)RESET); */
/*   return USART_ReceiveData(USART1); */
/* } */

void USART1_IRQHandler(void) {
  int ch; 

  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
    ch = USART_ReceiveData(USART1);
    if (!enqueue(&rxbuf,ch)) {}   // overflow case -- 
                                   // throw away data and perhaps flag status
  }
  if (USART_GetFlagStatus(USART1,USART_FLAG_TXE)) {
    ch = dequeue(&txbuf);
    if (ch) {
      USART_SendData(USART1,ch);
    }
    else {
      USART_ITConfig(USART1,USART_IT_TXE,DISABLE); 
    }
  }
}

/* //sends a string */
/* void putstring(char *s) { */
/*   while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)RESET); */
/*   int i = 0; */
/*   for( i; s[i] != '\0'; i++ ) { putchar(s[i]); } */
/* } */

//flush uart
void flush_uart(void) {
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE); 
}

//sends a string
void putstring(char *s) {
  int i;
  for(i=0;i<strlen(s); i++){
    putchar(s[i]);
  }
}



/* f3d_uart.c ends here */

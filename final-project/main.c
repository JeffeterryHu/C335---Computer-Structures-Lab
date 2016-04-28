/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author:
 * Created:
 * Last-Updated: 
 *           By: 

 /* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rect.h>

#define TIMER 20000
#define AUDIOBUFSIZE 128

rect_t rectA,rectB,rectC,rectD,rectE,rectF,rectG,rectH,rectI,rectJ,rectK,rectL;
rect_t rectTree1,rectTree2,rectTree3;
rect_t rectTree4,rectTree5,rectTree6;
int disappear(rect_t *rect);
void drawI();
void drawJ();
void drawK();
void drawL();
void drawT1();
void drawT2(int i);
void drawT3(int i );
void drawScore();
void gameover();
char arr[] = {0};
int score = 0;
void drawPerson();
int randspeed();
int s;

///////////////////////////////////////////////////

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */
BYTE Buff[512];		/* File read buffer */
int ret;

struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

int playAudio(char* file_name){

  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;

  f_mount(0, &Fatfs);/* Register volume work area */

  printf("\nOpen %s\n", file_name);
  rc = f_open(&fid, file_name, FA_READ);
  if(rc){die(rc);}
  if (!rc) {
    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return -1;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    
    // now skip all non-data chunks !
    
    while(1){
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    
    // audioplayerInit(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  
  if (rc) die(rc);
}

//playAudio(files[audioIndex]);


///////////////////////////////////////////

int main(void) {
  
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  f3d_timer2_init();
  f3d_dac_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_systick_init();
  f3d_lcd_init();
  f3d_user_btn_init();

  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  f3d_lcd_fillScreen(CYAN);

  nunchuk_t nunchuk; 
  int first_time_c;

  char files[4][20];
  sprintf(files[0], "jump.wav");
  sprintf(files[1],"over.wav");
  sprintf(files[2],"johncena.wav");
  sprintf(files[3],"YOURSUPPORT.wav");

  
  //tree-1 bottom
  int g,h;
  g = 110;
  h = 155;
  initRect(&rectTree1,g,h,5,5,BLACK);

  //tree-2 mid
  int g2,h2;
  g2 = 105;
  h2 = 145;
  initRect(&rectTree2,g2,h2,15,10,GREEN);
  //tree-3 top
  int g3,h3;
  g3 = 110;
  h3 = 140;
  initRect(&rectTree3,g3,h3,5,5,GREEN);

  //cloud1-bottom
  int a,b;
  a = 64;
  b = 45;
  initRect(&rectI,a,b,20,5,WHITE);

  //cloud1-top
  int a1,b1;
  a1 = 69;
  b1 = 40;
  initRect(&rectJ,a1,b1,10,5,WHITE);

  //cloud2-bottom
  int a2,b2;
  a2 = 100;
  b2 = 25;
  initRect(&rectL,a2,b2,20,5,WHITE);

  //cloud2-top
  int a3,b3;
  a3 = 105;
  b3 = 20;
  initRect(&rectK,a3,b3,10,5,WHITE);

  //head
  int m1,n1;
  m1 = 15;
  n1 = 130;
  initRect(&rectF,m1,n1,15,15,YELLOW);

  //left eye
  int x4,y4;
  x4 = 15;
  y4 = 135;
  initRect(&rectG,x4,y4,5,5,BLACK);

  //right eye
  int x5,y5;
  x5 = 25;
  y5 = 135;
  initRect(&rectH,x5,y5,5,5,BLACK);

  //left hand
  int x2,y2;
  x2 = 10;
  y2 = 145;
  initRect(&rectD,x2,y2,5,5,YELLOW);

  //right hand
  int x3,y3;
  x3 = 30;
  y3 = 145;
  initRect(&rectE,x3,y3,5,5,YELLOW);

  //body
  int m,n;
  m = 15;
  n = 145;
  initRect(&rectB,m,n,15,10,RED);

  //left foot
  int x,y;
  x = 15;
  y = 155;
  initRect(&rectA,x,y,5,5,BLACK);

  //right foot
  int x1,y1;
  x1 = 25;
  y1 = 155;
  initRect(&rectC,x1,y1,5,5,BLACK);
  drawScore(score);
  s = 1;


  ///////////////////////////////////////////// WHILE LOOP ////////////////////////////////////
  while (1){
    ///////////////game over///////////////
    if (collision(&rectE,&rectTree2,&rectC,&rectTree3)){
      gameover();
      playAudio(files[1]);
      first_time_c = 0;
      break;	
    }
    else{
      //tree moves
      moveRect(&rectTree1,-1,0,CYAN);
      moveRect(&rectTree2,-1,0,CYAN);
      moveRect(&rectTree3,-1,0,CYAN);
      if(disappear(&rectTree1) && disappear(&rectTree2) && disappear(&rectTree3)){
	s = randspeed();
	drawT1();
	drawT2(s);
	drawT3(s);
	score++;
	drawScore(score);
      }
 

      //cloud1 moves
      moveRect(&rectI, -1.5, 0,CYAN);
      moveRect(&rectJ, -1.5, 0,CYAN);
      if(disappear(&rectJ) && disappear(&rectI)){
	drawI();
	drawJ();
      }

      //cloud2 moves
      moveRect(&rectK, -1, 0,CYAN);
      moveRect(&rectL, -1, 0,CYAN);
      if(disappear(&rectK) && disappear(&rectL)){
	drawK();
	drawL();
      }
      delay(5);

      f3d_nunchuk_read(&nunchuk);
      unsigned char c = nunchuk.c;
      // unsigned char z = nunchuk.z;

      if(user_btn_read() || c == 1){
	if(first_time_c == 1){
	  // playAudio(files[0]);
	  for(y=160;y>128;y--){
	    if (collision(&rectE,&rectTree2,&rectC,&rectTree3)){
	      gameover();
	      first_time_c = 0;
	      playAudio(files[1]);
	      break;		
	    }
	    else{
	      //guy jump
	      moveRect(&rectA,0,-2,CYAN);
	      moveRect(&rectB,0,-2,CYAN);
	      moveRect(&rectC,0,-2,CYAN);//right foot
	      moveRect(&rectD,0,-2,CYAN);
	      moveRect(&rectE,0,-2,CYAN);//right hand
	      moveRect(&rectF,0,-2,CYAN);
	      moveRect(&rectG,0,-2,CYAN);
	      moveRect(&rectH,0,-2,CYAN);
	     

	      //tree moves
	      moveRect(&rectTree1,-1,0,CYAN);
	      moveRect(&rectTree2,-1,0,CYAN);
	      moveRect(&rectTree3,-1,0,CYAN);
	      if(disappear(&rectTree1) && disappear(&rectTree2) && disappear(&rectTree3)){
		s = randspeed();		  
		drawT1();
		drawT2(s);
		drawT3(s);
		score++;
		drawScore(score);
	      }
	      
	      //cloud1 moves
	      moveRect(&rectI, -1.5, 0,CYAN);
	      moveRect(&rectJ, -1.5, 0,CYAN);
	      if(disappear(&rectJ) && disappear(&rectI)){
		drawI();
		drawJ();
	      }

	      //cloud2 moves
	      moveRect(&rectK, -1, 0,CYAN);
	      moveRect(&rectL, -1, 0,CYAN);
	      if(disappear(&rectK) && disappear(&rectL)){
		drawK();
		drawL();
	      }
	      delay(2);
	    }
	  }
	  //end of the for loop
	
	  for(y=128;y<160;y++){
	    if (collision(&rectE,&rectTree2,&rectC,&rectTree3)){
	      gameover();
	      playAudio(files[1]);
	      first_time_c = 0;
	      break;		
	    }
	    else{
	      //guy jump
	      moveRect(&rectA,0,2,CYAN);
	      moveRect(&rectB,0,2,CYAN);
	      moveRect(&rectC,0,2,CYAN);
	      moveRect(&rectD,0,2,CYAN);
	      moveRect(&rectE,0,2,CYAN);
	      moveRect(&rectF,0,2,CYAN);
	      moveRect(&rectG,0,2,CYAN);
	      moveRect(&rectH,0,2,CYAN);
	

	      //tree moves
	      moveRect(&rectTree1,-1,0,CYAN);
	      moveRect(&rectTree2,-1,0,CYAN);
	      moveRect(&rectTree3,-1,0,CYAN);
	      if(disappear(&rectTree1) && disappear(&rectTree2) && disappear(&rectTree3)){
		s = randspeed();		  
		drawT1();
		drawT2(s);
		drawT3(s);
		score++;
		drawScore(score);
	      }
	     

	      //cloud1 moves
	      moveRect(&rectI, -1.5, 0,CYAN);
	      moveRect(&rectJ, -1.5, 0,CYAN);
	      if(disappear(&rectJ) && disappear(&rectI)){
		drawI();
		drawJ();
	      }

	      //cloud2 moves
	      moveRect(&rectK, -1, 0,CYAN);
	      moveRect(&rectL, -1, 0,CYAN);
	      if(disappear(&rectK) && disappear(&rectL)){
		drawK();
		drawL();
	      }
	      delay(2);
	    }
	  }
	  // end of the for loop

	  first_time_c = 0;
	}

	else{
	  first_time_c = 1;
	}
      }
    }
  }  
}

/////////////////////////// collision //////////////////////////
int collision(rect_t *rect1, rect_t *rect2,rect_t *rect3, rect_t *rect4){
  if(rect1->pos_x+5 >= rect2->pos_x && rect3->pos_y >= rect4->pos_y){
    return 1;
  }
  else{
    return 0;
  }
}
////////////////////////// draw score //////////////////////////////
void drawScore(int score){
  sprintf(&arr[0], "%d", score);
  f3d_lcd_drawString(80,10,"Score: ", BLACK,CYAN);
  f3d_lcd_drawString(115,10,&arr[0],BLACK,CYAN);
}
////////////////////////////cloud //////////////////////////
void drawI(){
  //cloud1-bottom
  int a,b;
  a = 113;
  b = 45;
  initRect(&rectI,a,b,20,5,WHITE);
}
void drawJ(){
  //cloud1-top
  int a1,b1;
  a1 = 113;
  b1 = 40;
  initRect(&rectJ,a1,b1,10,5,WHITE);
}
void drawL(){
  //cloud2-bottom
  int a2,b2;
  a2 = 100;
  b2 = 25;
  initRect(&rectL,a2,b2,20,5,WHITE);
}

void drawK(){
  //cloud2-top
  int a3,b3;
  a3 = 105;
  b3 = 20;
  initRect(&rectK,a3,b3,10,5,WHITE);
}
/////////////////////////////Tree///////////////////////////////////
void drawT1(){  	
  int g,h;
  g = 110;
  h = 155;
  initRect(&rectTree1,g,h,5,5,BLACK);
}

void drawT2(int i){
  int g2,h2;
  if (i){
    g2 = 105;
    h2 = 145;
    initRect(&rectTree2,g2,h2,15,10,GREEN);
  }
  else{
    g2 = 105;
    h2 = 140;
    initRect(&rectTree2,g2,h2,15,15,GREEN);
  }
}

void drawT3(int i){
  int g3,h3;
  if (i){
    g3 = 110;
    h3 = 140;
    initRect(&rectTree3,g3,h3,5,5,GREEN);
  }
  else{
    g3 = 110;
    h3 = 135;
    initRect(&rectTree3,g3,h3,5,5,GREEN);
  }

}

/////////////////////////draw person////////////////////////////////////////////
void drawPerson(){
  //left foot
  int x,y;
  x = 65;
  y = 150;
  initRect(&rectA,x,y,5,5,BLACK);

  //right foot
  int x1,y1;
  x1 = 65;
  y1 = 140;
  initRect(&rectC,x1,y1,5,5,BLACK);

  //head
  int m1,n1;
  m1 = 40;
  n1 = 140;
  initRect(&rectF,m1,n1,15,15,YELLOW);

  //left eye
  int x4,y4;
  x4 = 45;
  y4 = 140;
  initRect(&rectG,x4,y4,5,5,BLACK);

  //right eye
  int x5,y5;
  x5 = 45;
  y5 = 150;
  initRect(&rectH,x5,y5,5,5,BLACK);

  //left hand
  int x2,y2;
  x2 = 55;
  y2 = 135;
  initRect(&rectD,x2,y2,5,5,YELLOW);

  //right hand
  int x3,y3;
  x3 = 55;
  y3 = 155;
  initRect(&rectE,x3,y3,5,5,YELLOW);

  //body
  int m,n;
  m = 55;
  n = 140;
  initRect(&rectB,m,n,10,15,RED);
}
////////////////////////// disappear //////////////////////////////////
int disappear(rect_t *rect){
  if(rect->pos_x == 0){
    while (rect->width > 0){
      drawRect(rect->pos_x, rect->pos_y, rect->width, rect->depth, CYAN);
      rect->width -= 5;
      drawRect(rect->pos_x, rect->pos_y, rect->width, rect->depth, rect->color);
    }
    return 1;
  }
  else {return 0;}
}
/////////////////////GAME OVER///////////////////////////////////////
void gameover(){
  f3d_lcd_fillScreen(CYAN); 
  f3d_lcd_drawString(35, 40, "GAME OVER", BLACK, CYAN);
  f3d_lcd_drawString(25, 80, "PRESS RESET TO", BLACK, CYAN);
  f3d_lcd_drawString(35, 90, "PLAY AGAIN", BLACK, CYAN);
  f3d_lcd_drawString(35, 60, "Score: ", BLACK, CYAN);
  sprintf(&arr[0], "%d", score);
  f3d_lcd_drawString(80,60,&arr[0],BLACK,CYAN);
  drawPerson();
}
////////////////////////////////////////////////////////////////////////
int randspeed(){
  int r = rand() % 2;
  return r;
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */

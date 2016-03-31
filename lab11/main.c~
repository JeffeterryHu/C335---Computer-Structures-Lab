/* main.c --- 
 * 
 * Filename: main.c
 * Description: lab 10 main.c
 * Author: Shichao Hu && Yangjun Liu
 * Created: 3/24/2016
 * Last Modified: 3/31/2016

/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>

#define rgb(r, g, b) (((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))

struct bmpfile_magic {
  unsigned char magic[2];
};

struct bmpfile_header {
  uint32_t filesz;
  uint16_t creator1;
  uint16_t creator2;
  uint32_t bmp_offset;
};

typedef struct {
  uint32_t header_sz;
  int32_t width;
  int32_t height;
  uint16_t nplanes;
  uint16_t bitspp;
  uint32_t compress_type;
  uint32_t bmp_bytesz;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors;
  uint32_t nimpcolors;
} BITMAPINFOHEADER;

struct bmppixel{ // little endian byte order
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
BYTE Buff[128];		/* File read buffer */

uint16_t convertColor(struct bmppixel pixel) {
  return rgb(pixel.r, pixel.g, pixel.b);
}

int main(void) { 
  char footer[20];
  int i;

  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  f3d_lcd_init();
  f3d_delay_init();
  f3d_rtc_init();
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);


  f_mount(0, &Fatfs); /* Register volume work area (never fails) */

  printf("\nOpen an existing file (message.txt).\n");
    rc = f_open(&Fil, "MESSAGE.TXT", FA_READ);
  if (rc) die(rc);
 
  printf("\nType the file content.\n");
  for (;;) {
    rc = f_read(&Fil, Buff, sizeof Buff, &br);	/* Read a chunk of file */
    if (rc || !br) break;			/* Error or end of file */
    for (i = 0; i < br; i++)		        /* Type the data */
      putchar(Buff[i]);
  }
  if (rc) die(rc);
  
  printf("\nClose the file.\n");
  rc = f_close(&Fil);
  if (rc) die(rc);
  
  printf("\nCreate a new file (hello.txt).\n");
  rc = f_open(&Fil, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
  if (rc) die(rc);
  
  printf("\nWrite a text data. (Hello world!)\n");
  rc = f_write(&Fil, "Hello world!\r\n", 14, &bw);
  if (rc) die(rc);
  printf("%u bytes written.\n", bw);
  
  printf("\nClose the file.\n");
  rc = f_close(&Fil);
  if (rc) die(rc);
  
  printf("\nOpen root directory.\n");
  rc = f_opendir(&dir, "");
  if (rc) die(rc);
  
  printf("\nDirectory listing...\n");
  for (;;) {
    rc = f_readdir(&dir, &fno);		/* Read a directory item */
    if (rc || !fno.fname[0]) break;	/* Error or end of dir */
    if (fno.fattrib & AM_DIR)
      printf("   <dir>  %s\n", fno.fname);
    else
      printf("%8lu  %s\n", fno.fsize, fno.fname);
  }
  if (rc) die(rc);
  
  printf("\nTest completed.\n");

  rc = disk_ioctl(0,GET_SECTOR_COUNT,&retval);
  printf("%d %d\n",rc,retval);


  /***************************************OUR CODE************************************/
  
  struct bmpfile_magic magic;
  struct bmpfile_header header;
  struct bmppixel pixelArray[128];
  uint16_t colorArray[128];
  BITMAPINFOHEADER info;

  nunchuk_t nunchuk;
  float accel[3] = { 0, 0, 0 };
  
  int f;
  int counter = 0;
  int prev_pic = -1;
  int prev_orientation = -1;
  int orientation = 0;

  while(1){
    char* img;
    int x;
    int y;
    int *position;
    int limit, x_reset, y_reset;
    int row = 0;
    int col = 128;
    // int pressed;

    f3d_accel_read(accel);
    f3d_nunchuk_read(&nunchuk);
    delay(120);


    ///////////////nunchuk button///////////////
    unsigned char jx = nunchuk.jx;
    unsigned char c = nunchuk.c;
    unsigned char z = nunchuk.z;

    if (jx >= 0 && jx < 127) {
      counter -= 1;
    }
    else if (jx > 127 && jx <= 256) {
      counter += 1;
    }

    if(z != 0){
      counter -= 1;
    }
    if(c != 0){
      counter += 1;
    }

    printf("%c %c %c\n", jx, c, z);


    ////////////////// determine the orientation according to the accelData /////////////////

    /////portrait/////
    if ((int) accel[0]<0) {
      orientation = 0;
    }
    /////upside down protrait/////
    if ((int) accel[0]>0) {
      orientation = 1;
    }
    /////right landscape/////
    if ((int) accel[1]<0) {
      orientation = 2;
    }
    /////left landscape/////
    if ((int) accel[1]>0) {
      orientation = 3;
    }

    ///////////avoid redrawing the same pic or when the orientation is unchanged//////////////
    if ((abs(counter)%3 == prev_pic) && (orientation == prev_orientation)){
      continue;
    }
    else{
      prev_orientation = orientation;
    }

    /////////////////////////////// associate image with prev_pic variable ///////////////////
    ////pic no.0////
    if (abs(counter)%3 == 0) {
      prev_pic = 0;
      img = "GT3.bmp";
    } 
    ////pic no.1////
    if (abs(counter)%3 == 1) {
      prev_pic = 1;
      img = "P45.bmp";
    } 
    ////pic no.2////
    if (abs(counter)%3 == 2) {
      prev_pic = 2;
      img = "E46.bmp";
    }

    ///////////////////////////// read the given image ///////////////////////////////
    if ((f = f_open(&Fil, img, FA_READ)) == -1) {
      return 1;
    }
    if (rc) die(rc);
    f_read(&Fil, &magic, sizeof(magic), &br);
    if (rc) die(rc);
    f_read(&Fil, &header, sizeof(header), &br);
    if (rc) die(rc);
    f_read(&Fil, &info, sizeof(info), &br);
    if (rc) die(rc);


    ///////// display image with different orientation according to the tilt angle of board ////////
    /* switch (orientation) { */
    /* /////portrait draw handling///// */
    /* case 0: */
    /*   f3d_lcd_fillScreen(BLACK); */
    /*   x = 0; */
    /*   y = 160; */
    /*   position = &x; */
    /*   limit = 128; */
    /*   x_reset = -128; */
    /*   y_reset = -1; */
    /*   break; */
    /* /////upside down protrait draw handling///// */
    /* case 1: */
    /*   f3d_lcd_fillScreen(BLACK); */
    /*   x = 128; */
    /*   y = 0; */
    /*   position = &x; */
    /*   limit = 0; */
    /*   x_reset = 128; */
    /*   y_reset = 1; */
    /*   break; */
    /* /////right landscape draw handling///// */
    /* case 2: */
    /*   f3d_lcd_fillScreen(BLACK); */
    /*   x = 128; */
    /*   y = 144; */
    /*   position = &y; */
    /*   limit = 16; */
    /*   x_reset = -1; */
    /*   y_reset = 128; */
    /*   break; */
    /* /////left landscape draw handling///// */
    /* case 3: */
    /*   f3d_lcd_fillScreen(BLACK); */
    /*   x = 0; */
    /*   y = 16; */
    /*   position = &y; */
    /*   limit = 144; */
    /*   x_reset = 1; */
    /*   y_reset = -128; */
    /*   break; */
    
    /* default: */
    /*   break; */
    /* } */
    f3d_lcd_fillScreen(BLACK);
    //////////////////////////// use drawPixel function to draw the image //////////////////////
    for (row = 0; row < 160; row++) {
      f_read(&Fil, &pixelArray, sizeof(pixelArray), &br);
      if (rc||!br) break;
      for (i = 0; i < 128; i++) {
	colorArray[i] = convertColor(pixelArray[i]);
      }

      /////portrait draw handling/////
      if (orientation == 0) {
	f3d_lcd_setAddrWindow(0, row, ST7735_width-1, col, 0b010);
      }

      /////upside down protrait draw handling/////
      else if (orientation == 1) {
	f3d_lcd_setAddrWindow(0, row, ST7735_width-1, col, 0b100);
      }

      /////right landscape draw handling/////
      else if (orientation == 2) {
	f3d_lcd_setAddrWindow(16, row, 144, col, 0b001);
      }

      /////left landscape draw handling/////
      else {
	f3d_lcd_setAddrWindow(16, row, 144, col, 0b111);
      } 
      
      f3d_lcd_pushColor(colorArray, 128);
    }


    /* //f3d_lcd_drawPixel(x,y,rgb(pix.r, pix.g, pix.b)); */
    
    /* if (orientation == 0 || orientation == 3) { */
    /*   *position += 1; */
    /*   if (*position >= limit) { */
    /* 	x += x_reset; */
    /* 	y += y_reset; */
    /*   } */
    /* } */
    /*   else { */
    /* 	*position -= 1; */
    /* 	if (*position <= limit) { */
    /* 	  x += x_reset; */
    /* 	  y += y_reset; */
    /* 	} */
    /*   } */
    
    f_close(&Fil);
  }
  
  return 0;
}
  /********************* Ends Here *************************/


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */

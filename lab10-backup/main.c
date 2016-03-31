/* main.c --- 
 * 
 * Filename: main.c
 * Description: lab 10 main.c
 * Author: Shichao Hu
 * Partner: Yangjun Liu
 * Created: 3/24/2016

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
#include <fcntl.h>

////////////////////////////////////////////////////////////////////////

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

struct bmppixel { // little endian byte order
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

///////////////////////////////////////////////////////////////////////

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
BYTE Buff[128];		/* File read buffer */

int main(void) { 
  char footer[20];
  int count=0;
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

  f3d_lcd_fillScreen(WHITE);


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


  /********************* Our Code ***************************/
  
  struct bmpfile_magic magic;
  struct bmpfile_header header;
  struct bmppixel pix;
  BITMAPINFOHEADER info;

  int f;

  nunchuk_t nunchuk_data;
  float accelData[3] = { 0, 0, 0 };

  int counter = 0;
  int old_pic = -1;
  int old_orientation = -1;
  int orientation = 0;

  while(1){
    f3d_nunchuk_read(&nunchuk_data);
    delay(100);

    if (counter > 100) counter = 0;
    if (nunchuk_data.c) {
      counter += 1;
    }

    if (nunchuk_data.z) {
      counter += 2;
    }

    printf("%d", counter);

    f3d_accel_read(accelData);

    if ((int) accelData[0]>0) orientation = 2; // upside down
    if ((int) accelData[0]<0) orientation = 0; // rightside up
    if ((int) accelData[1]>0) orientation = 3; // left
    if ((int) accelData[1]<0) orientation = 1; // right

    if ((abs(counter)%3 == old_pic) && (orientation == old_orientation)) continue;
    old_orientation = orientation;

    char* img;

    if (abs(counter)%3 == 0) {
      old_pic = 0;
      img = "GT3.bmp";
    } 
    else if (abs(counter) % 3 == 1) {
      old_pic = 1;
      img = "P45.bmp";
    } 
    else {
      old_pic = 2;
      img = "E46.bmp";
    }

    if ((f = f_open(&Fil, img, FA_READ)) == -1) {
      return 1;
    }
    if (rc) die(rc);
    f_read(&Fil, &magic, sizeof(magic), &br);
    f_read(&Fil, &header, sizeof(header), &br);
    f_read(&Fil, &info, sizeof(info), &br);
    int x = 0;
    int y = 160;
    int *check;
    int limit, x_reset, y_reset;

    switch (orientation) {
    case 0:
      x = 0;
      y = 160;
      check = &x;
      limit = 128;
      x_reset = -128;
      y_reset = -1;
      break;
    case 1:
      x = 128;
      y = 128;
      check = &y;
      limit = 128;
      x_reset = -1;
      y_reset = -128;
      break;
    case 2:
      x = 0;
      y = 0;
      check = &x;
      limit = 128;
      x_reset = -128;
      y_reset = 1;
      break;
    case 3:
      x = 0;
      y = 0;
      check = &y;
      limit = 128;
      x_reset = 1;
      y_reset = -128;
      break;
    default:
      break;
    }
    
    while (!(rc||!br)) {
      f_read(&Fil, &pix, sizeof(pix), &br);

      f3d_lcd_drawPixel(x,y,rgb(pix.r, pix.g, pix.b));

      *check = *check + 1;

      if(*check>=limit){
        x += x_reset;
        y += y_reset;
      }
    }

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

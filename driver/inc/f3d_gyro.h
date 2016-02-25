/* f3d_gyro.h */
 
/* Author: Shichao Hu
 * Partner: Broadmore Chang Tung
 * Date Created: 2/18/2016
 * Last Modified by: Shichao Hu
 * Date Last Modified: 2/25/2016
 * Part of: Lab6
 */

/* Code: */
#include <stm32f30x.h>

#define L3G_Sensitivity_250dps   (float) 114.285f
#define L3G_Sensitivity_500dps   (float) 57.1429f
#define L3G_Sensitivity_2000dps   (float) 14.285f

#define GYRO_CS_LOW() GPIO_ResetBits(GPIOE, GPIO_Pin_3)
#define GYRO_CS_HIGH() GPIO_SetBits(GPIOE, GPIO_Pin_3)

void f3d_gyro_interface_init();
void f3d_gyro_init(void);
void f3d_gyro_read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void f3d_gyro_write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
static uint8_t f3d_gyro_sendbyte(uint8_t);
void f3d_gyro_getdata(float *pfData);

/* f3d_gyro.h ends here */

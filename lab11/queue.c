/* queue.c --- 
 * 
 * Filename: queue.c
 * Description: 
 * Author: Shichao Hu && Broadmore Tung
 * Created: 3/31/2016
 * Last-Updated: 
 *           By: 
 * URL: 
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change Log:
 * 
 * 
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */

#include <f3d_led.h>
#include <f3d_user_btn.h>
#include "queue.h"



struct Queue {
  uint16_t pRD , pWR;
  uint8_t q[ QUEUE_SIZE ];
};

void init_queue(queue_t *buf) {
}

//int enqueue (queue_t *buf, int data) {
int enqueue (struct queue *q, uint8_t data) {
  if ( QueueFull (q)){
    return 0;
  }
  else {
    q->q[q->pWR] = data;
    q->pWR = ((q->pWR + 1) ==  QUEUE_SIZE ) ? 0 : q->pWR + 1;
  }
  return 1;
}

//int dequeue (queue_t *buf) {
int dequeue (struct queue *q, uint8_t *data) {
  if ( queue_empty (q)){
    return 0;
  }
  else {
    *data = q->q[q->pRD ];
    q->pRD = ((q->pRD + 1) == QUEUE_SIZE ) ? 0 : q->pRD + 1;
  }
  return 1;
}

//int queue_empty(queue_t *buf) {
int queue_empty(queue_t *buf) {  
  return (buf->pWR == buf->pRD);
}

int main(void){
  f3d_led_init();
  f3d_user_btn_init();
}



/* queue.c ends here */

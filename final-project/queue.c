/* queue.c --- 
 * 
 * Filename: queue.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Feb  7 19:49:26 2013 (-0500)
 * Version: 
 * Last-Updated: Luke Street (ldstreet), Xiaohui Wang (wang502)
 *           By: 
 *     Update #: 0
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
#include <stdio.h>
#include "queue.h"
void init_queue(queue_t *buf) {
	buf->head = -1;
	buf->tail = 0;
}

//add new item to queue
int enqueue (queue_t *buf, int data) {
	if(buf->head == -1 && buf->tail == 0){
		buf->head++;
		buf->buffer[buf->head] = data;	
		return 1;
	} else if(buf->head == buf->tail - 1){
		return 0;
	} else if (buf->head == QUEUE_SIZE - 1 && buf->tail == 0){
		return 0;
	} else {
	
		buf->head++;
		if(buf->head >= QUEUE_SIZE){
			buf->head = 0;
		}	
		buf->buffer[buf->head] = data;	
		return 1;
	}
}

//remove and return first item in queue in fifo order
int dequeue (queue_t *buf) {
	if(buf->head != -1){
		int res = buf->buffer[buf->tail];
		buf->tail++;
		if(buf->head == buf->tail - 1){
			buf->head = -1;
			buf->tail = 0;
		}
		if(buf->tail >= QUEUE_SIZE){
			buf->tail = 0;
		}
		
		return res;
	} else {
		return 0;
	}
}

//check if queue is empty
int queue_empty(queue_t *buf) {
	if (buf->head == -1){
		return 1;
	} else {
		return 0;
	}
}
/*
//debugging functions
int printQueue(queue_t *q){
  int i;
  for(i = 0; i < QUEUE_SIZE; i++){
    printf("%d ", q->buffer[i]);
  }
  printf("\nH: %d, T: %d\n", q->head, q->tail);
}

int main(void){
  queue_t q;
  init_queue(&q);
  printQueue(&q);
  enqueue(&q, 5);
  printQueue(&q);
  enqueue(&q, 4);
  printQueue(&q);
  enqueue(&q, 2);
  printQueue(&q);
  enqueue(&q, 6);
  printQueue(&q);
  enqueue(&q, 9);
  printQueue(&q);
  enqueue(&q, 8);
  printQueue(&q);
  printf("pop: %d\n", dequeue(&q));
  printQueue(&q);
  printf("pop: %d\n", dequeue(&q));
  printQueue(&q);

  return 1;
}
*/


/* queue.c ends here */

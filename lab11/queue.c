/* queue.c --- 
 * 
 * Filename: queue.c
 * Description: 
 * Author: Shichao Hu
 * Maintainer: Shichao Hu
 * Partner: Broadmore Tung
 * Created: 3/31/2016
 * Version: 
 * Last-Updated: 4/7/2016
 *           By: Shichao Hu
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

#include <queue.h>
#include <f3d_uart.h>

// this is inicialization
void init_queue(queue_t *buf) {
  // set head of queue to 0
  buf->head = 0; 
  // set tail of queue to 0
  buf->tail = 0;
  int i;
  for(i=0; i<QUEUE_SIZE; i++){
    buf->buffer[i] = 0;
  }
}

int enqueue (queue_t *buf, int data) {
  // check the buffer
  if(buf->head == buf->tail && buf->buffer[buf->head] != 0){
    return 0;
  }
  else{
    // in the else case, when the character is added to the buffer, return 1
    buf->buffer[buf->tail] = data;
    buf->tail = (buf->tail + 1) % QUEUE_SIZE;
    return 1;
  }
}

int dequeue (queue_t *buf) {
  // this is checking if the buf is empty
  if(queue_empty(buf)){
    return 0;
  }
  else{
    // if empty, return 0, else remove a character from buffer
    int outcome = buf->buffer[buf->head];
    buf->buffer[buf->head] = 0;
    buf->head = (buf->head + 1) % QUEUE_SIZE;
    return outcome;
  }
}

int queue_empty(queue_t *buf) {
  // if the buffer is empty, return 1, which means nothing between head and tail
  if(buf->buffer[buf->head] == 0 && buf->head == buf->tail){
    return 1;
  }
  else{
  return 0;
  }
}


/* queue.c ends here */

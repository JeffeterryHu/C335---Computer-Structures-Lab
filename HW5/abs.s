/* abs.s
 *	
 * Author: Shichao Hu
 * Created: 4/4/2016
 * Last-Updated: 4/4/2016
 *           By: Shichao Hu
 * Part of: HW5
 */
	
	.text
	.syntax unified
	.thumb
	.global	abs	
	.type abs, %function
	
/*
	***** My Code *****

	(notes from office hour)
	b: unconditional branch
	bne:branch if not equal (then goto ..)
	beq:branch if equal (then goto ..)
	bgt:branch if greater than (then goto ..)
	bit:branch if less than (then goto ..)
	
	determine the absolute value of a number
	1. if number is > 0, over
	2. if not, 0 minus the number to get the absolute value
	
*/

abs:
	push {r4-r7}
	cmp r0, 0         // compare the number with 0
	bgt .END          // if the number is greater than 0, r0 is the absolute value, goto .END
	movs r1, 0        // if not, let r1 = 0
	subs r2, r1, r0   // (r2 = r1 - r0) which is the absolute value
	movs r0, r2       // let r0 equals to the absolute value

	.END:
	
	pop {r4-r7}	
	bx	lr

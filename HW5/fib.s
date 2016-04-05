/* fib.s
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
	.global	fib	
	.type fib, %function

/*
	***** My Code *****

	(notes from office hour)
	b: unconditional branch
	bne:branch if not equal (then goto ..)
	beq:branch if equal (then goto ..)
	bgt:branch if greater than (then goto ..)
	bit:branch if less than (then goto ..)
	
	find the given number of fibonacci sequence
	
*/
		
fib:
	push {r4-r7}

	movs r1, 1        // let r1 = prev1 (value = 1)
	movs r2, 1	  // let r2 = prev2 (value = 1)
	movs r3, 1        // let r3 = val (value = 1)
	movs r4, 0        // let r4 = i (value = 0)
	
	cmp r0, 0         // compare the given number with 0
	beq .RETURN1   	  // if the number is 0, then goto RETURN1
	cmp r0, 1 	  // compare the given number with 1
	beq .RETURN2	  // if the number is 1, then goto RETURN2
	cmp r0, 2 	  // compare the given number with 2
	beq .RETURN2	  // if the number is 2, then goto RETURN2
//	movs r0, 2        // r0 = num - 2 (fib_c.c)
	b .TRUE           // then goto TRUE

	.TRUE:
	
//	cmp r4, 0         // compare i with 0
//	bne .FALSE        // if i not equals to 0, goto FALSE
//	cmp r4, r0        // compare i with number-2 (r0)
//	bgt .FALSE	  // if i > number-2, goto FALSE
	adds r3, r1, r2   // if i = 0, then val = prev1 + prev2
	movs r2, r1       // prev2 = prev1
	movs r1, r3       // prev1 = val
	adds r4, 1        // i++
	b .FALSE

	.FALSE:

	subs r5, r0, 2    // let r5 = num - 2
	cmp r4, r5        // compare r4 and r5
	blt .TRUE         // if i < (num - 2) is true, goto TRUE
	b .RETURN            // if flase, goto RETURN
	
	.RETURN1:         // when 0 is the given number

	movs r0, 0        // let r0 = 0
	pop {r4-r7}
	bx	lr

	.RETURN2:         // when 1 or 2 is the given number

	movs r0, 1        // let r0 = 1
	pop {r4-r7}
	bx	lr

	.RETURN:

	movs r0, r3       // if for loop ends, let r0 = r3 and return r0
	pop {r4-r7}
	bx	lr			

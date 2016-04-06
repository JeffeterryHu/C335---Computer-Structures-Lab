/* gcd.s
 *	
 * Author: Shichao Hu
 * Created: 4/4/2016
 * Last-Updated: 4/4/2016
 *           By: Shichao Hu
 * Part of: HW5
 * for this gcd.s, I collaborated with Yu Gao
 */

	.text
	.syntax unified
	.thumb
	.global	gcd	
	.type gcd, %function
	
/*
	******* My Code ********

	(notes from office hour)
	b: unconditional branch
	bne:branch if not equal (then goto ..)
	beq:branch if equal (then goto ..)
	bgt:branch if greater than (then goto ..)
	bit:branch if less than (then goto ..)
	
	find the greatest common denominator

	first, I used r3 = a, r4 = b, r5 = 1 and the same logic
	nothing shows up except "Fib tests passed"
	then, I made r3 as a variable which changes the value all time (r3 = a or r3 = b),
	then things work, this confused me
*/
	
gcd:	
	push {r4-r7}
	movs r2, 0        // let r2 = d    
	movs r4, 1        // let r4 = 1

	.Label1:           /* first while loop */

	movs r3, r0	  // let r3 = a
	ands r3, r4       // this will check (a & 1)
	bne .THEN         // if r3(a) is odd, goto label THEN

	movs r3, r1	  // let r3 = b
	ands r3, r4       // this will check (b & 1)
	bne .THEN          // is b is odd, goto THEN

	asrs r0, 1	  // let a >>= 1 (according to gcd_c.c)
	asrs r1, 1	  // let b >>= 1 (according to gcd_c.c)
	adds r2, 1	  // let d += 1 (according to gcd_c.c)
	b .Label1	  // repeat this process until the while loop condition does not match

	.THEN:
	
	.Label2:           /* second while loop */

	cmp r0, r1        // compare r0 and r1 ( a, b)
	beq .END          // if r1 equals r0 (a = b), then goto END

	movs r3, r0	  // let r3 = a
	ands r3, r4       // check (a & 1)
	bne .Label4

	asrs r0, 1	  // let a>>= 1
	b .Label2	  // goto while loop (Label2)

	.Label4: 	  /* first else if */

	movs r3, r1       // let r3 = b
	ands r3, r4       // check (b & 1)
	bne .Label5	  // if b is not odd, goto Label5	

	asrs r1, 1	  // if b is odd, then b >>= 1
	b .Label2	  // goto while loop (Label2)
	
	.Label5: 	  /* second else if */

	subs r3, r0, r1	  // if a > b, then let r3 = a - b
	ble .Label6	  // if a < b, then goto Label6

	asrs r3, 1        // then r3 >> 1
	movs r0, r3	  // let a equals to the value of r3
	b .Label2         // goto while loop (Label2)

	.Label6: 	  /* else */

	subs r3, r1, r0   // let r3 = b - a
	asrs r3, 1        // then r3 >> 1
	movs r1, r3       // let b equals to the value of b - a
	b .Label2         // goto while loop (Label2)
	
	.END:

	lsls r0, r2       // logic shift left

	pop {r4-r7}
	bx	lr

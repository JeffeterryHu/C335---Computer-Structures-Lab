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

	x = first number
	y = second number
	while(y != 0){
		t = y
		y = x % y
		x = t
	}
	then x is the gcd

*/
	
gcd:	
	push {r4-r7}
	movs r2, 0        // let r2 = 0 which is d
	movs r3, r0       // let r3 = r0 (a)
	movs r4, r1       // let r4 = r1 (b)
	movs r5, 1        // let r5 = 1

	.Label1:           /* first while loop */

	ands r3, r5       // this will check (a & 1)
	bne .END	  // if r3(a) is odd, goto label END
	ands r4, r5       // this will check (b & 1)
	bne .END          // is b is odd, goto END
	asrs r0, 1	  // let a >>= 1 (according to gcd_c.c)
	asrs r1, 1	  // let b >>= 1 (according to gcd_c.c)
	adds r2, 1	  // let d += 1 (according to gcd_c.c)
	b .Label1	  // repeat this process until the while loop condition does not match

	.Label2:           /* second while loop */

	cmp r0, r1        // compare r0 and r1 ( a, b)
	beq .Label3        // if r1 equals r0 (a = b), then goto Label3
	ands r3, r5       // check (a & 1)
	bne .Label4
	asrs r0, 1	  // let a>>= 1
	b .Label2	  // goto while loop (Label2)

	.Label3:

	lsls r0, r2       // logic shift left
	pop {r4-r7}
	bx	lr
	
	.Label4: 	  /* first else if */

	ands r4, r5       // check (b & 1)
	bne .Label5	  // if b is not odd, goto Label5	
	asrs r1, 1	  // if b is odd, then b >>= 1
	b .Label2	  // goto while loop (Label2)
	
	.Label5: 	  /* second else if */

	ands r0, r1	  // check a and b
	subs r5, r0, r1	  // if a > b, then let r5 = a - b
	ble .Label6	  // if a < b, then goto Label6
	asrs r5, 1        // then r5 >> 1
	movs r0, r5	  // let a equals to the value of r5
	b .Label2          // goto while loop (Label2)

	.Label6: 	  /* else */

	subs r5, r1, r0   // let r5 = b - a
	asrs r5, 1    // then r5 >> 1
	movs r1, r5       // let b equals to the value of r5 = b - a
	b .Label2          // goto while loop (Label2)


	.END:

/* testvector.h --- 
 * 
 * Filename: testvector.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Oct  8 09:10:58 2015
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Copyright (c) 2015 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
#include <stdint.h>

enum {ADCS, ADDS, SUBS, ANDS, BICS, LSLS, ASRS, ALL};
typedef struct testvector {
  int32_t instruction;          // Specific instruction type
  int32_t rn;                   // Register where operand 1 should be stored
  int32_t rn_value;             // Value of operand 1   
  int32_t rm;                   // Register where operand 2 should be stored
  int32_t rm_value;             // Value of operand 2  
  int32_t expected_result;      // Expected resultant value of the operation
  int32_t previous_psr;         // Value of the PSR prior to instruction to test.  
  int32_t expected_psr;         // NZCV in upper nibble 
} testvector_t;

extern testvector_t vectors[];
void test_instructions(int);

/* testvector.h ends here */

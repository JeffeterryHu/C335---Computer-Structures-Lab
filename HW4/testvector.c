/* testvector.c --- 
 * 
 * Filename: testvector.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Oct  8 12:39:28 2015
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "machine.h"
#include "testvector.h"
#include "vector.h"

int dump_state(int vector) {
  char instr_name[5];

  printf("Test Vector Number %d\n",vector);
  printf("Rn is R%d with value = 0x%08x\n",vectors[vector].rn,vectors[vector].rn_value);
  printf("Rm is R%d with value = 0x%08x\n",vectors[vector].rm,vectors[vector].rm_value);
  switch (vectors[vector].instruction) {
  case ADCS:
    strcpy(instr_name,"ADCS");
    break;
  case ADDS:
    strcpy(instr_name,"ADDS");
    break;
  case SUBS:
    strcpy(instr_name,"SUBS");
    break;
  case BICS:
    strcpy(instr_name,"BICS");
    break;
  case ANDS:
    strcpy(instr_name,"ANDS");
    break;
  case LSLS:
    strcpy(instr_name,"LSLS");
    break;
  case ASRS:
    strcpy(instr_name,"ASRS");
    break;
  default:
    break;
  }
  printf("Expected operation is %s\n",instr_name);
  printf("Expected Rn Value (result) = 0x%08x, Current Rn Value = 0x%08x\n",vectors[vector].expected_result,reg[vectors[vector].rn]);
  printf("Expected Rm Value = 0x%08x, Current Rm Value = 0x%08x\n",vectors[vector].rm_value, reg[vectors[vector].rm]);
  printf("PSR Before Instruction = 0x%08x\n",vectors[vector].previous_psr);
  printf("Expected PSR After Instruction = 0x%08x, Actual PSR After Instruction = 0x%08x\n\n",vectors[vector].expected_psr,psr);
}


void test_instructions(int instr_type) {
  int i;
  int attempt_vector = 0;
  int error_count = 0;
  for (i=0;i<(sizeof(vectors)/32);i++) {
    attempt_vector = 0;
    // Load the operand registers with data
    reg[vectors[i].rn] = vectors[i].rn_value;
    reg[vectors[i].rm] = vectors[i].rm_value;
    
    // Load the current state of the psr 
    psr = vectors[i].previous_psr; 
    switch (vectors[i].instruction) {
    case ADCS:
      if ((instr_type == ADCS) || (instr_type == ALL)) {
	adcs(vectors[i].rn, vectors[i].rm);
	attempt_vector = 1;
      }
      break;
    case ADDS:
      if ((instr_type == ADDS) || (instr_type == ALL)) {
	adds(vectors[i].rn, vectors[i].rm);
	attempt_vector = 1;
      }
      break;
    case SUBS:
      if (instr_type == SUBS) {
	//	subs(vectors[i].rn, vectors[i].rm);
	attempt_vector = 1;
      }
      break;
    case ANDS:
      if ((instr_type == ANDS) || (instr_type == ALL)) {
	ands(vectors[i].rn, vectors[i].rm);
	attempt_vector = 1;
      }
      break;
    case BICS:
      if ((instr_type == BICS) || (instr_type == ALL)) {
	bics(vectors[i].rn, vectors[i].rm);
	attempt_vector = 1;
      }
      break;
    case LSLS:
      if ((instr_type == LSLS) || (instr_type == ALL)) {
	lsls(vectors[i].rn, vectors[i].rm);
	attempt_vector = 1;
      }
      break;
    case ASRS:
      if ((instr_type == ASRS) || (instr_type == ALL)) {
	asrs(vectors[i].rn, vectors[i].rm);
	attempt_vector = 1;
      }
      break;
    default:
      break;
    }
    if (attempt_vector) {
      if (reg[vectors[i].rn] != vectors[i].expected_result) {
	error_count++;
	printf("----------------------------------------------------\n");
	printf("ERROR: Result Value Error:\n");
	dump_state(i);
      }
      if (psr != vectors[i].expected_psr) {
	error_count++;
	printf("----------------------------------------------------\n");
	printf("ERROR: Status Flag Error:\n");
	dump_state(i);
      }
      if (reg[vectors[i].rm] != vectors[i].rm_value) {
	error_count++;
	printf("----------------------------------------------------\n");
	printf("ERROR: Rm modification Error:\n");
	dump_state(i);
      }
    }
  }
  if (!error_count) {
    printf("All Tests Passed!\n");
  }
  else {
    printf("Error Count = %d\n",error_count);
  }
} 

/* testvector.c ends here */

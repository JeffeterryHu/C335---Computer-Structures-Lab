/* flags.h --- 
 * 
 * Filename: flags.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Fri Oct  9 13:40:16 2015
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

#define CLEAR_Z psr &= ~(0x40000000) 
#define SET_Z psr |= 0x40000000

#define CLEAR_N psr &= ~(0x80000000)
#define SET_N psr |= 0x80000000

#define CLEAR_C psr &= ~(0x20000000)
#define SET_C psr |= 0x20000000

#define CLEAR_V psr &= ~(0x10000000)
#define SET_V psr |= 0x10000000

/* flags.h ends here */

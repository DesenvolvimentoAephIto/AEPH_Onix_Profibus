/* 
 * File: handle_bits
 * Author: Eng. Tarc�sio Souza de Melo
 * Comments:
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef HANDLE_BITS_H
#define	HANDLE_BITS_H

/*Includes--------------------------------------------------------------------*/

/*Defines---------------------------------------------------------------------*/
/*Enable bit on y*/
#define set_bit(y, bit) (y |= (1 << bit))
/*Disable bit on y*/
#define clr_bit(y, bit) (y &= ~(1 << bit))
/*Change bit on y*/
#define cpl_bit(y, bit) (y ^= (1 << bit))
/*Return bit in y*/
#define tst_bit(y, bit) (y & (1 << bit))

#endif


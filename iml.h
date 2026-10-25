/****************************************************************************/
/*                                                                          */
/*       ESONE subroutines in 'C' for the 1330 PC-CAMAC Interface (WinNT)   */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*              Module:         IML.H                                       */
/*              Version:        1.0                                         */
/*              Author:         Francis Golding for Hytec Electronics       */
/*              Date:           23-May-1997                                 */
/*                                                                          */
/*              Copyright 1997  Hytec Electronics,                          */
/*                              5  Cradock Road, Reading,                   */
/*                              Berkshire RG2 0JT.                          */
/*                                                                          */
/*              All Rights Reserved                                         */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*  Definitions of CAMAC function codes, error codes and data structures.   */
/*                                                                          */
/****************************************************************************/
/* Modification history                                                     */
/*                                                                          */
/*   -Aug-1995 gc Initial creation                                          */
/* 23-May-1997 fg Modification for Windows NT                               */
/*                                                                          */
/****************************************************************************/

/*   Define ERROR CODES                                                */

/*   CAMAC DATAWAY OPERATIONS                                          */

#define c_success	0   // CAMAC dataway operation was successful Q + X */
#define c_notq		1   /* CAMAC dataway operation returned X and ~ Q   */
#define c_notx          2   /* CAMAC dataway operation returned Q and ~ X   */
#define c_notqx		3   /* CAMAC dataway operation returned ~X and ~Q   */

/*   CAMAC MODULE AND LAM DEFINITION                                   */

#define c_err_c        64   /* Crate number not in range 0..6          */
#define c_err_n        65   /* Station number not in range 1..24       */
#define c_err_a        66   /* Sub-address not in range 0..15          */
#define c_err_f        67   /* Function code not in range 0..41        */
#define c_err_lid      69   /* Illegal LAM identifier                  */
#define c_err_qmode    72   /* 4th element of control block illegal    */
#define c_err_nodevice 75   // Failed to open the CAMAC device 

#define c_err_fail     99   /* Controller error - no CAMAC cycle       */


/*  CAMAC register structure */
struct camac_ext
{       
	int     c;
	int     n;
	int     a;
};

/*  CAMAC lam structure */
struct camac_lam
{       
	int     c;
	int     n;
	int     a;
	int     inta[2];
};

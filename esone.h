/****************************************************************************/
/*                                                                          */
/*       ESONE subroutines in 'C' for the 1330 PC-CAMAC Interface (WinNT)   */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*              Module:         ESONE.H                                     */
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
/*  ESONE - Header file                                                     */
/*                                                                          */
/****************************************************************************/
/* Modification history                                                     */
/*                                                                          */
/*  3-Aug-1995 gc Initial creation                                          */
/* 23-May-1997 fg Modification for Windows NT                               */
/*                                                                          */
/****************************************************************************/

/*

DESCRIPTION:

	This unit provides the interface to the I/O routines for CAMAC
	when used with 	HYTEC ELECTRONICS 1330 Interface.


	SINGLE ACTION

		CSSA
		CFSA
		CDREG
		CCCZ
		CCCC 
		CCCI  
		CCCD  
		CTCI  
		CTCD  
		CTGL  

	Block Transfer

		CSUBC
        CSUBR
        CFUBC
        CFUBR

	Definitions for all the CAMAC LAM procedures are defined here

        CCLM
        CCLC
        CTLM
        CDLAM
        CCLNK
        CCULK
        CCRGL special
        CDZEL

*/

/****************************************************************************/
/* Include the function code, error code and structure definitions          */

#include "iml.h"

/* PROTOTYPES for ESONE CAMAC functions                                     */

	void __declspec(dllexport) _stdcall cccc  ( struct camac_ext* );
	void __declspec(dllexport) _stdcall cccd  ( struct camac_ext* ,int*);
	void __declspec(dllexport) _stdcall ccci  ( struct camac_ext* ,int*);
	void __declspec(dllexport) _stdcall cccz  ( struct camac_ext* );
	void __declspec(dllexport) _stdcall ccinit ( void );
	void __declspec(dllexport) _stdcall cclc  ( struct camac_lam* );
	void __declspec(dllexport) _stdcall cclm  ( struct camac_lam* ,int*);
	void __declspec(dllexport) _stdcall cclnk ( struct camac_lam* ,
	                     void (__cdecl *user_handler)());
	void __declspec(dllexport) _stdcall ccrgl ( struct camac_lam* );
	void __declspec(dllexport) _stdcall cculk ( struct camac_lam* );
	void __declspec(dllexport) _stdcall cdlam ( struct camac_lam*, int*, int*, int*, int*, int* );
	void __declspec(dllexport) _stdcall cdreg ( struct camac_ext*, int*, int*, int*, int* );
	void __declspec(dllexport) _stdcall cfmad ( int*, struct camac_ext*, long*, int*);
	void __declspec(dllexport) _stdcall cfsa  ( int*, struct camac_ext*, long*, int*);
	void __declspec(dllexport) _stdcall cfubc ( int*, struct camac_ext*, long*, int*);
	void __declspec(dllexport) _stdcall cfubr ( int*, struct camac_ext*, long*, int*);
	void __declspec(dllexport) _stdcall csmad ( int*, struct camac_ext*, short*,int*);
	void __declspec(dllexport) _stdcall cssa  ( int*, struct camac_ext*, short*, int*);
	void __declspec(dllexport) _stdcall csubc ( int*, struct camac_ext*, short*,int*);
	void __declspec(dllexport) _stdcall csubr ( int*, struct camac_ext*, short*,int*);
	void __declspec(dllexport) _stdcall ctcd  ( struct camac_ext* ,int*);
	void __declspec(dllexport) _stdcall ctci  ( struct camac_ext* ,int*);
	void __declspec(dllexport) _stdcall ctgl  ( struct camac_ext* ,int*);
	void __declspec(dllexport) _stdcall ctlm  ( struct camac_lam* ,int*);
	void __declspec(dllexport) _stdcall ctstat( int* );
	void __cdecl clcnt( long*);
	void __cdecl clwait();

	
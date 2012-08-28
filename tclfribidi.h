/*
 * fribidi.h --
 *
 *	This header file contains the function declarations needed for
 *	all of the source files in this package.
 *
 * Copyright (c) 1998-1999 Scriptics Corporation.
 * Copyright (c) 2003 ActiveState Corporation.
 * Copyright (c) 2012 IOhannes m zmölnig, forum::für::umläute
 *
 * See the file "LICENSE.txt" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#ifndef _FRIBIDI
#define _FRIBIDI

#include <tcl.h>

/*
 * Windows needs to know which symbols to export.
 */

#ifdef BUILD_fribidi
#undef TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLEXPORT
#endif /* BUILD_fribidi */

/*
 * Only the _Init function is exported.
 */

EXTERN int	Fribidi_Init(Tcl_Interp * interp);

#endif /* _FRIBIDI */

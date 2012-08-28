/*
 * tclfribidi.c --
 *
 *	This file implements a Tcl interface to libFriBidi
 *  a library for bidirectional text output
 *
 * Copyright (c) 2012 IOhannes m zmölnig
 *
 * See the file "LICENSE.txt" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

/*
 * Modified from sampleextension.c by IOhannes m zmölnig, 8/28/12
 * Modified from tclmd5.c by Dave Dykstra, dwd@bell-labs.com, 4/22/97
 */

#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tclfribidi.h"

#include <fribidi.h>

#define TCL_READ_CHUNK_SIZE 4096

/*
 * pyfribidi: 
 *       log2vis(logical, base_direction=273, encoding='utf-8', clean=False, reordernsm=True)
 *            Return string reordered visually according to base direction.
 *            Return the same type of input string, either unicode or string using
 *            encoding.
 *            
 *            Note that this function does not handle line breaking. You should
 *            call log2vis with each line.
 *            
 *            Arguments:
 *            - logical: unicode or encoded string
 *            - base_direction: optional logical base direction. Accepts one of
 *              the constants LTR, RTL or ON, defined in this module. ON calculate
 *              the base direction according to the BiDi algorithm.
 *            - encoding: optional string encoding (ignored for unicode input)
 *
 * notes:     --reordernsm Reorder NSM sequences to follow their base character
 *            --clean      Remove explicit format codes in visual string / output
 *    
 */

/*
 * tcl-fribidi: 
 *       proc fribidi::log2vis { logical {base_direction 273} {encoding 'utf-8'} }
 */
static int 
Log2vis_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) {
  Tcl_Obj *inputObj = NULL;

  

  Tcl_SetObjResult(interp, Tcl_NewStringObj("Hello, World!", -1));
  return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Fribidi_Init --
 *
 *	Initialize the new package.  The string "Fribidi" in the
 *	function name must match the PACKAGE declaration at the top of
 *	configure.in.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	The Fribidi package is created.
 *	One new command "fribidi::log2vis" is added to the Tcl interpreter.
 *
 *----------------------------------------------------------------------
 */


int
Fribidi_Init(Tcl_Interp *interp)
{
  Tcl_Namespace *nsPtr; /* pointer to hold our own new namespace */

    /*
     * This may work with 8.0, but we are using strictly stubs here,
     * which requires 8.1.
     */
    if (Tcl_InitStubs(interp, "8.1", 0) == NULL) {
	return TCL_ERROR;
    }
    if (Tcl_PkgRequire(interp, "Tcl", "8.1", 0) == NULL) {
	return TCL_ERROR;
    }
    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
	return TCL_ERROR;
    }

    /* create the namespace named 'hello' */
    nsPtr = Tcl_CreateNamespace(interp, "fribidi", NULL, NULL);
    if (nsPtr == NULL) {
      return TCL_ERROR;
    }
    Tcl_CreateObjCommand(interp, "fribidi::log2vis", (Tcl_ObjCmdProc *) Log2vis_Cmd,
	    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    return TCL_OK;
}

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

static FriBidiCharSet getEncoding(const char*s) {
  if(!strcmp("utf-8", s))
    return FRIBIDI_CHAR_SET_UTF8;


  return FRIBIDI_CHAR_SET_NOT_FOUND;
}

/*
 * tcl-fribidi: 
 *       proc fribidi::log2vis { logical {base_direction 273} {encoding 'utf-8'} }
 */
static int 
Log2vis_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) {
  char*input=NULL;
  char*output=NULL;
  int msglen;
  FriBidiCharSet enc=FRIBIDI_CHAR_SET_UTF8;


  int direction=FRIBIDI_PAR_ON;
  char*encoding="utf-8";
  switch(objc) {
  default:
  case 4:
    enc=getEncoding(Tcl_GetString(objv[3]));
    if(FRIBIDI_CHAR_SET_NOT_FOUND==enc)
      return TCL_ERROR;
  case 3:
    if (Tcl_GetIntFromObj(interp, objv[2], &direction) != TCL_OK)
      return TCL_ERROR;
    switch(direction) {
    case FRIBIDI_PAR_LTR:
    case FRIBIDI_PAR_RTL:
    case FRIBIDI_PAR_ON:
    case FRIBIDI_PAR_WLTR:
    case FRIBIDI_PAR_WRTL:
      break;
    default:
      return TCL_ERROR;
    }
  case 2:
    input=Tcl_GetString(objv[1]);
    break;
  case 1:
  case 0:
    goto wrongArgs;
  }
  if(input) {
    FriBidiParType   dir=direction;
    FriBidiChar *str_in = NULL, *str_out = NULL;
    FriBidiStrIndex ulen = 0;
    FriBidiLevel err = 0;
    int u32len=0;

    FriBidiCharSet enc=getEncoding(encoding);

    msglen=strlen(input);

    output=calloc(msglen*2, sizeof(*output));

    u32len=2*msglen*sizeof(FriBidiChar);
    
    str_in  = malloc(u32len);
    str_out = malloc(u32len);
    /* convert UTF8 to UTF32 */
    ulen = fribidi_charset_to_unicode(enc, input, msglen, str_in);
    /* reshape the UTF32 string */
    err = fribidi_log2vis(str_in, ulen, &dir, str_out, 0, 0, 0);
    if(err) {
      msglen = fribidi_unicode_to_charset(enc, str_out, ulen, output);
    }
    free(str_in);
    free(str_out);
  }


  Tcl_AppendResult (interp, output,
                    (char *) NULL);
  //Tcl_SetObjResult(interp, Tcl_NewStringObj("Hello, World!", -1));
  /*
  Tcl_AppendResult (interp, input,
                    "  ",
                    encoding,
                    (char *) NULL);
  */
  free(output);
  return TCL_OK;
  
 wrongArgs:
  Tcl_AppendResult (interp, "wrong # args: should be:\n",
                    "  ",
                    Tcl_GetString(objv[0]),
                    " string ?basedir? ?encoding?\n",
                    "    ",
                    "with <basedir> being one of\n",
                    "      fribidi::par::LTR \t(Left-To-Right)\n",
                    "      fribidi::par::LTR \t(Right-To-Left)\n",
                    "      fribidi::par::ON  \t(DirectiON-Neutral)\n",
                    "      fribidi::par::WLTR\t(Weak Left-To-Right)\n",
                    "      fribidi::par::WRTL\t(Weak Right-To-left)\n",
                    "    ",
                    "and <encoding> being one of\n",
                    "      'utf-8'\n'",
                    (char *) NULL);
  return TCL_ERROR;
}
#define TCLFRIBIDI_CONST(name)                                          \
  static int                                                            \
  name##_Const(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]) { \
    Tcl_SetIntObj(Tcl_GetObjResult(interp), FRIBIDI_PAR_##name);         \
    return TCL_OK;                                                      \
  }
TCLFRIBIDI_CONST(LTR);
TCLFRIBIDI_CONST(RTL);
TCLFRIBIDI_CONST(ON);
TCLFRIBIDI_CONST(WLTR);
TCLFRIBIDI_CONST(WRTL);

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
    nsPtr = Tcl_CreateNamespace(interp, "fribidi::par", NULL, NULL);
    if (nsPtr == NULL) {
      return TCL_ERROR;
    }

    Tcl_CreateObjCommand(interp, "fribidi::log2vis", (Tcl_ObjCmdProc *) Log2vis_Cmd,
	    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
    Tcl_CreateObjCommand(interp, "fribidi::par::LTR", (Tcl_ObjCmdProc *) LTR_Const ,(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
    Tcl_CreateObjCommand(interp, "fribidi::par::RTL", (Tcl_ObjCmdProc *) RTL_Const ,(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
    Tcl_CreateObjCommand(interp, "fribidi::par::ON",  (Tcl_ObjCmdProc *) ON_Const  ,(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
    Tcl_CreateObjCommand(interp, "fribidi::par::WLTR",(Tcl_ObjCmdProc *) WLTR_Const,(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
    Tcl_CreateObjCommand(interp, "fribidi::par::WRTL",(Tcl_ObjCmdProc *) WRTL_Const,(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
    return TCL_OK;
}

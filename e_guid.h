#pragma once


#define H_INT		0
#define H_MODULE	1	// IModuleInterface
#define H_FUNCP		2
#define H_PLUG		3	// IPluginInterface


#define GLIBID(fn_, int_)	 ( (fn_) | ((int_) << 5) )


#define G_LIB_COMMAND 	GLIBID(	1, H_INT)	// cCmd
#define G_LIB_BIND		GLIBID(	2, H_INT)	// cBind
#define G_LIB_CONSOLE	GLIBID(	3, H_INT)	// cConsole

#define G_LIB_GUIHUD	GLIBID(	4, H_INT)	// cGuiHud
#define G_LIB_GUI		GLIBID(	5, H_INT)	// cGUI
#define G_LIB_INPUT		GLIBID(	6, H_INT)	// cInput

#define G_LIB_SDK		GLIBID( 7, H_INT)	// IHagSdk

#define G_LIB_FILE		GLIBID( 8, H_INT)	// new cEngFILE


#define G_MODULE_ENGINE	GLIBID( 1, H_MODULE)
#define G_MODULE_SDK	GLIBID( 2, H_MODULE)
#define G_MODULE_GAME	GLIBID( 3, H_MODULE)


#define G_FN_EXECPTR	GLIBID( 1, H_FUNCP) // tExecPTR

#define G_FN_CHKPTR		GLIBID( 2, H_FUNCP) // t_exc_IsValidPtr
#define G_FN_MEMCPY		GLIBID( 3, H_FUNCP) // t_exc_memcpy


#define G_PLUG_GAME		GLIBID( 1, H_PLUG )

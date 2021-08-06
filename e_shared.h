#pragma once
#include "e_container.h"


class IMemoryManager;
class IWINAPI;
class ISTRF;
class IPatcher;
class IImageHelper;
class cGuiHud;
class cGUI;
class cInput;
class cCmd;
class cBind;
class cConsole;
class IHagSdk;
class IPluginInterface;




struct TSharedInterfaces
{
	XVAR< IPatcher *		>	Patcher;	// 
	XVAR< IMemoryManager *	>	Memory;		// 
	XVAR< IWINAPI *			>	Api;		// 
	XVAR< ISTRF *			>	Str;		// 
	XVAR< IImageHelper *	>	Image;		// 

	XVAR< cCmd *			>	Cmd;		// G_LIB_COMMAND
	XVAR< cBind *			>	Bind;		// G_LIB_BIND
	XVAR< cConsole *		>	Console;	// G_LIB_CONSOLE

	cGuiHud *					GuiHud;		// G_LIB_GUIHUD
	cGUI *						GUI;		// G_LIB_GUI
	cInput *					Input;		// G_LIB_INPUT

	XVAR< IHagSdk *			>	Sdk;		// G_LIB_SDK
};





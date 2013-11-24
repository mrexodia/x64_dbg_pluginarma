#ifndef _PLUGINMAIN_H
#define _PLUGINMAIN_H

#include <windows.h>
#include "pluginsdk\_plugins.h"

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif //DLL_EXPORT

//superglobal variables
extern int hPlugin;
extern PROCESS_INFORMATION* fdProcessInfo;

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct);
DLL_EXPORT bool plugstop();

#ifdef __cplusplus
}
#endif

#endif //_PLUGINMAIN_H

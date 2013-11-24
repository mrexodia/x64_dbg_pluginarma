#include "pluginmain.h"

#define plugin_name "sample plugin"
#define plugin_version 001
#define plugin_sdkversion 1

int hPlugin;

DLL_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct)
{
    initStruct->pluginVersion=plugin_version;
    initStruct->sdkVersion=plugin_sdkversion;
    strcpy(initStruct->pluginName, plugin_name);
    hPlugin=initStruct->pluginHandle;
    return true;
}

DLL_EXPORT bool plugstop()
{
    return true;
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch(fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

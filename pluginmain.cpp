#include "pluginmain.h"

#include "armasec.h"

#define plugin_name "Armadillo Plugin"
#define plugin_version 001
#define plugin_sdkversion 1

int hPlugin;
PROCESS_INFORMATION fdProcessInfo_;
PROCESS_INFORMATION* fdProcessInfo;

static void cbCreateProcess(CBTYPE type, void* callbackInfo)
{
    PLUG_CB_CREATEPROCESS* info=(PLUG_CB_CREATEPROCESS*)callbackInfo;
    fdProcessInfo=&fdProcessInfo_;
    memcpy(fdProcessInfo, info->fdProcessInfo, sizeof(PROCESS_INFORMATION));
}

static void cbExitProcess(CBTYPE type, void* callbackInfo)
{
    fdProcessInfo=0;
}

DLL_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct)
{
    initStruct->pluginVersion=plugin_version;
    initStruct->sdkVersion=plugin_sdkversion;
    strcpy_s(initStruct->pluginName, plugin_name);
    hPlugin=initStruct->pluginHandle;
    _plugin_registercallback(hPlugin, CB_CREATEPROCESS, cbCreateProcess);
    _plugin_registercallback(hPlugin, CB_EXITPROCESS, cbExitProcess);
    _plugin_registercommand(hPlugin, "armasec", cbArmaSec, true);
    _plugin_registercommand(hPlugin, "armaccess", cbArmAccess, true);
    _plugin_registercommand(hPlugin, "armaiat", cbArmaIat, true);
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

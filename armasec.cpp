#include "armasec.h"
#include "pluginsdk\TitanEngine\TitanEngine.h"
#include <stdio.h>

#ifdef _WIN64
#define UE_PARAMETER_DUINT UE_PARAMETER_QWORD
#else
#define UE_PARAMETER_DUINT UE_PARAMETER_DWORD
#endif // _WIN64

static void cbVirtualProtect()
{
    duint cip=GetContextData(UE_CIP);
    DeleteBPX(cip);
    _plugin_logputs("[ARMASEC] VirtualProtect breakpoint reached!");
    duint sec_addr=GetFunctionParameter(fdProcessInfo->hProcess, UE_FUNCTION_STDCALL, 1, UE_PARAMETER_DUINT);
    char varcmd[256]="";
#ifdef _WIN64
    sprintf(varcmd, "$result=%llX", sec_addr);
#else
    sprintf(varcmd, "$result=%X", sec_addr);
#endif // _WIN64
    DbgCmdExec(varcmd);
    GuiDisasmAt(sec_addr, cip);
    _plugin_debugpause();
}

static void cbOpenMutexA()
{
    DeleteBPX(GetContextData(UE_CIP));
    _plugin_logputs("[ARMASEC] OpenMutexA breakpoint reached!");
    duint mutex_addr=GetFunctionParameter(fdProcessInfo->hProcess, UE_FUNCTION_STDCALL, 3, UE_PARAMETER_DUINT);
    char mutex_name[30] = "";
    GetRemoteString(fdProcessInfo->hProcess, (void*)mutex_addr, &mutex_name, sizeof(mutex_name));
    if(CreateMutexA(0, false, mutex_name))
    {
        _plugin_logprintf("[ARMASEC] Mutex %s created!\n", mutex_name);
        duint vp=DbgValFromString("VirtualProtect");
        if(!vp)
        {
            _plugin_logputs("[ARMASEC] VirtualProtect could not be found!");
            _plugin_debugpause();
        }
        if(!SetBPX(vp, UE_BREAKPOINT_TYPE_UD2, (void*)cbVirtualProtect))
        {
            _plugin_logputs("[ARMASEC] Failed to set UD2 breakpoint at VirtualProtect!");
            _plugin_debugpause();
        }
        else
            _plugin_logputs("[ARMASEC] VirtualProtect breakpoint set!");
    }
    else
    {
        _plugin_logputs("[ARMASEC] Failed to create mutex!");
        _plugin_debugpause();
    }
}

bool cbArmaSec(int argc, char* argv[])
{
    duint oma=DbgValFromString("OpenMutexA");
    if(!oma)
    {
        _plugin_logputs("[ARMASEC] OpenMutexA could not be found!");
        return false;
    }
    if(!SetBPX(oma, UE_BREAKPOINT_TYPE_UD2, (void*)cbOpenMutexA))
    {
        _plugin_logputs("[ARMASEC] Failed to set UD2 breakpoint at OpenMutexA!");
        return false;
    }
    _plugin_logputs("[ARMASEC] OpenMutexA breakpoint set!");
    DbgCmdExecDirect("run");
    return true;
}

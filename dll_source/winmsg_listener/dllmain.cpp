#include <windows.h>
#include <thread>
#include <vector>
#include <mutex>
#include <set>
#include <condition_variable>
#include <sstream>
#include "qqq.h"

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;

    // Debugg attach
    // MessageBoxA(0, "", "", 0);

    InitClass* init_class = std::make_unique<InitClass>().release();

    // TODO: add delete later signal to avoid memory leaks, and emit it when dll unloaded
    // or save it to global unique pointer

    return TRUE;
}


// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void writeMemory(
    uintptr_t const address,
    const void* bytes,
    int length
) {
    size_t base = reinterpret_cast<size_t>(GetModuleHandle(0));
    DWORD old_prot;
    VirtualProtect(reinterpret_cast<void*>(base + address), length, PAGE_EXECUTE_READWRITE, &old_prot);
    memcpy(reinterpret_cast<void*>(base + address), bytes, length);
    VirtualProtect(reinterpret_cast<void*>(base + address), length, old_prot, &old_prot);
}

void enablePatches() {
    // patch
    writeMemory(0x1FD557, "\xEB\x0C", 2);
    writeMemory(0x1FD742, "\xC7\x87\xE0\x02\x00\x00\x01\x00\x00\x00\xC7\x87\xE4\x02\x00\x00\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90", 26);
    writeMemory(0x1FD756, "\x90\x90\x90\x90\x90\x90", 6);
    writeMemory(0x1FD79A, "\x90\x90\x90\x90\x90\x90", 6);
    writeMemory(0x1FD7AF, "\x90\x90\x90\x90\x90\x90", 6);
}

DWORD WINAPI MainThread(void* hModule) {
    // enable the patches
    enablePatches();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, MainThread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

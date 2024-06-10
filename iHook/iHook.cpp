#include <iostream>
#include<windows.h>
#include<TlHelp32.h>


DWORD GetProcessId(const char* processName) {
    PROCESSENTRY32 pe32 { 0 };
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    Process32First(snapShot, &pe32);

    while (Process32Next(snapShot, &pe32)) {
        if (!strcmp(processName, pe32.szExeFile)) {
            CloseHandle(snapShot);
            return pe32.th32ProcessID;
        }
    }
    CloseHandle(snapShot);
    return -1;
}

DWORD GetThreadId(DWORD processId) {
    THREADENTRY32 te32{ 0 };
    te32.dwSize = sizeof(THREADENTRY32);
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    Thread32First(snapShot, &te32);

    while (Thread32Next(snapShot, &te32)) {
        if (te32.th32OwnerProcessID == processId) {
            CloseHandle(snapShot);
            return te32.th32ThreadID;
        }
    }
    CloseHandle(snapShot);
    return -1;
}


int main(int argc, char* argv[])
{
    char* processName = argv[1];
    char* dllPath = argv[2];
    char* dllProc = argv[3];
    
        if (!processName) {
            printf("[-] Provide A Process Name.\n");
            return -1;
        }
     
        if (!dllPath) {
            printf("[-] Provide A Dll Path.\n");
            return -1;
        }
        

        if (!dllProc) {
            printf("[-] Provide A Dll Procedure Name.\n");
            return -1;
        }
        

    DWORD processId = GetProcessId(processName);
    HMODULE loadedDll = LoadLibraryA(dllPath);
    FARPROC targetProc = GetProcAddress(loadedDll, dllProc);

        if (processId == -1) {
            printf("[-] Failed To Find Process.\n");
            return -1;
        }
        
        if (!targetProc) {
            printf("[-] Failed To Find Procedure.\n");
            return -1;
        }
        

    DWORD threadId = GetThreadId(processId);

    printf("[*] Target Thread Id: %ld\n", threadId);

    if (!SetWindowsHookExA(WH_KEYBOARD, (HOOKPROC)targetProc, loadedDll, threadId)) {
        printf("[-] Failed To Set Hook Up.");
        return -1;
    }
    
    printf("[+] Set Hook Up Successfully, Beginning Message Translate / Dispatch Loop!\n");

    MSG Msg;

    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

}

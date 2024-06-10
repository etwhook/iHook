#include<windows.h>


bool isAlreadyRunning(HANDLE hSemaphore) {
    return hSemaphore != NULL && GetLastError() == ERROR_ALREADY_EXISTS;
}

void mainPayload() {
    MessageBoxA(NULL, "Injected!", "Injected!", 0);
}

extern "C" __declspec(dllexport) LRESULT testFunction(int code, WPARAM wParam, LPARAM lParam) {
    HANDLE hSemaphore = CreateSemaphoreA(NULL, 10, 10, "Injected");

    // Getting Payload To Execute Only Once With Semaphores:
    /*
    if (!isAlreadyRunning(hSemaphore)) {
        mainPayload();
    }
    */

    mainPayload();
   
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}



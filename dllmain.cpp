/*
 * dao_autosave.dll
 * ----------------
 * Sends F5 (quicksave) to Dragon Age: Origins every 120 seconds.
 *
 * Build in Visual Studio:
 *   - Platform: x86 (32-bit)
 *   - Configuration: Release
 *   - C/C++ -> Precompiled Headers -> Not Using Precompiled Headers
 *   - C/C++ -> Code Generation -> Runtime Library -> Multi-threaded (/MT)
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static const DWORD SAVE_INTERVAL_MS = 120000; // 120 seconds

static HANDLE        g_hThread = NULL;
static volatile bool g_running = false;

static HWND FindProcessWindow(const char* exeName)
{
    struct Ctx { const char* name; HWND hwnd; };
    Ctx ctx{ exeName, NULL };

    EnumWindows([](HWND hwnd, LPARAM lp) -> BOOL {
        auto* c = reinterpret_cast<Ctx*>(lp);
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
        if (h) {
            char path[MAX_PATH] = {};
            DWORD sz = MAX_PATH;
            if (QueryFullProcessImageNameA(h, 0, path, &sz)) {
                const char* slash = strrchr(path, '\\');
                const char* fname = slash ? slash + 1 : path;
                if (_stricmp(fname, c->name) == 0 && IsWindowVisible(hwnd)) {
                    c->hwnd = hwnd;
                    CloseHandle(h);
                    return FALSE;
                }
            }
            CloseHandle(h);
        }
        return TRUE;
        }, reinterpret_cast<LPARAM>(&ctx));

    return ctx.hwnd;
}

static void PressF5()
{
    HWND game = FindProcessWindow("daorigins.exe");
    if (game)
    {
        SetForegroundWindow(game);
        Sleep(100);
        keybd_event(VK_F5, 0x3F, 0, 0);
        Sleep(50);
        keybd_event(VK_F5, 0x3F, KEYEVENTF_KEYUP, 0);
    }
}

static DWORD WINAPI SaveThread(LPVOID)
{
    while (g_running)
    {
        for (DWORD slept = 0; slept < SAVE_INTERVAL_MS && g_running; slept += 500)
            Sleep(500);

        if (!g_running) break;

        PressF5();
    }
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hInst);
        g_running = true;
        g_hThread = CreateThread(NULL, 0, SaveThread, NULL, 0, NULL);
        break;

    case DLL_PROCESS_DETACH:
        g_running = false;
        if (g_hThread) {
            WaitForSingleObject(g_hThread, 2000);
            CloseHandle(g_hThread);
            g_hThread = NULL;
        }
        break;
    }
    return TRUE;
}
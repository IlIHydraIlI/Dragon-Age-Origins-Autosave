#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// ── Configuration ────────────────────────────────────────────────────────────
static const DWORD SAVE_INTERVAL_MS = 120000; // 120 seconds (2 minutes)
static HANDLE        g_hThread = NULL;
static volatile bool g_hRunning = false;

// ── System dinput8.dll Forwarding Setup ──────────────────────────────────────
typedef HRESULT(WINAPI* PFN_DirectInput8Create)(HINSTANCE, DWORD, REFIID, LPVOID*, LPVOID);
typedef HRESULT(WINAPI* PFN_DllCanUnloadNow)();
typedef HRESULT(WINAPI* PFN_DllGetClassObject)(REFCLSID, REFIID, LPVOID*);
typedef HRESULT(WINAPI* PFN_DllRegisterServer)();
typedef HRESULT(WINAPI* PFN_DllUnregisterServer)();

static PFN_DirectInput8Create g_OldDirectInput8Create = NULL;
static PFN_DllCanUnloadNow    g_OldDllCanUnloadNow = NULL;
static PFN_DllGetClassObject   g_OldDllGetClassObject = NULL;
static PFN_DllRegisterServer   g_OldDllRegisterServer = NULL;
static PFN_DllUnregisterServer g_OldDllUnregisterServer = NULL;
static HMODULE                 g_hSystemDInput = NULL;

static void LoadSystemDInput()
{
    if (!g_hSystemDInput) {
        char sysPath[MAX_PATH];
        GetSystemDirectoryA(sysPath, MAX_PATH);
        strcat(sysPath, "\\dinput8.dll");

        g_hSystemDInput = LoadLibraryA(sysPath);
        if (g_hSystemDInput) {
            g_OldDirectInput8Create = (PFN_DirectInput8Create)GetProcAddress(g_hSystemDInput, "DirectInput8Create");
            g_OldDllCanUnloadNow = (PFN_DllCanUnloadNow)GetProcAddress(g_hSystemDInput, "DllCanUnloadNow");
            g_OldDllGetClassObject = (PFN_DllGetClassObject)GetProcAddress(g_hSystemDInput, "DllGetClassObject");
            g_OldDllRegisterServer = (PFN_DllRegisterServer)GetProcAddress(g_hSystemDInput, "DllRegisterServer");
            g_OldDllUnregisterServer = (PFN_DllUnregisterServer)GetProcAddress(g_hSystemDInput, "DllUnregisterServer");
        }
    }
}

// ── Natively Exported Functions ──────────────────────────────────────────────
extern "C" __declspec(dllexport) HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPVOID punkOuter) {
    LoadSystemDInput();
    return g_OldDirectInput8Create ? g_OldDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter) : E_FAIL;
}
extern "C" __declspec(dllexport) HRESULT WINAPI DllCanUnloadNow() {
    LoadSystemDInput();
    return g_OldDllCanUnloadNow ? g_OldDllCanUnloadNow() : E_FAIL;
}
extern "C" __declspec(dllexport) HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
    LoadSystemDInput();
    return g_OldDllGetClassObject ? g_OldDllGetClassObject(rclsid, riid, ppv) : E_FAIL;
}
extern "C" __declspec(dllexport) HRESULT WINAPI DllRegisterServer() {
    LoadSystemDInput();
    return g_OldDllRegisterServer ? g_OldDllRegisterServer() : S_OK;
}
extern "C" __declspec(dllexport) HRESULT WINAPI DllUnregisterServer() {
    LoadSystemDInput();
    return g_OldDllUnregisterServer ? g_OldDllUnregisterServer() : S_OK;
}

// ── Autosave Logic ───────────────────────────────────────────────────────────
struct WindowSearchContext {
    const char* targetExe;
    HWND foundHwnd;
};

static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    WindowSearchContext* ctx = reinterpret_cast<WindowSearchContext*>(lParam);
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (hProcess) {
        char path[MAX_PATH] = { 0 };
        DWORD size = MAX_PATH;
        if (QueryFullProcessImageNameA(hProcess, 0, path, &size)) {
            const char* slash = strrchr(path, '\\');
            const char* exeName = slash ? slash + 1 : path;
            if (_stricmp(exeName, ctx->targetExe) == 0 && IsWindowVisible(hwnd)) {
                ctx->foundHwnd = hwnd;
                CloseHandle(hProcess);
                return FALSE;
            }
        }
        CloseHandle(hProcess);
    }
    return TRUE;
}

static void PressF5() {
    WindowSearchContext ctx = { "daorigins.exe", NULL };
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&ctx));
    if (ctx.foundHwnd && GetForegroundWindow() == ctx.foundHwnd) {
        keybd_event(VK_F5, 0x3F, 0, 0);
        Sleep(50);
        keybd_event(VK_F5, 0x3F, KEYEVENTF_KEYUP, 0);
    }
}

static DWORD WINAPI SaveThread(LPVOID) {
    while (g_hRunning) {
        for (DWORD slept = 0; slept < SAVE_INTERVAL_MS && g_hRunning; slept += 500) {
            Sleep(500);
        }
        if (!g_hRunning) break;
        PressF5();
    }
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hInst);
        g_hRunning = true;
        g_hThread = CreateThread(NULL, 0, SaveThread, NULL, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        g_hRunning = false;
        if (g_hThread) {
            WaitForSingleObject(g_hThread, 2000);
            CloseHandle(g_hThread);
        }
        if (g_hSystemDInput) {
            FreeLibrary(g_hSystemDInput);
        }
        break;
    }
    return TRUE;
}
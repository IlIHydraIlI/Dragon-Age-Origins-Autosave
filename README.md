# dao_autosave

A simple DLL that automatically quicksaves Dragon Age: Origins every 120 seconds by sending F5 to the game. DA:O has no reliable autosave, so this exists to save your progress without you having to think about it.

> **Notice:** This DLL simply presses F5 on a timer. It is subject to the same restrictions as a manual quicksave — it will not save during combat, conversations, cutscenes, or any other situation where the game itself would normally block a quicksave.

---

## Building

You will need **Visual Studio** (any recent version) with the Desktop C++ workload installed.

1. Create a new **Dynamic-Link Library (DLL)** project
2. Replace the contents of `dllmain.cpp` with `dao_autosave.cpp`
3. Set the following project properties:
   - **Platform:** x86 *(required — DA:O is a 32-bit game)*
   - **Configuration:** Release
   - **C/C++ → Precompiled Headers:** Not Using Precompiled Headers
   - **C/C++ → Code Generation → Runtime Library:** Multi-threaded (/MT)
4. Build with `Ctrl+Shift+B`
5. Your DLL will be at `Release\dao_autosave.dll`

---

## Injection

My preferred injection method is **[Special K](https://www.special-k.info/)**, which is an excellent tool in its own right as a framerate limiter, HDR fix, and general game enhancer. It includes an add-ons feature that can inject DLLs automatically when the game launches, so you never have to think about it again.

### Injecting with Special K

1. Set up Special K for Dragon Age: Origins as normal
2. Open the Special K control panel in-game
3. Navigate to **Plug-ins / Add-ons**
4. Add `dao_autosave.dll` to the list
5. Restart the game — the DLL will be injected automatically on every launch

### Alternative injectors

Any standard DLL injector will also work if you don't use Special K:

- **Cheat Engine** — Process list → right-click `daorigins.exe` → Load DLL
- **RemoteDLL** — free, lightweight, straightforward
- **Process Hacker** — right-click `daorigins.exe` → Properties → Modules → Load DLL

---

## Configuration

To change the save interval, edit the following line at the top of `dao_autosave.cpp` before building:

```cpp
static const DWORD SAVE_INTERVAL_MS = 120000; // 120 seconds
```

---

## How it works

The DLL spawns a background thread on injection. Every 120 seconds it finds the DA:O window, brings it to the foreground, and uses `keybd_event` to send an F5 keypress, triggering the game's built-in quicksave.

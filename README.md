# Dragon Age: Origins Autosave Proxy DLL

A lightweight **`dinput8.dll` proxy** for **Dragon Age: Origins** that automatically performs a **Quick Save (F5)** every **120 seconds**. It forwards all DirectInput calls to the original Windows DLL, allowing the game to function normally while adding automatic saves.

---

## Why?

Dragon Age: Origins has fairly infrequent autosaves, making it easy to lose progress if you forget to quick save or the game crashes. This DLL periodically triggers the game's built-in Quick Save, helping reduce lost progress without modifying the game or requiring external software.

> ⚠️ **Notice:** This DLL simply presses **F5** on a timer. It is subject to the same restrictions as a manual quick save—it **will not save** during combat, conversations, cutscenes, or any other situation where the game itself would normally block a quick save.

---

## Features

* Automatic quick save every 120 seconds
* Uses the game's native **F5 Quick Save**
* Lightweight `dinput8.dll` proxy
* No configuration or external software required

---

## Installation

Copy `dinput8.dll` into the same folder as `daorigins.exe` (typically the `bin_ship` directory). Launch the game as normal.

---

## Uninstall

Delete `dinput8.dll` from the game directory. No other files are modified.

---

## Building

### Requirements

* Visual Studio 2019 or newer
* Desktop development with C++

### Project Setup

1. Add `dllmain.cpp` and `dinput8.def` to your project.
2. Open **Project Properties** → **Linker** → **Input**.
3. Set **Module Definition File** to:

```text
dinput8.def
```

> ⚠️ **Custom Autosave Interval:** To change how often the game auto-saves, edit the `SAVE_INTERVAL_MS` constant near the top of `dllmain.cpp` before building:
>
> ```cpp
> static const DWORD SAVE_INTERVAL_MS = 120000; // 120 seconds
> ```
>
> The value is specified in **milliseconds**. For example:
>
> * `60000` = 60 seconds
> * `120000` = 2 minutes
> * `300000` = 5 minutes

### Build

Build the project as an **x86 DLL**, then copy the resulting `dinput8.dll` next to `daorigins.exe`.

---

## Compatibility

Compatible with **Dragon Age: Origins** and **Ultimate Edition**. Since this is a `dinput8.dll` proxy, it may conflict with other mods that also replace `dinput8.dll`.

---

## License

Released under the **MIT License**.

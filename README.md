test # Dragon Age: Origins Autosave Proxy DLL

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub Releases](https://img.shields.io/github/v/release/IlIHydraIlI/Dragon-Age-Origins-Autosave)](https://github.com/IlIHydraIlI/Dragon-Age-Origins-Autosave/releases)

A lightweight **`dinput8.dll` proxy** for **Dragon Age: Origins** that automatically triggers a **Quick Save (F5)** every 120 seconds. It seamlessly forwards all DirectInput calls to the original Windows DLL, allowing your game to function normally while safeguarding your progress against unexpected crashes.

---

## 💡 Why this exists?

Vanilla *Dragon Age: Origins* has infrequent autosaves, making it easy to lose hours of progress if you forget to quick save or the game crashes. This plugin periodically invokes the game's built-in quick save function automatically, requiring zero background software or heavy mod managers.

> [!IMPORTANT]  
> **Native Save Constraints:** Because this DLL programmatically presses the native **F5** key, it is subject to the exact same rules as a manual save. It **will not save** during active combat, dialogue sequences, cutscenes, or any other state where the game vanilla engine normally blocks saving.

---

## 🚀 Features

* **Set-and-Forget Automation:** Triggers an automatic quick save every 120 seconds by default.
* **Engine Native:** Uses the game's native save pipeline (no safe-file corruption risks).
* **Ultra-Lightweight:** Zero performance footprint; works as a passive `dinput8.dll` proxy.
* **Vanilla Friendly:** Requires no external configuration files or auxiliary software.

---

## 🛠️ Installation & Uninstallation

### Install
1. Download `dinput8.dll` from the [Releases](https://github.com/IlIHydraIlI/Dragon-Age-Origins-Autosave/releases) page.
2. Drop `dinput8.dll` directly into your game's executable directory (typically `...\Dragon Age Origins\bin_ship\`, right next to `daorigins.exe`).
3. Launch the game as normal.

### Uninstall
* Simply delete `dinput8.dll` from your game folder. No vanilla game files are ever touched or modified.

---

## ⚙️ Customizing the Save Interval

If you want to change how frequently the game saves, you can adjust the `SAVE_INTERVAL_MS` constant near the top of `dllmain.cpp` before compiling:

```cpp
static const DWORD SAVE_INTERVAL_MS = 120000; // 120 seconds (default)
```

The value is handled in milliseconds. For example:
* `60000` = 1 minute
* `180000` = 3 minutes
* `300000` = 5 minutes

---

## 💻 Building from Source

### Requirements
* Visual Studio 2019 or newer
* **Desktop development with C++** workload installed

### Project Configuration
1. Add `dllmain.cpp` and `dinput8.def` to your Visual Studio project.
2. Open **Project Properties** → **Linker** → **Input**.
3. Set the **Module Definition File** field to: `dinput8.def`

> [!WARNING]  
> **Compilation Target:** You **must** compile the project as an **x86 DLL (32-bit)**. Compiling as x64 will cause the game to crash on startup, as *Dragon Age: Origins* is a 32-bit application.

---

## 🧩 Compatibility

* Fully compatible with both the **Standard Edition** and **Ultimate Edition** (Steam, GOG, and EA App).
* **Mod Conflicts:** Because this uses the `dinput8.dll` slot to hook into the engine, it will conflict out-of-the-box with other mods utilizing the same proxy method. 

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

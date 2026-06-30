# Dragon Age: Origins Autosave Proxy & ASI Plugin

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub Releases](https://img.shields.io/github/v/release/IlIHydraIlI/Dragon-Age-Origins-Autosave)](https://github.com/IlIHydraIlI/Dragon-Age-Origins-Autosave/releases/latest)
[![GitHub Releases](https://img.shields.io/github/v/release/IlIHydraIlI/Dragon-Age-Origins-Autosave)](https://github.com/IlIHydraIlI/Dragon-Age-Origins-Autosave/releases/tag/v2.0-Compatability)

A lightweight automation utility for **Dragon Age: Origins** that automatically triggers a **Quick Save (F5)** every 120 seconds. Available as a standalone **`dinput8.dll` proxy** for vanilla games, or as a **`.asi` plugin** for maximum compatibility with other mod setups (like Lyall's Component Fix).

---

## 💡 Why this exists?

Vanilla *Dragon Age: Origins* has infrequent autosaves, making it easy to lose hours of progress if you forget to quick save or the game crashes. This plugin periodically invokes the game's built-in quick save function automatically, requiring zero background software or heavy mod managers.

> [!IMPORTANT]  
> **Native Save Constraints:** Because this plugin programmatically presses the native **F5** key, it is subject to the exact same rules as a manual save. It **will not save** during active combat, dialogue sequences, cutscenes, or any other state where the vanilla game engine normally blocks saving.

---

## 🚀 Features

* **Set-and-Forget Automation:** Triggers an automatic quick save every 120 seconds by default.
* **Engine Native:** Uses the game's native save pipeline (no save-file corruption risks).
* **Ultra-Lightweight:** Zero performance footprint; runs as a passive background thread.
* **Anti-Focus Guard:** Automatically detects if you Alt-Tab out of the game and suspends key emulation so it won't type "F5" into your web browser or Discord.

---

## 🛠️ Installation & Uninstallation

Choose the **one** version that matches your current game setup:

### Option 1: Vanilla Installation (No other major mods)
*Use this if your game folder doesn't already have a custom `dinput8.dll` file.*

1. Download `dinput8.dll` from the Releases page.
2. Drop `dinput8.dll` directly into your game's executable directory (typically `...\Dragon Age Origins\bin_ship\`, right next to `daorigins.exe`).
3. Launch the game as normal.

### Option 2: Compatibility Installation (Using Lyall's Fix / ASI Loaders)
*Use this if you already use Lyall's Component Fix, Ultimate ASI Loader, or any mod using its own `dinput8.dll` proxy.*

1. Download `dao_autosave.asi` from the Releases page.
2. Drop `dao_autosave.asi` directly into your game's executable directory (`...\Dragon Age Origins\bin_ship\`). 
3. *(Alternative)* If your specific loader uses a plugins folder, you can place it inside `...\bin_ship\plugins\` instead.

### Uninstall
* Simply delete `dinput8.dll` or `dao_autosave.asi` from your game folder. No vanilla game files are ever touched or modified.

---

## ⚙️ Customizing the Save Interval

If you want to change how frequently the game saves, you can adjust the `SAVE_INTERVAL_MS` constant near the top of `dllmain.cpp` before compiling:

```cpp
static const DWORD SAVE_INTERVAL_MS = 120000; // 120 seconds (default)
```

The value is handled in milliseconds. For example:
* 60000 = 1 minute
* 180000 = 3 minutes
* 300000 = 5 minutes

---

## 💻 Building from Source

### Requirements
* Visual Studio 2019 or newer
* **Desktop development with C++** workload installed

### Project Configuration

#### For the Vanilla Version (dinput8.dll)
1. Add `dllmain.cpp` and `dinput8.def` to your Visual Studio project.
2. Open **Project Properties** -> **Linker** -> **Input**.
3. Set the **Module Definition File** field to: `dinput8.def`
4. Set the Output File name to `dinput8.dll`.

#### For the Compatibility Version (dao_autosave.asi)
1. Add `dllmain.cpp` to your Visual Studio project (do not include `dinput8.def`).
2. Open **Project Properties** -> **General**.
3. Set the **Configuration Type** to **Dynamic Library (.dll)**.
4. Open **Linker** -> **General** and change the **Output File** extension name to `dao_autosave.asi`.

> [!WARNING]  
> **Compilation Target:** You **must** compile the project as an **x86 DLL (32-bit)**. Compiling as x64 will cause the game to crash on startup, as *Dragon Age: Origins* is a 32-bit application.

---

## 🧩 Compatibility

* Fully compatible with both the **Standard Edition** and **Ultimate Edition** (Steam, GOG, and EA App).
* Fully compatible with **Lyall's Dragon Age Origins Component Fix** and other ASI loaders when using the `.asi` version.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

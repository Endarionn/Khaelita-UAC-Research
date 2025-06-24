

![180b09c3-4d22-4c24-a4b0-4bc1df2b74b7](https://github.com/user-attachments/assets/b7b766bc-fc51-48be-9e39-2f8389d46b76)

---

# 🧬 Khaelita UAC Research

> Proof-of-Concept implementation of a well-known UAC bypass technique using `fodhelper.exe`, written in pure C.
> **For educational and research purposes only.**

---

## ⚙️ Overview

**Khaelita UAC Research** is a security research project demonstrating how Windows UAC (User Account Control) can be bypassed via `fodhelper.exe` and the registry-based auto-elevate mechanism.

This technique involves creating a specially crafted registry key that allows a non-elevated process to run a chosen executable with elevated privileges — without triggering a UAC prompt.

The implementation is minimal, native (written in C), and designed for learning purposes.

---

## 🛠️ Compilation

This project requires **Microsoft Visual Studio** or **MSVC (cl.exe)** to compile.

Depending on your system architecture:

| Operating System | Compiler Prompt                               |
| ---------------- | --------------------------------------------- |
| 64-bit Windows   | `x64 Native Tools Command Prompt for VS 2022` |
| 32-bit Windows   | `x86 Native Tools Command Prompt for VS 2022` |

### 🛠️ Build Command

From the appropriate Developer Command Prompt, run:

```bash
cl.exe /EHsc Bypass.c /Fe:Bypass.exe Advapi32.lib Shell32.lib
```

* `Advapi32.lib` — required for registry manipulation
* `Shell32.lib` — required for ShellExecuteA

---

## 🚀 How It Works

```text
1. The user drag-and-drops an executable (.exe) onto Bypass.exe.
2. The path is embedded into a `cmd.exe /c ""path""` payload.
3. A registry key is written to:
   HKCU\Software\Classes\ms-settings\Shell\Open\command
   with an empty "DelegateExecute" value.
4. fodhelper.exe is launched (auto-elevated system binary).
5. The payload executable is silently launched with admin rights.
6. Registry keys are deleted afterwards to clean up.
```

---

## 🔡️ How to Defend Against This

```text
- Set UAC settings to "Always notify".
- Restrict write access to registry paths like HKCU\Software\Classes.
- Block or monitor usage of fodhelper.exe in non-standard contexts.
- Use AppLocker, WDAC, or similar application whitelisting policies.
- Audit registry edits and process launches for anomalies.
- Educate users on privilege escalation techniques and risks.
```

---

## ⚠️ Legal Notice

> This software is provided for **educational, ethical research, and awareness purposes only**.
> Any unauthorized use, replication, or deployment of this tool in real-world environments may violate laws and regulations.
> The author takes **no responsibility for misuse**.

---

## 📄 License

MIT License — Use, modify, learn.
Don't break the law. Respect the knowledge.

---

## 🧠 About

Created by **Yağız Atalay** under the **Khaelita** security research label.
Made to explore, understand, and help secure real-world privilege escalation vectors.

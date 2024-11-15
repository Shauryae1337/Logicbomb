#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REG_PATH "Software\\SampleKey" // Path to the registry key
#define REG_VALUE_NAME "SampleValue"   // Name of the value

void handle_registry_logic() {
    HKEY hKey;
    DWORD dwDisposition;
    DWORD value = 0;
    DWORD valueSize = sizeof(value);

    // Try to open or create the key in HKEY_CURRENT_USER
    LONG status = RegCreateKeyExA(
        HKEY_CURRENT_USER,           // Key Root
        REG_PATH,                    // Subkey
        0,                           // Reserved
        NULL,                        // Class (optional)
        REG_OPTION_NON_VOLATILE,     // Options
        KEY_ALL_ACCESS,              // Desired Access
        NULL,                        // Security Attributes
        &hKey,                       // Handle to the key
        &dwDisposition               // Opened or created
    );

    if (status != ERROR_SUCCESS) {
        printf("Failed to open or create registry key. Error: %ld\n", status);
        return;
    }

    if (dwDisposition == REG_CREATED_NEW_KEY) {
        // Generate a random delay (10 to 99 ms)
        srand((unsigned int)time(NULL));
        int randomDelay = (rand() % 90000) + 10000; // Random number between 10 and 99

        printf("No registry key detected. Waiting %d milliseconds before creating the key...\n", randomDelay);
        Sleep(randomDelay);

        // Key was created, set initial value to 1
        value = 1;
        if (RegSetValueExA(hKey, REG_VALUE_NAME, 0, REG_DWORD, (BYTE*)&value, sizeof(value)) == ERROR_SUCCESS) {
            printf("Key created after delay, value set to 1.\n");
        } else {
            printf("Failed to set value for new key.\n");
        }
    } else if (dwDisposition == REG_OPENED_EXISTING_KEY) {
        // Key already exists, retrieve its value
        status = RegQueryValueExA(hKey, REG_VALUE_NAME, NULL, NULL, (BYTE*)&value, &valueSize);
        if (status == ERROR_SUCCESS) {
            if (value < 10) {
                value++;
                if (RegSetValueExA(hKey, REG_VALUE_NAME, 0, REG_DWORD, (BYTE*)&value, sizeof(value)) == ERROR_SUCCESS) {
                    printf("Value incremented to %lu.\n", value);
                } else {
                    printf("Failed to update value.\n");
                }
            } else if (value == 10) {
                printf("Hello, World!\n");
            }
        } else {
            printf("Failed to query existing value. Error: %ld\n", status);
        }
    }

    // Close the registry key handle
    RegCloseKey(hKey);
}

int main() {
    handle_registry_logic();
    return 0;
}

#include "memoryread.h"
#include <stdarg.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#include <cstdio> // for printf
#else
#include <cstdio> // for printf
#include <dirent.h>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <dlfcn.h> // for dlopen, dlsym
#endif

namespace MemoryRead {

    MemoryReader::MemoryReader(const std::string& gameName) : libc_handle(nullptr), kernel32_handle(nullptr) {
        int piD = getProcessIdByName(gameName);
        if(piD != -1){
            processId = piD;
        }else{
            throw std::runtime_error("Failed to find process");
        }
        #ifdef _WIN32
            kernel32_handle = LoadLibrary("kernel32.dll");
            if (!kernel32_handle) {
                throw std::runtime_error("Failed to load kernel32.dll");
            }
        #else
            libc_handle = dlopen("libc.so.6", RTLD_LAZY);
            if (!libc_handle) {
                throw std::runtime_error("Failed to load libc.so.6");
            }
        #endif
    }

    MemoryReader::~MemoryReader() {
        #ifdef _WIN32
            if (kernel32_handle) {
                FreeLibrary((HMODULE)kernel32_handle);
            }
        #else
            if (libc_handle) {
                dlclose(libc_handle);
            }
        #endif
    }

    int MemoryReader::getProcessIdByName(const std::string& processName) {
        #ifdef _WIN32
            // Windows-specific implementation
            HANDLE hProcessSnap;
            PROCESSENTRY32 pe32;
            hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hProcessSnap == INVALID_HANDLE_VALUE) {
                return -1;
            }
            pe32.dwSize = sizeof(PROCESSENTRY32);
            if (!Process32First(hProcessSnap, &pe32)) {
                CloseHandle(hProcessSnap);
                return -1;
            }
            do {
                if (processName == pe32.szExeFile) {
                    CloseHandle(hProcessSnap);
                    return pe32.th32ProcessID;
                }
            } while (Process32Next(hProcessSnap, &pe32));
            CloseHandle(hProcessSnap);
            return -1;
        #else
            // Linux-specific implementation
            DIR* dir = opendir("/proc");
            if (!dir) {
                return -1;
            }
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_DIR) {
                    pid_t pid = atoi(entry->d_name);
                    if (pid > 0) {
                        std::string cmdPath = std::string("/proc/") + entry->d_name + "/cmdline";
                        std::ifstream cmdFile(cmdPath);
                        std::string cmdLine;
                        if (cmdFile) {
                            std::getline(cmdFile, cmdLine, '\0');
                            if (!cmdLine.empty() && cmdLine.find(processName) != std::string::npos) {
                                closedir(dir);
                                return pid;
                            }
                        }
                    }
                }
            }
            closedir(dir);
            return -1;
        #endif
    }
}

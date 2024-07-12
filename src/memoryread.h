#ifndef MEMORYREAD_H
#define MEMORYREAD_H

#include <stdexcept>
#include <iostream>
#include <cstdarg>

namespace MemoryRead {

    class MemoryReader {
        public:
            MemoryReader(const std::string& gameName);
            ~MemoryReader();
            int getProcessIdByName(const std::string& processName);
            int processId;
        private:
            void* libc_handle;
            void* kernel32_handle;
    };

}

#endif // MEMORYREAD_H

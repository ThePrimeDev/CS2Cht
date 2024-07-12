#include <iostream>
#include <thread>
#include <chrono>
#include "memoryread.h"
#include "renderer/imguimain.h"

bool started = false;
std::unique_ptr<MemoryRead::MemoryReader> reader;
int main() {
    while (!started) {
        started = true;
        try {
            reader = std::make_unique<MemoryRead::MemoryReader>("cs2");
            started = true;
            std::cout << "Successfully attached to CS2. PID: " << reader->processId << std::endl;
            int ret = MainWindow::makeChtWindow();
        } catch (const std::runtime_error& e) {
            std::cout << "CS2 is not started. Retrying..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds before retrying
        }
    }
    
    int ret = MainWindow::makeChtWindow();
    return 0;
}
    
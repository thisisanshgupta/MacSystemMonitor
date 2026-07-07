//compile with: clang++ -std=c++17 main.cpp CPU.cpp RAM.cpp Process.cpp Battery.cpp -o main -framework IOKit -framework CoreFoundation
#include <iostream>
#include <string>
#include <unistd.h>

#include "CPU.hpp"
#include "RAM.hpp"
#include "Process.hpp"
#include "Battery.hpp"

int main(int argc, char* argv[]) {
    std::string filter = (argc >= 2) ? argv[1] : "";

    CPU cpu_subsystem;
    RAM ram_subsystem;
    ProcessManager proc_subsystem;
    Battery battery_subsystem;

    while (true) {
        std::cout << "\033[2J\033[1H";
        std::cout << "================== SYSTEM STATISTICS ==================\n";

        double cpu_val = cpu_subsystem.get_percentage();
        if (cpu_val >= 0.0) std::cout << "CPU Load:  " << cpu_val << "%\n";
        else std::cout << "CPU Load:  [Calculating delta...]\n";

        ram_subsystem.display();
        battery_subsystem.display();

        if (!filter.empty()) {
            std::cout << "Filter Active: \"" << filter << "\"\n";
        }
        //uncomment the below line to display currently running processes
        //proc_subsystem.display(filter);

        std::cout << std::flush;
        sleep(1);
    }
    return 0;
}

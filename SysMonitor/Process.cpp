#include "Process.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <libproc.h>

void ProcessManager::display(const std::string& filter_keyword){
    int expected_bytes = proc_listpids(PROC_ALL_PIDS, 0, nullptr, 0);
    std::vector<pid_t> pids(expected_bytes / sizeof(pid_t));
    int actual_bytes = proc_listpids(PROC_ALL_PIDS, 0, pids.data(), expected_bytes);
    int num_procs = actual_bytes / sizeof(pid_t);

    std::cout << "\nPID\t│ PROCESS NAME\n";
    std::cout << "────────┼───────────────────────────────────────────────\n";

    for(int i = 0; i < num_procs; ++i){
        if (pids[i] <= 0) continue;
        char path_buffer[PROC_PIDPATHINFO_MAXSIZE];
        if(proc_pidpath(pids[i], path_buffer, sizeof(path_buffer)) > 0){
            std::string full_path(path_buffer);
            std::string name = full_path.substr(full_path.find_last_of("/\\")+1);
            if (!filter_keyword.empty()) {
                std::string lower_name = name, lower_filter = filter_keyword;
                std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
                std::transform(lower_filter.begin(), lower_filter.end(), lower_filter.begin(), ::tolower);
                if (lower_name.find(lower_filter) == std::string::npos) continue;
                }
                std::cout << pids[i] << "\t│ " << name << "\n";
        }
    }
}

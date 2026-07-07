#include "RAM.hpp"
#include <cstdlib>
#include <iostream>
#include <mach/mach.h>
#include <mach/vm_statistics.h>
#include <mach/mach_host.h>

void RAM::display(){
    mach_port_t host_port = mach_host_self();
    vm_size_t page_size;
    host_page_size(host_port, &page_size);

    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;

    if (host_statistics64(host_port, HOST_VM_INFO64, (host_info_t)&vm_stats, &count) != KERN_SUCCESS) {
            std::cerr << "[-] Failed to fetch RAM metrics.\n";
            return;
    }

    long long free_bytes = (long long)vm_stats.free_count * page_size;
    long long active_bytes = (long long)vm_stats.active_count * page_size;
    long long inactive_bytes = (long long)vm_stats.inactive_count * page_size;
    long long wire_bytes = (long long)vm_stats.wire_count * page_size;
    long long used_bytes = active_bytes + inactive_bytes + wire_bytes;
    long long total_bytes = used_bytes + free_bytes;
    const double GB = 1024.0*1024.0*1024.0;

    std::cout << "RAM Usage: " << (used_bytes / GB) << " GB / " << (total_bytes / GB)
                  << " GB (" << ((double)used_bytes / total_bytes) * 100.0 << "%)\n";
}

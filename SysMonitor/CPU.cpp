#include "CPU.hpp"
#include <mach/mach.h>
#include <mach/processor_info.h>
#include <mach/mach_host.h>

double CPU::get_percentage(){
    host_cpu_load_info_data_t cpu_info;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    kern_return_t kr = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO,(host_info_t)&cpu_info, &count);

    if(kr != KERN_SUCCESS) return -1.0;

    unsigned long long user = cpu_info.cpu_ticks[CPU_STATE_USER];
    unsigned long long system = cpu_info.cpu_ticks[CPU_STATE_SYSTEM];
    unsigned long long idle = cpu_info.cpu_ticks[CPU_STATE_IDLE];
    unsigned long long nice = cpu_info.cpu_ticks[CPU_STATE_NICE];

    if(!has_prev_cpu){
        prev_user = user;
        prev_system = system;
        prev_idle = idle;
        prev_nice = nice;
        has_prev_cpu = true;
        return 0.0;
    }

    unsigned long long delta_user = user - prev_user;
    unsigned long long delta_system = system - prev_system;
    unsigned long long delta_idle = idle - prev_idle;
    unsigned long long delta_nice = nice - prev_nice;

    unsigned long long active_ticks = delta_user + delta_system + delta_nice;
    unsigned long long total_ticks = active_ticks + delta_idle;

    prev_user = user;
    prev_system = system;
    prev_idle = idle;
    prev_nice = nice;

    if (total_ticks == 0){
        return 0.0;
    }
    return (static_cast<double>(active_ticks)/total_ticks)*100.0;
}

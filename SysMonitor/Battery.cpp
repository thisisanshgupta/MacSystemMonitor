#include "Battery.hpp"
#include <iostream>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>
#include <CoreFoundation/CoreFoundation.h>

void Battery::display() {
    // Take a snapshot of all system hardware power sources
    CFTypeRef power_info = IOPSCopyPowerSourcesInfo();
    CFArrayRef power_sources = IOPSCopyPowerSourcesList(power_info);

    if (!power_sources || CFArrayGetCount(power_sources) == 0) {
        std::cout << "Battery: No internal battery detected (Desktop Mac?)\n";
        if (power_sources) CFRelease(power_sources);
        CFRelease(power_info);
        return;
    }

    CFDictionaryRef description = IOPSGetPowerSourceDescription(power_info, CFArrayGetValueAtIndex(power_sources, 0));
    if (description) {
        int capacity = 0;
        CFNumberRef capacity_ref = (CFNumberRef)CFDictionaryGetValue(description, CFSTR(kIOPSCurrentCapacityKey));
        if (capacity_ref) {
            CFNumberGetValue(capacity_ref, kCFNumberIntType, &capacity);
        }

        CFBooleanRef charging_ref = (CFBooleanRef)CFDictionaryGetValue(description, CFSTR(kIOPSIsChargingKey));
        bool is_charging = charging_ref ? CFBooleanGetValue(charging_ref) : false;

        std::cout << "Battery:   " << capacity << "% [" << (is_charging ? "Charging ⚡️" : "Discharging 🔋") << "]\n";
    }

    CFRelease(power_sources);
    CFRelease(power_info);
}

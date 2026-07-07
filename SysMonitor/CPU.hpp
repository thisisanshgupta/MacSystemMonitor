#ifndef CPU_HPP
#define CPU_HPP

class CPU{
    private:
        unsigned long long prev_user = 0;
        unsigned long long prev_system = 0;
        unsigned long long prev_idle = 0;
        unsigned long long prev_nice = 0;
        bool has_prev_cpu = false;
    public:
        double get_percentage();
};
#endif

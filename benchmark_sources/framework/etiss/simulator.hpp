#pragma once

#include <cstdint>
#include <cstdio>

enum : uint32_t {
    SH_SYS_EXIT = 0x20,                 // semihosting operation number
    ADP_Stopped_ApplicationExit = 0x20026 // reason code in the arg block
};

struct sh_exit_args {
    uint32_t reason;     // field 1
    uint32_t stopcode;   // field 2
};

class Simulator
{
public:
    Simulator() = default;

    inline void begin_measurement() {}
    inline void end_measurement() {}

    void terminate(int code)
    {
        sh_exit_args args = {
            ADP_Stopped_ApplicationExit,
            static_cast<uint32_t>(code)
        };
        riscv_semihosting_exit_extended(static_cast<int>(SH_SYS_EXIT), (void*)&args);
    }

    static inline long riscv_semihosting_exit_extended(int reason, void* argpack)
    {
        register long a0 asm("a0") = reason;
        register void* a1 asm("a1") = argpack;
        register long ret asm("a0");

        asm volatile(
            ".option push            \n"
            ".option norvc          \n"
            ".balign 16             \n"
            "slli x0, x0, 0x1f      \n"
            "ebreak                 \n"
            "srai x0, x0, 7         \n"
            ".option pop            \n"
            : "=r"(ret)             // output operand
            : "r"(a0), "r"(a1)      // input operands
            : "memory"
        );

        return ret; // not reached
    }

    ~Simulator() = default;
};

#pragma once
#include <cstdint>

class memory
{
public:
    template<typename T>
    static T read(uintptr_t address)
    {
        return *(T*)address;
    }

    template<typename T>
    static void write(uintptr_t address, T value)
    {
        *(T*)address = value;
    }
    static void DumpMemory(void* ptr, size_t bytes);
};


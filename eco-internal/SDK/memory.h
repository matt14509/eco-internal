#pragma once
#include "Headers.h"

class memory
{
public:
    template<typename T>
    static T read(uintptr_t address)
    {
        return *(T*)address;
    }
    template<typename T>
    static void read(uintptr_t address, T* out, size_t count)
    {
        memcpy(out, (void*)address, count * sizeof(T));
    }
    template<typename T>
    static void read(uintptr_t address, T& out)
    {
        memcpy(&out, (const void*)address, sizeof(T));
    }
    template<typename T>
    static void write(uintptr_t address, T value)
    {
        *(T*)address = value;
    }
    static void DumpMemory(void* ptr, size_t bytes);
};


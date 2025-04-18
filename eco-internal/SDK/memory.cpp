#include "memory.h"
#include <cstdint>
#include <stdio.h>

void memory::DumpMemory(void* ptr, size_t bytes)
{
    printf("Dump at 0x%p:\n", ptr);
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < bytes; i++)
    {
        if (i % 16 == 0)
            printf("\n%08llX: ", (uintptr_t)ptr + i);
        printf("%02X ", p[i]);
    }
    printf("\n");
}
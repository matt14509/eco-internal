#pragma once
#include <vector>
template<typename T>
struct UnityList
{
    void* klass;
    void* monitor;
    struct {
        T* items; 
        int size;
        int version;
    } fields;
};

std::vector<void*> GetPlayerBasePointers(void* listPtr)
{
    std::vector<void*> result;
    if (!listPtr) return result;

    // Unity List<T> â IL2CPP:
    // [0x00] void* klass
    // [0x08] void* monitor
    // [0x10] void* items 
    // [0x18] int size
    // [0x1C] int version

    void* items = *(void**)((uintptr_t)listPtr + 0x10);
    int size = *(int*)((uintptr_t)listPtr + 0x18);

    if (!items || size <= 0) return result;

    void** itemsArray = (void**)items;
    for (int i = 0; i < size; ++i)
    {
        if (itemsArray[i])
            result.push_back(itemsArray[i]);
    }
    return result;
}
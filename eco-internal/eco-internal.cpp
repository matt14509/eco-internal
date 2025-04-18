#include <windows.h>
#include "MinHook.h"
#include "Vector.h"
#include <cstdio>
#include "memory.h"




typedef void* (*tGetActivePlayers)();
tGetActivePlayers GetActivePlayers = nullptr;
typedef void(*tSetPosition)(void* transform, Vector3 pos);
typedef Vector3(*tGetPosition)(void* transform);
typedef void(*tUpdate)(void* _this);
typedef void(*tToggleFly)(void* _this);
typedef void(*tPlayer_SetPosition)(void* _this,Vector3 pos);
typedef void* (*tGetMotor)(void* playerBase);

tPlayer_SetPosition Player_SetPosition = nullptr;
tToggleFly ToggleFly = nullptr;
tUpdate oUpdate = nullptr;
tSetPosition SetPosition = nullptr;
tGetPosition GetPosition = nullptr;
tGetMotor GetMotor = nullptr;


void hkUpdate(void* _this)
{

    if (GetAsyncKeyState('F') & 0x8000)
    {
        Sleep(50);
        ToggleFly(_this);

    }
    if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
    {
        Sleep(30);
        Player_SetPosition(_this,Vector3{ 763,1000,543 });
    }
    if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
    {
      
        void* motor = GetMotor(_this);
        printf("Motor ptr = 0x%p\n", motor);
        //memory::DumpMemory(motor, 1000);
        //hl 1 bhop
        memory::write<float>((uintptr_t)motor + 0x78, 15.0f);    // moveSpeed
        memory::write<float>((uintptr_t)motor + 0x7C, 200.0f);   // maxSpeed
        memory::write<float>((uintptr_t)motor + 0x84, 1000.0f);  // groundAccel
        memory::write<float>((uintptr_t)motor + 0x88, 1000.0f);  // airAccel
        memory::write<float>((uintptr_t)motor + 0xF8, 20.0f);    // jumpForce
        memory::write<float>((uintptr_t)motor + 0x154, 15.0f);    // swim up speed
    }
    if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
    {
        Sleep(30);
        void* listPtr = GetActivePlayers();
        printf("listPtr = 0x%p\n", listPtr);

    }
    //void* transform = *(void**)((uintptr_t)_this + TransformOffset);
    //if (transform)
    //{
    //    Vector3 curPos = GetPosition(transform);
    //    Vector3 pos = curPos + Vector3{ 0.0f, 1.0f, 0.0f };
    //    SetPosition(transform, pos);
    //}
    oUpdate(_this); 
}

DWORD WINAPI MainThread(LPVOID param)
{

    HMODULE hModule = (HMODULE)param;


    AllocConsole(); 


    FILE* fp_out;
    FILE* fp_err;
    FILE* fp_in;

    freopen_s(&fp_out, "CONOUT$", "w", stdout);
    freopen_s(&fp_err, "CONOUT$", "w", stderr);
    freopen_s(&fp_in, "CONIN$", "r", stdin);



    uintptr_t base = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
    //uintptr_t setPositionAddr = base + 0x4B1B690;
    //uintptr_t getPositionAddr = base + 0x4B1A930;
    uintptr_t updateAddr = base + 0xB64340;

    uintptr_t toggleFlyAddr = base + 0xB64150;
    uintptr_t Player_SetPositionAddr = base + 0xB63B10;
    //SetPosition = (tSetPosition)setPositionAddr;
    //GetPosition = (tGetPosition)getPositionAddr;
    uintptr_t getActivePlayersAddr = base + 0xB55640;
    uintptr_t getMotorAddr = base + 0xB65B80;

    GetMotor = (tGetMotor)getMotorAddr;
    GetActivePlayers = (tGetActivePlayers)getActivePlayersAddr;
    ToggleFly = (tToggleFly)toggleFlyAddr;
    Player_SetPosition = (tPlayer_SetPosition)Player_SetPositionAddr;

    MH_Initialize();
    MH_CreateHook((LPVOID)updateAddr, &hkUpdate, reinterpret_cast<LPVOID*>(&oUpdate));
    MH_EnableHook((LPVOID)updateAddr);
    MessageBoxA(0, "Hooked! END to unhook", "Info", MB_OK);


    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 0x8000)
            break;
        Sleep(100);
    }

    MH_DisableHook((LPVOID)updateAddr);

    MH_RemoveHook((LPVOID)updateAddr);

    MH_Uninitialize();

    MessageBoxA(0, "Unhooked!", "Info", MB_OK);

    if (fp_out) fclose(fp_out);
    if (fp_err) fclose(fp_err);
    if (fp_in)  fclose(fp_in);

    FreeConsole();

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, MainThread, hModule, 0, 0);
    }
    return TRUE;
}

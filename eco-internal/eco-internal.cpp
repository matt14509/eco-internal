//eco-internal.cpp
#include <windows.h>
#include "MinHook.h"
#include "Vector.h"
#include <cstdio>

#include "memory.h"
#include"GUI_main_menu.h"
#include "offsets.h"
#include <ConfigInstance.h>

typedef void* (*tGetActivePlayers)();
tGetActivePlayers GetActivePlayers = nullptr;
typedef void(*tSetPosition)(void* transform, Vector3 pos);
typedef Vector3(*tGetPosition)(void* transform);
typedef void(*tUpdate)(void* _this);
typedef void(*tToggleFly)(void* _this);
typedef void(*tPlayer_SetPosition)(void* _this, Vector3 pos);
typedef void* (*tGetMotor)(void* playerBase);

tPlayer_SetPosition Player_SetPosition = nullptr;
tToggleFly ToggleFly = nullptr;
tUpdate oUpdate = nullptr;
tSetPosition SetPosition = nullptr;
tGetPosition GetPosition = nullptr;
tGetMotor GetMotor = nullptr;


void* g_debug_motor_ptr = nullptr;


void hkUpdate(void* _this)
{
    if (ConfigInstance.Movement.needToggleFly)
    {
        ToggleFly(_this);
        ConfigInstance.Movement.needToggleFly = false; 
    }

        memory::read<Vector3>((uintptr_t)_this + 0xE0, ConfigInstance.Movement.saved_pos);

    if (ConfigInstance.Movement.needTeleport)
    {
        Player_SetPosition(_this, ConfigInstance.Movement.teleportCoords);
        ConfigInstance.Movement.needTeleport = false; 
    }

    void* motor = GetMotor(_this);
    g_debug_motor_ptr = motor;
 
        memory::write<float>((uintptr_t)motor + 0x78, ConfigInstance.Movement.moveSpeed);    // moveSpeed
        memory::write<float>((uintptr_t)motor + 0x7C, ConfigInstance.Movement.maxSpeed);   // maxSpeed
        memory::write<float>((uintptr_t)motor + 0x84, ConfigInstance.Movement.groundAccel);  // groundAccel
        memory::write<float>((uintptr_t)motor + 0x88, ConfigInstance.Movement.airAccel);  // airAccel
        memory::write<float>((uintptr_t)motor + 0xF8, ConfigInstance.Movement.jumpForce);    // jumpForce
        memory::write<float>((uintptr_t)motor + 0x154, ConfigInstance.Movement.swimUpSpeed);   // swim up speed
    
    if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
    {
        Sleep(30);
        void* listPtr = GetActivePlayers();
        printf("listPtr = 0x%p\n", listPtr);
    }
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
    uintptr_t updateAddr = base + offsets::Player::update_RVA;
    uintptr_t toggleFlyAddr = base + offsets::Player::toggle_fly_RVA;
    uintptr_t Player_SetPositionAddr = base + offsets::Player::set_position_RVA;
    uintptr_t getActivePlayersAddr = base + 0xB55640;
    uintptr_t getMotorAddr = base + offsets::Player::get_motor_RVA;

    GetMotor = (tGetMotor)getMotorAddr;
    GetActivePlayers = (tGetActivePlayers)getActivePlayersAddr;
    ToggleFly = (tToggleFly)toggleFlyAddr;
    Player_SetPosition = (tPlayer_SetPosition)Player_SetPositionAddr;

    MH_Initialize();
    MH_CreateHook((LPVOID)updateAddr, &hkUpdate, reinterpret_cast<LPVOID*>(&oUpdate));
    MH_EnableHook((LPVOID)updateAddr);


    void* pPresent = window::GetPresentAddress();
    printf("Present address: %p\n", pPresent);
    if (pPresent)
    {
        MH_CreateHook(pPresent, &window::hkPresent, reinterpret_cast<void**>(&window::oPresent));
        if (MH_EnableHook(pPresent) != MH_OK) printf("MH_EnableHook(pPresent) failed!\n"); 
        else
        {
            printf("MH_EnableHook(pPresent) successful!\n");
        }
        
    }

    MessageBoxA(0, "Hooked! END to unhook", "Info", MB_OK);

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 0x8000)
            break;
        Sleep(100);
    }

    MH_DisableHook((LPVOID)updateAddr);
    MH_RemoveHook((LPVOID)updateAddr);

    if (pPresent)
    {
        MH_DisableHook(pPresent);
        MH_RemoveHook(pPresent);
    }
    if (window::oWndProc && window::g_hWnd)
    {
        SetWindowLongPtr(window::g_hWnd, GWLP_WNDPROC, (LONG_PTR)window::oWndProc);
        window::oWndProc = nullptr;
    }

    if (window::g_ImGuiInitialized)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

    }

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
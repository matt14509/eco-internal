//GUI_main_menu.cpp
#include "Headers.h"


#include "GUI_main_menu.h"
#include <ConfigInstance.h>
#include <memory.h>


#define DEBUG true

Present_t window::oPresent = nullptr;
bool window::g_ImGuiInitialized = false;
HWND window::g_hWnd = nullptr;
WNDPROC  window::oWndProc = nullptr;
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


void* window::GetPresentAddress()
{

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("DummyWindow"), NULL };
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindow(wc.lpszClassName, _T("Dummy Window"), WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, wc.hInstance, NULL);


    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.Width = 100;
    sd.BufferDesc.Height = 100;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ID3D11Device* pDevice = NULL;
    ID3D11DeviceContext* pContext = NULL;
    IDXGISwapChain* pSwapChain = NULL;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevelArray, 1,
        D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featureLevel, &pContext);

    if (FAILED(hr))
        return nullptr;


    void** pVTable = *(void***)pSwapChain;
    void* pPresent = pVTable[8]; 


    pSwapChain->Release();
    pDevice->Release();
    pContext->Release();
    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return pPresent;
}



LRESULT CALLBACK hkWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    return CallWindowProc(window::oWndProc, hWnd, msg, wParam, lParam);
}

// ImGui hook stuff
typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain*, UINT, UINT);

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
bool g_ShowMenu = true;


HRESULT __stdcall window::hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    static bool firstCall = true;
    if (firstCall)
    {
        printf("Init ImGui...\n");
        firstCall = false;
    }

    if (!g_ImGuiInitialized)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice)))
        {
            g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);

            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            g_hWnd = sd.OutputWindow;

            ImGui::CreateContext();
            ImGui_ImplWin32_Init(g_hWnd);
            ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

            if (!oWndProc)
                oWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);
            g_ImGuiInitialized = true;
        }
    }


    ID3D11RenderTargetView* pRTV = nullptr;
    ID3D11Texture2D* pBackBuffer = nullptr;
    if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
    {
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRTV);
        g_pd3dDeviceContext->OMSetRenderTargets(1, &pRTV, NULL);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (GetAsyncKeyState(VK_INSERT) & 1)
        g_ShowMenu = !g_ShowMenu;

    if (g_ShowMenu)
    {
        ImGui::Begin("Menu", nullptr);

        if (ImGui::BeginTabBar("MainTabs"))
        {
            if (ImGui::BeginTabItem("ESP"))
            {
                if (ImGui::BeginTabBar("EspTabs"))
                {
                    if (ImGui::BeginTabItem("Player ESP"))
                    {
                        
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Entity ESP"))
                    {
                        
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Aim"))
            {
                
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Misc"))
            {
                if (ImGui::BeginTabBar("misc_tabs"))
                {
                    if (ImGui::BeginTabItem("Movement"))
                    {
                        if (ImGui::Button("Set Standard All"))
                        {
                            ConfigInstance.Movement.moveSpeed = 3.30f;
                            ConfigInstance.Movement.maxSpeed = 6.00f;
                            ConfigInstance.Movement.groundAccel = 30.00f;
                            ConfigInstance.Movement.airAccel = 5.00f;
                            ConfigInstance.Movement.jumpForce = 11.00f;
                            ConfigInstance.Movement.swimUpSpeed = 1.80f;
                        }
                        ImGui::Separator();
                        ImGui::PushID("moveSpeed");
                        ImGui::SliderFloat("moveSpeed", &ConfigInstance.Movement.moveSpeed, 0.0f, 10000.0f, "%.2f");
                        ImGui::SameLine();
                        if (ImGui::Button("Set Standard")) ConfigInstance.Movement.moveSpeed = 3.30f;
                        ImGui::PopID();

                        ImGui::PushID("maxSpeed");
                        ImGui::SliderFloat("maxSpeed", &ConfigInstance.Movement.maxSpeed, 0.0f, 10000.0f, "%.2f");
                        ImGui::SameLine();
                        if (ImGui::Button("Set Standard")) ConfigInstance.Movement.maxSpeed = 6.00f;
                        ImGui::PopID();

                        ImGui::PushID("groundAccel");
                        ImGui::SliderFloat("groundAccel", &ConfigInstance.Movement.groundAccel, 0.0f, 10000.0f, "%.2f");
                        ImGui::SameLine();
                        if (ImGui::Button("Set Standard")) ConfigInstance.Movement.groundAccel = 30.00f;
                        ImGui::PopID();

                        ImGui::PushID("airAccel");
                        ImGui::SliderFloat("airAccel", &ConfigInstance.Movement.airAccel, 0.0f, 10000.0f, "%.2f");
                        ImGui::SameLine();
                        if (ImGui::Button("Set Standard")) ConfigInstance.Movement.airAccel = 5.00f;
                        ImGui::PopID();

                        ImGui::PushID("jumpForce");
                        ImGui::SliderFloat("jumpForce", &ConfigInstance.Movement.jumpForce, 0.0f, 10000.0f, "%.2f");
                        ImGui::SameLine();
                        if (ImGui::Button("Set Standard")) ConfigInstance.Movement.jumpForce = 11.00f;
                        ImGui::PopID();

                        ImGui::PushID("swimUpSpeed");
                        ImGui::SliderFloat("swimUpSpeed", &ConfigInstance.Movement.swimUpSpeed, 0.0f, 10000.0f, "%.2f");
                        ImGui::SameLine();
                        if (ImGui::Button("Set Standard")) ConfigInstance.Movement.swimUpSpeed = 1.80f;
                        ImGui::PopID();


                        ImGui::Separator();
                        if (ImGui::Button("Toggle Fly"))
                        {
                            ConfigInstance.Movement.needToggleFly = true;
                        }
                        if (ImGui::Button("Set current corsd"))
                        {
                            ConfigInstance.Movement.teleportCoords = ConfigInstance.Movement.saved_pos;
                        }
                        float tempCoords[3] = { ConfigInstance.Movement.teleportCoords.x,
                        ConfigInstance.Movement.teleportCoords.y,
                        ConfigInstance.Movement.teleportCoords.z };

                        if (ImGui::InputFloat3("Teleport coords", tempCoords, "%.2f"))
                        {
                            ConfigInstance.Movement.teleportCoords = Vector3(tempCoords[0], tempCoords[1], tempCoords[2]);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Teleport"))
                        {
                            ConfigInstance.Movement.needTeleport = true;
                        }
                        if (ImGui::Button("HL1 BHOP")) 
                        {
                            ConfigInstance.Movement.moveSpeed = 15.0f;
                            ConfigInstance.Movement.maxSpeed = 1000.0f;
                            ConfigInstance.Movement.groundAccel = 400.0f;
                            ConfigInstance.Movement.airAccel = 400.0f;
                            ConfigInstance.Movement.jumpForce = 20.0f;
                            ConfigInstance.Movement.swimUpSpeed = 15.0f;
                        }
                        ImGui::EndTabItem();
                    }

                   
                    ImGui::EndTabBar();
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("config"))
            {
                static char save_configNameBuffer[60] = "config";
                ImGui::InputTextWithHint("Config Name for save", "Enter config name", save_configNameBuffer, IM_ARRAYSIZE(save_configNameBuffer), ImGuiInputTextFlags_CharsNoBlank);
                if (ImGui::Button("Save config")) {
                    ConfigInstance.SaveToFile(save_configNameBuffer);
                }
                static char load_configNameBuffer[60] = "config";
                ImGui::InputTextWithHint("Config Name for load", "Enter config name", load_configNameBuffer, IM_ARRAYSIZE(load_configNameBuffer));
                if (ImGui::Button("Load config")) {
                    ConfigInstance.LoadFromFile(load_configNameBuffer);
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("debug") && DEBUG)
            {
                extern void* g_debug_motor_ptr;
                uintptr_t m = (uintptr_t)g_debug_motor_ptr;
                ImGui::Text("moveSpeed:    %.2f", memory::read<float>(m + 0x78));
                ImGui::Text("maxSpeed:     %.2f", memory::read<float>(m + 0x7C));
                ImGui::Text("groundAccel:  %.2f", memory::read<float>(m + 0x84));
                ImGui::Text("airAccel:     %.2f", memory::read<float>(m + 0x88));
                ImGui::Text("jumpForce:    %.2f", memory::read<float>(m + 0xF8));
                ImGui::Text("swimUpSpeed:  %.2f", memory::read<float>(m + 0x154));
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();

        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        //draw_list->AddRect(ImVec2(100, 100), ImVec2(200, 200), IM_COL32(255, 0, 0, 255), 2.0f, ImDrawFlags_None);
    }



    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


    if (pRTV) pRTV->Release();
    if (pBackBuffer) pBackBuffer->Release();

    return oPresent(pSwapChain, SyncInterval, Flags);
}
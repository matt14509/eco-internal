
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
// ImGui
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain*, UINT, UINT);
class window
{
public:
	static HRESULT __stdcall  hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	static void* GetPresentAddress();
	static Present_t oPresent;
	static HWND g_hWnd;
	static bool g_ImGuiInitialized;
	static WNDPROC oWndProc;
private:

};


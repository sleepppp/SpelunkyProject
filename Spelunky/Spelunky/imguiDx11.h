// ImGui Win32 + DirectX11 binding

// Implemented features:
//  [X] User texture binding. Use 'ID3D11ShaderResourceView*' as ImTextureID. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace ImGui
{
	IMGUI_API bool        Create(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
	IMGUI_API void        Delete();
	IMGUI_API void        Update();
	IMGUI_API void        Render();
	IMGUI_API void        UpdateMouseCursor();
	IMGUI_API void		  CreateFontsTexture();
	IMGUI_API bool		  WndProc(void* hwnd, unsigned int msg, unsigned int wParam, long lParam);

	// Use if you want to reset your rendering device without losing ImGui state.
	IMGUI_API void        Invalidate();
	IMGUI_API bool        Validate();
}

// Handler for Win32 messages, update mouse/keyboard data.
// You may or not need this for your implementation, but it can serve as reference for handling inputs.
// Commented out to avoid dragging dependencies on <windows.h> types. You can copy the extern declaration in your code.
/*
IMGUI_API LRESULT   WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
*/

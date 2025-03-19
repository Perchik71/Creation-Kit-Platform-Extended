// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/RegistratorWindow.h"
#include "RenderWindow.h"
#include "MainWindow.h"
#include "Editor API/SSE/BGSRenderWindow.h"
#include "Editor API/SSE/BSGraphicsTypes.h"
#include "NiAPI/NiSourceTexture.h"
#include "Patches/D3D11Patch.h"

#include <comdef.h>
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace Microsoft::WRL;
		using namespace NiAPI::SkyrimSpectialEdition;
		using namespace EditorAPI::SkyrimSpectialEdition;

		extern ID3D11Device* pointer_d3d11DeviceIntf;
		extern ID3D11DeviceContext* pointer_d3d11DeviceContext;
		extern std::vector<ID3D11RenderTargetView*> pointer_mainRenderTargetView;
		extern ImFont* imguiFonts[3];
		extern uintptr_t gGlobAddrDeviceContext;

		extern ImVec4 gImGuiGreenColor;
		extern ImVec4 gImGuiOrangeColor;
		extern ImVec4 gImGuiRedColor;
		extern ImVec4 gImGuiGreyColor;
		extern bool gImGuiShowDrawInfo;

		namespace SkyrimSpectialEdition
		{
			RenderWindow* GlobalRenderWindowPtr = nullptr;

			bool RenderWindow::HasOption() const
			{
				return false;
			}

			bool RenderWindow::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* RenderWindow::GetOptionName() const
			{
				return nullptr;
			}

			const char* RenderWindow::GetName() const
			{
				return "Render Window";
			}

			bool RenderWindow::HasDependencies() const
			{
				return true;
			}

			Array<String> RenderWindow::GetDependencies() const
			{
				return { "D3D11 Patch" };
			}

			bool RenderWindow::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				// In version 1.6.1130, this is fixed, however, the camera is being installed
				return eEditorCurrentVersion <= EDITOR_SKYRIM_SE_LAST;
			}

			bool RenderWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&_oldWndProc =
						voltek::detours_function_class_jump(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)), &RenderWindow::HKWndProc);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), &RenderWindow::setFlagLoadCell);
					EditorAPI::SkyrimSpectialEdition::BGSRenderWindow::Singleton = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));

					if (GlobalEnginePtr->GetEditorVersion() >= EDITOR_SKYRIM_SE_1_6_1130)
					{
						auto rel = _RELDATA_RAV(3);
						lpRelocator->PatchNop(rel, 0xB);
						lpRelocator->DetourCall(rel, (uintptr_t)&DrawFrameEx);
					}

					return true;
				}

				return false;
			}

			bool RenderWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			RenderWindow::RenderWindow() : BaseWindow(), Classes::CUIBaseWindow(),
				_BlockInputMessage(true)
			{
				Assert(!GlobalRenderWindowPtr);
				GlobalRenderWindowPtr = this;
			}

			LRESULT CALLBACK RenderWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG) 
				{
					GlobalRegistratorWindowPtr->RegisterMajor(Hwnd, "RenderWindow");
					GlobalRenderWindowPtr->m_hWnd = Hwnd;
					GlobalRenderWindowPtr->_BlockInputMessage = true;

					return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(),
						Hwnd, Message, wParam, lParam);
				}
				////// Fix WHITE area (Eats my eyes at startup) (only 1.6.1130 or newer)
				else if (Message == WM_PAINT)
				{
					PAINTSTRUCT ps;
					HDC dc = BeginPaint(Hwnd, &ps);

					::Core::Classes::UI::CUICanvas Canvas = dc;
					Canvas.Fill(RGB(0x6A, 0x6A, 0x6A));
					
					EndPaint(Hwnd, &ps);
					return S_OK;
				}
				else if (Message == WM_ERASEBKGND)
				{
					// An application should return nonzero if it erases the background; otherwise, it should return zero.
					return S_FALSE;
				}
				///////
				// Don't let us reduce the window too much
				else if (Message == WM_GETMINMAXINFO)
				{
					if (lParam)
					{
						LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
						lpMMI->ptMinTrackSize.x = 96;	// 96 min tex size
						lpMMI->ptMinTrackSize.y = 96;
					}

					return S_OK;
				}
				else if (GlobalRenderWindowPtr->_BlockInputMessage) 
				{
					switch (Message) 
					{
					case WM_KEYUP:
					case WM_KEYDOWN:
					case WM_SYSCHAR:
					case WM_SYSKEYUP:
					case WM_SYSKEYDOWN:
					case WM_MOUSEMOVE:
					case WM_MOUSEWHEEL:
					case WM_LBUTTONUP:
					case WM_LBUTTONDOWN:
					case WM_LBUTTONDBLCLK:
					case WM_RBUTTONUP:
					case WM_RBUTTONDOWN:
					case WM_RBUTTONDBLCLK:
					case WM_MBUTTONUP:
					case WM_MBUTTONDOWN:
					case WM_MBUTTONDBLCLK:
						// block messages
						return 0;
					default:
						break;
					}
				}
				else if ((Message == WM_KEYUP) && (wParam == VK_F1))
				{
					gImGuiShowDrawInfo = !gImGuiShowDrawInfo;
				}

				return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void RenderWindow::ImGuiDrawInfo()
			{
				// IMGUI
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				if (gImGuiShowDrawInfo)
				{
					// IMGUI DRAWINFO

					ImGui::SetNextWindowPos({ 5.0f, 5.0f });
					ImGui::Begin("Display Info", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDecoration |
						ImGuiWindowFlags_AlwaysAutoResize);
					ImGui::PushFont(imguiFonts[1]);

					auto& io = ImGui::GetIO();
			
					ImGui::Text("FPS: %.0f", io.Framerate);
					ImGui::NewLine();

					BGSRenderWindow* RenderWindow = BGSRenderWindow::Singleton.GetSingleton();
					if (RenderWindow)
					{
						auto Cell = RenderWindow->GetCurrentCell();
						if (Cell)
						{
							auto Name = Cell->EditorID;
							if (!Name) Name = "";

							if (Cell->IsInterior())
								ImGui::Text("Current Cell: %s (%08X)", EditorAPI::BSString::Converts::AnsiToUtf8(Name).c_str(), 
									Cell->FormID);
							else
								ImGui::Text("Current Cell: %s (%i, %i) (%08X)", EditorAPI::BSString::Converts::AnsiToUtf8(Name).c_str(), 
									Cell->GridX, Cell->GridY, Cell->FormID);
						}

						const auto& CameraPos = RenderWindow->Camera->GetPosition();
						ImGui::Text("Camera: %.3f, %.3f, %.3f", CameraPos.x, CameraPos.y, CameraPos.z);
					}

					ImGui::PopFont();
					ImGui::PushFont(imguiFonts[2]);
					ImGui::TextColored(gImGuiGreyColor, "(Show/Hide press key F1)");
					ImGui::PopFont();
					ImGui::End();
				}

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}

			void RenderWindow::DrawFrameEx(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
			{
				if (!(GlobalRenderWindowPtr->_ImagespaceAA))
				{
					GlobalRenderWindowPtr->_ImagespaceAA = std::make_unique<ImagespaceAA>();
					if (!GlobalRenderWindowPtr->_ImagespaceAA->Install(GlobalRenderWindowPtr->Handle))
						_ERROR("An error occurred in the shader compilation.");
				}
	
				if (pointer_d3d11DeviceContext && gGlobAddrDeviceContext)
				{
					auto RenderTarget = (ID3D11RenderTargetView**)(gGlobAddrDeviceContext + 0x88);
					auto RenderTarget2 = (ID3D11RenderTargetView**)(gGlobAddrDeviceContext + 0xA48);	// Used by gizmo enabled
					if (RenderTarget && RenderTarget2)
					{
						ComPtr<ID3D11RenderTargetView> pRenderTargetView;
						ComPtr<ID3D11DepthStencilView> pDepthStencilView;
						pointer_d3d11DeviceContext->OMGetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.GetAddressOf());

						if (pRenderTargetView.Get() == *RenderTarget2)
						{
							pointer_d3d11DeviceContext->OMSetRenderTargets(1, RenderTarget, nullptr);
							GlobalRenderWindowPtr->_ImagespaceAA->Draw(This);
							ImGuiDrawInfo();
							pointer_d3d11DeviceContext->OMSetRenderTargets(1, RenderTarget2, pDepthStencilView.Get());
						}
						else if (pRenderTargetView.Get() == *RenderTarget)
						{
							GlobalRenderWindowPtr->_ImagespaceAA->Draw(This);
							ImGuiDrawInfo();
						}
					}
				}

				// PRESENT

				This->Present(SyncInterval, Flags);
			}

			void RenderWindow::setFlagLoadCell()
			{
				GlobalRenderWindowPtr->_BlockInputMessage = false;
			}
		}
	}
}
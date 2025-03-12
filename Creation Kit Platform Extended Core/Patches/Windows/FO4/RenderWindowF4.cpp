// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/RegistratorWindow.h"
#include "RenderWindowF4.h"
#include "MainWindowF4.h"
#include "Editor API/FO4/TESF4.h"
#include "Editor API/FO4/BGSRenderWindow.h"
#include "Editor API/FO4/BGSRenderWindowCamera.h"
#include "UITheme/VarCommon.h"

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace Microsoft::WRL;

		extern ID3D11Device* pointer_d3d11DeviceIntf;
		extern ID3D11DeviceContext* pointer_d3d11DeviceContext;
		extern ImFont* imguiFonts[3];
		extern uintptr_t gGlobAddrDeviceContext;

		ImVec4 gImGuiGreenColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		ImVec4 gImGuiOrangeColor = { 1.0f, 0.6f, 0.0f, 1.0f };
		ImVec4 gImGuiRedColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		ImVec4 gImGuiGreyColor = { 0.5f, 0.5f, 0.5f, 1.0f };

		static bool gImGuiShowDrawInfo = true;

		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::EditorAPI::Fallout4;

			RenderWindow* GlobalRenderWindowPtr = nullptr;
			RenderWindow::Area rcSafeDrawArea;

			uintptr_t pointer_RenderWindow_Mov_data[11];

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
				return eEditorCurrentVersion <= EDITOR_FALLOUT_C4_LAST;
			}

			bool RenderWindow::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				auto PatchVer = lpRelocationDatabaseItem->Version();

				if ((PatchVer == 1) || (PatchVer == 2))
				{
					*(uintptr_t*)&_oldWndProc =
						voltek::detours_function_class_jump(_RELDATA_ADDR(0), (uintptr_t)&HKWndProc);
					_TempDrawArea = (Area*)_RELDATA_ADDR(1);
					TESUnknown::Instance = (TESUnknown*)_RELDATA_ADDR(2);
					BGSRenderWindow::Singleton = _RELDATA_ADDR(3);
				
					for (uint32_t i = 4; i < lpRelocationDatabaseItem->Count() && i < 14; i++)
						pointer_RenderWindow_Mov_data[i - 4] = _RELDATA_ADDR(i);
					
					BGSRenderWindow::Settings::Movement::FlagsSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[0];
					BGSRenderWindow::Settings::Movement::SnapGridValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[1];
					BGSRenderWindow::Settings::Movement::SnapAngleValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[2];
					BGSRenderWindow::Settings::Movement::ArrowSnapValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[3];
					BGSRenderWindow::Settings::Movement::ObjectRotateValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[4];
					BGSRenderWindow::Settings::Movement::ObjectMoveValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[5];
					BGSRenderWindow::Settings::Movement::CameraRotateValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[6];
					BGSRenderWindow::Settings::Movement::CameraZoomValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[7];
					BGSRenderWindow::Settings::Movement::CameraZoomOrthoValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[8];
					BGSRenderWindow::Settings::Movement::CameraPanValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[9];
					BGSRenderWindow::Settings::Movement::LandspaceMultValueSingleton = (uintptr_t)&pointer_RenderWindow_Mov_data[10];

					auto rel = _RELDATA_RAV(15);

					if (PatchVer == 1)
						lpRelocator->PatchNop(rel, 0x4B);
					else
						lpRelocator->PatchNop(rel, 0x44);

					lpRelocator->DetourCall(rel, (uintptr_t)&ImGuiDraw);

					rel = _RELDATA_RAV(16);
					lpRelocator->PatchNop(rel, 0x14);
					lpRelocator->DetourCall(rel, (uintptr_t)&UpdateDrawInfo);

					return true;
				}

				return false;
			}

			bool RenderWindow::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			RenderWindow::RenderWindow() : BaseWindow(), Classes::CUIBaseWindow()
			{
				Assert(!GlobalRenderWindowPtr);
				GlobalRenderWindowPtr = this;
			}

			LRESULT CALLBACK RenderWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				auto Result = ImGui_ImplWin32_WndProcHandler(Hwnd, Message, wParam, lParam);
				if (Result) return Result;

				switch (Message)
				{
					case WM_INITDIALOG:
					{
						GlobalRegistratorWindowPtr->RegisterMajor(Hwnd, "RenderWindow");
						GlobalRenderWindowPtr->m_hWnd = Hwnd;
						return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
					}
					// Don't let us reduce the window too much
					case WM_GETMINMAXINFO:
					{
						if (lParam)
						{
							LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
							lpMMI->ptMinTrackSize.x = 96;	// 96 min tex size
							lpMMI->ptMinTrackSize.y = 96;
						}
						return S_OK;
					}
					case WM_ACTIVATE:
					{
						// Fix bug loss of window size

						if (LOWORD(wParam) == WA_INACTIVE)
							rcSafeDrawArea = *_TempDrawArea;
						else
							*_TempDrawArea = rcSafeDrawArea;
					}
					break;
					case WM_KEYUP:
					{
						if (wParam == VK_F1)
							gImGuiShowDrawInfo = !gImGuiShowDrawInfo;
					}
					break;
				}

				return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void RenderWindow::ImGuiDraw(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
			{
				if (pointer_d3d11DeviceContext && gGlobAddrDeviceContext)
				{
					auto RenderTarget = (ID3D11RenderTargetView**)(gGlobAddrDeviceContext + 0x88);
					if (RenderTarget)
					{
						ComPtr<ID3D11RenderTargetView> pRenderTargetViews;
						pointer_d3d11DeviceContext->OMGetRenderTargets(1, pRenderTargetViews.GetAddressOf(), nullptr);

						if (pRenderTargetViews.Get() == *RenderTarget)
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

								ImGui::Text("Draw calls: ");
								ImGui::SameLine(0.0f, 0.0f);

								if (BGSRenderWindow::DrawInfo::DrawCalls < 8000)
									ImGui::TextColored(gImGuiGreenColor, "%u", BGSRenderWindow::DrawInfo::DrawCalls);
								else if (BGSRenderWindow::DrawInfo::DrawCalls < 12000)
									ImGui::TextColored(gImGuiOrangeColor, "%u", BGSRenderWindow::DrawInfo::DrawCalls);
								else
									ImGui::TextColored(gImGuiRedColor, "%u", BGSRenderWindow::DrawInfo::DrawCalls);

								ImGui::SameLine(0.0f, 0.0f);
								ImGui::Text(" polys: %u fps: %u", BGSRenderWindow::DrawInfo::Polys, BGSRenderWindow::DrawInfo::FramePerSecond);
								ImGui::NewLine();

								BGSRenderWindow* RenderWindow = BGSRenderWindow::Singleton.GetSingleton();
								if (RenderWindow)
								{
									auto Cell = RenderWindow->GetCurrentCell();
									if (Cell)
									{
										auto EditorID = Cell->GetEditorID_NoVTable();

										if (Cell->IsInterior())
											ImGui::Text("Current Cell: %s (%08X)", EditorID, Cell->FormID);
										else
											ImGui::Text("Current Cell: %s (%i, %i) (%08X)", EditorID, Cell->GridX, Cell->GridY, Cell->FormID);
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
					}
				}

				// PRESENT

				This->Present(SyncInterval, Flags);
			}

			void RenderWindow::UpdateDrawInfo(char* Dest, UINT Size, const char* FormatStr, ...)
			{
				// parse "%u main draw calls, %u polys, %s textures, %u FPS"

				va_list ap;
				va_start(ap, FormatStr);

				BGSRenderWindow::DrawInfo::DrawCalls = va_arg(ap, UINT);
				BGSRenderWindow::DrawInfo::Polys = va_arg(ap, UINT);
				strcpy_s(BGSRenderWindow::DrawInfo::TexturesSize, va_arg(ap, char*));
				BGSRenderWindow::DrawInfo::FramePerSecond = va_arg(ap, UINT);

				va_end(ap);
			}
		}
	}
}
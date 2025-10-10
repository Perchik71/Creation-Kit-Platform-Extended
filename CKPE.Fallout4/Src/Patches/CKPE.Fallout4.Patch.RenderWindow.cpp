// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <wrl/client.h>
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <CKPE.Keyboard.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BGSRenderWindow.h>
#include <Patches/CKPE.Fallout4.Patch.RenderWindow.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			extern ID3D11DeviceContext* pointer_d3d11DeviceContext;
			extern ImFont* imguiFonts[3];
			extern ImVec4 gImGuiGreyColor;
			extern std::uintptr_t gGlobAddrDeviceContext;

			ImVec4 gImGuiGreenColor = { 0.0f, 1.0f, 0.0f, 1.0f };
			ImVec4 gImGuiOrangeColor = { 1.0f, 0.6f, 0.0f, 1.0f };
			ImVec4 gImGuiRedColor = { 1.0f, 0.0f, 0.0f, 1.0f };
			ImVec4 gImGuiGreyColor = { 0.5f, 0.5f, 0.5f, 1.0f };

			static float StepInRender = 15.f;
			static bool HideMainImguiWnd = true;

			//static std::uintptr_t pointer_RenderWindow_Mov_data[11];

			struct Area
			{
				SIZE WindowSize;
				SIZE WindowSize2;	// I don't know why, but they are the same

				inline Area& operator=(const Area& a)
				{
					WindowSize = a.WindowSize;
					WindowSize2 = a.WindowSize2;

					return *this;
				}
			};
			static Area* _TempDrawArea, rcSafeDrawArea;

			RenderWindow::RenderWindow() : Common::PatchBaseWindow()
			{
				SetName("Render Window");
				Singleton = this;
			}

			bool RenderWindow::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* RenderWindow::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool RenderWindow::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> RenderWindow::GetDependencies() const noexcept(true)
			{
				return { "D3D11 Patch" };
			}

			bool RenderWindow::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool RenderWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				StepInRender = _READ_OPTION_FLOAT("Graphics", "fStepInRender", 15.f);
				StepInRender = std::min(std::max(StepInRender, 15.f), 100.f);

				*(std::uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKWndProc);
				_TempDrawArea = (Area*)__CKPE_OFFSET(1);

			//	TESUnknown::Instance = (TESUnknown*)__CKPE_OFFSET(2);
				EditorAPI::BGSRenderWindow::Singleton = __CKPE_OFFSET(3);

				EditorAPI::BGSRenderWindow::Settings::Movement::FlagsSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(4);
				EditorAPI::BGSRenderWindow::Settings::Movement::SnapGridValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(5);
				EditorAPI::BGSRenderWindow::Settings::Movement::SnapAngleValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(6);
				EditorAPI::BGSRenderWindow::Settings::Movement::ArrowSnapValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(7);
				EditorAPI::BGSRenderWindow::Settings::Movement::ObjectRotateValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(8);
				EditorAPI::BGSRenderWindow::Settings::Movement::ObjectMoveValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(9);
				EditorAPI::BGSRenderWindow::Settings::Movement::CameraRotateValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(10);
				EditorAPI::BGSRenderWindow::Settings::Movement::CameraZoomValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(11);
				EditorAPI::BGSRenderWindow::Settings::Movement::CameraZoomOrthoValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(12);
				EditorAPI::BGSRenderWindow::Settings::Movement::CameraPanValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(13);
				EditorAPI::BGSRenderWindow::Settings::Movement::LandspaceMultValueSingleton = (EditorAPI::Setting*)__CKPE_OFFSET(14);

				auto rel = __CKPE_OFFSET(15);

				if (verPatch == 1)
					SafeWrite::WriteNop(rel, 0x4B);
				else
					SafeWrite::WriteNop(rel, 0x44);

				Detours::DetourCall(rel, (std::uintptr_t)&DrawFrameEx);

				rel = __CKPE_OFFSET(16);
				SafeWrite::WriteNop(rel, 0x14);
				Detours::DetourCall(rel, (std::uintptr_t)&UpdateDrawInfo);

				*(std::uintptr_t*)&EditorAPI::BGSRenderWindow::Pick::GetRefFromNiNode =
					Detours::DetourClassJump(__CKPE_OFFSET(17), (uintptr_t)&EditorAPI::BGSRenderWindow::Pick::HKGetRefFromNiNode);

				return true;
			}

			INT_PTR CALLBACK RenderWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					RenderWindow::Singleton->m_hWnd = Hwnd;

					return CallWindowProc(RenderWindow::Singleton->GetOldWndProc(),
						Hwnd, Message, wParam, lParam);
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
				// Fix bug loss of window size
				else if (Message == WM_GETMINMAXINFO)
				{
					if (LOWORD(wParam) == WA_INACTIVE)
						rcSafeDrawArea = *_TempDrawArea;
					else
						*_TempDrawArea = rcSafeDrawArea;

					return S_OK;
				}
				else
				{
					/*if (Message == WM_KEYDOWN)
					{
						if (!Keyboard::IsAltPressed() && !Keyboard::IsControlPressed() && Keyboard::IsShiftPressed())
						{
							switch (wParam)
							{
							case 'W':
							{
								auto delta = StepInRender;
								auto& local = const_cast<EditorAPI::NiAPI::NiTransform&>(
									EditorAPI::BGSRenderWindow::Singleton->Camera->Node->GetLocalTransform());

								local.m_Translate.x += local.m_Rotate.m_pEntry[0][1] * delta;
								local.m_Translate.y += local.m_Rotate.m_pEntry[1][1] * delta;
								local.m_Translate.z += local.m_Rotate.m_pEntry[2][1] * delta;
								return 0;
								break;
							}
							case 'S':
							{
								auto delta = -StepInRender;
								auto& local = const_cast<EditorAPI::NiAPI::NiTransform&>(
									EditorAPI::BGSRenderWindow::Singleton->Camera->Node->GetLocalTransform());

								local.m_Translate.x += local.m_Rotate.m_pEntry[0][1] * delta;
								local.m_Translate.y += local.m_Rotate.m_pEntry[1][1] * delta;
								local.m_Translate.z += local.m_Rotate.m_pEntry[2][1] * delta;
								break;
							}
							case 'A':
							{
								auto delta = -StepInRender;
								auto& local = const_cast<EditorAPI::NiAPI::NiTransform&>(
									EditorAPI::BGSRenderWindow::Singleton->Camera->Node->GetLocalTransform());

								local.m_Translate.x += local.m_Rotate.m_pEntry[0][0] * delta;
								local.m_Translate.y += local.m_Rotate.m_pEntry[1][0] * delta;
								local.m_Translate.z += local.m_Rotate.m_pEntry[2][0] * delta;
								return 0;
								break;
								break;
							}
							case 'D':
							{
								auto delta = StepInRender;
								auto& local = const_cast<EditorAPI::NiAPI::NiTransform&>(
									EditorAPI::BGSRenderWindow::Singleton->Camera->Node->GetLocalTransform());

								local.m_Translate.x += local.m_Rotate.m_pEntry[0][0] * delta;
								local.m_Translate.y += local.m_Rotate.m_pEntry[1][0] * delta;
								local.m_Translate.z += local.m_Rotate.m_pEntry[2][0] * delta;
								break;
								break;
							}
							default:
								break;
							}
						}
					}
					else */if (Message == WM_KEYUP)
					{
						if (!Keyboard::IsAltPressed() && !Keyboard::IsControlPressed() && !Keyboard::IsShiftPressed())
						{
							if (wParam == VK_F1)
								HideMainImguiWnd = !HideMainImguiWnd;
						}
					}
				}

				return CallWindowProc(RenderWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void RenderWindow::UpdateDrawInfo(char* Dest, UINT Size, const char* FormatStr, ...)
			{
				// parse "%u main draw calls, %u polys, %s textures, %u FPS"

				va_list ap;
				va_start(ap, FormatStr);

				EditorAPI::BGSRenderWindow::DrawInfo::DrawCalls = va_arg(ap, UINT);
				EditorAPI::BGSRenderWindow::DrawInfo::Polys = va_arg(ap, UINT);
				strcpy_s(EditorAPI::BGSRenderWindow::DrawInfo::TexturesSize, va_arg(ap, char*));
				EditorAPI::BGSRenderWindow::DrawInfo::FramePerSecond = va_arg(ap, UINT);

				va_end(ap);
			}

			void RenderWindow::DrawFrameEx(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
			{
				auto Root = RenderWindow::Singleton.GetSingleton();
				if (!Root) return;

				if (!(Root->_ImagespaceAA))
				{
					Root->_ImagespaceAA = std::make_unique<D3D11ImagespaceAA>();
					if (!Root->_ImagespaceAA->Install(Root->Handle))
						_ERROR("An error occurred in the shader compilation.");
				}

				if (pointer_d3d11DeviceContext && gGlobAddrDeviceContext)
				{
					auto RenderTarget = (ID3D11RenderTargetView**)(gGlobAddrDeviceContext + 0x88);
					auto RenderTarget2 = (ID3D11RenderTargetView**)(gGlobAddrDeviceContext + 0xA48);	// Used by gizmo enabled
					if (RenderTarget && RenderTarget2)
					{
						Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
						Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
						pointer_d3d11DeviceContext->OMGetRenderTargets(1, pRenderTargetView.GetAddressOf(),
							pDepthStencilView.GetAddressOf());

						if (pRenderTargetView.Get() == *RenderTarget2)
						{
							pointer_d3d11DeviceContext->OMSetRenderTargets(1, RenderTarget, nullptr);
							Root->_ImagespaceAA->Draw(This);
							ImGuiDrawInfo();
							pointer_d3d11DeviceContext->OMSetRenderTargets(1, RenderTarget2, pDepthStencilView.Get());
						}
						else if (pRenderTargetView.Get() == *RenderTarget)
						{
							Root->_ImagespaceAA->Draw(This);
							ImGuiDrawInfo();
						}
					}
				}

				// PRESENT

				This->Present(SyncInterval, Flags);
			}

			void RenderWindow::ImGuiDrawInfo() noexcept(true)
			{
				if (HideMainImguiWnd)
					return;

				// IMGUI
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				auto Root = EditorAPI::BGSRenderWindow::Singleton.GetSingleton();
				auto& io = ImGui::GetIO();
				if (Root)
				{
					// io.MousePos always { -1, -1 }, let's fix it
					auto p = Root->GetMousePos();
					io.MousePos = { (float)p.x, (float)p.y };
				}

				// DRAW

				ImGui::SetNextWindowPos({ 5.0f, 5.0f });
				ImGui::Begin("#Default Info Overlay", nullptr, ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);

				if (ImGui::BeginTable("##Info Overlay Data", 2))
				{
					ImGui::PushFont(imguiFonts[1]);
					ImGui::TableSetupColumn("First", ImGuiTableColumnFlags_WidthStretch, 130);
					ImGui::TableSetupColumn("Second", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("FPS:");

					ImGui::TableNextColumn();
					ImGui::Text("%.0f", io.Framerate);

					auto ActiveCell = EditorAPI::BGSRenderWindow::Singleton->GetCurrentCell();
					if (ActiveCell)
					{
						ImGui::Dummy(ImVec2(1, 10));

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Current Cell: ");

						ImGui::TableNextColumn();

						auto Name = ActiveCell->GetEditorID_orig();
						if (!Name) Name = "";

						if (ActiveCell->HasInterior())
							ImGui::Text("%s (%08X)", StringUtils::WinCPToUtf8(Name).c_str(), ActiveCell->FormID);
						else
							ImGui::Text("%s (%i, %i) (%08X)", StringUtils::WinCPToUtf8(Name).c_str(),
								ActiveCell->GridX, ActiveCell->GridY, ActiveCell->FormID);

						/*auto& ii = EditorAPI::BGSRenderWindow::Singleton->Camera->Node->GetLocalTransform();

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Camera:");
						ImGui::TableNextColumn();
						ImGui::Text("p(%.3f, %.3f, %.3f)", ii.m_Translate.x, ii.m_Translate.y, ii.m_Translate.z);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::TableNextColumn();
						ImGui::Text("r(%.3f, %.3f, %.3f)", Math::Rad2Deg(ii.m_Rotate.m_pEntry[1][0]),
							Math::Rad2Deg(ii.m_Rotate.m_pEntry[1][1]), Math::Rad2Deg(ii.m_Rotate.m_pEntry[1][2]));*/

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::TableNextColumn();
						ImGui::PushFont(imguiFonts[2]);
						ImGui::TextColored({ 0.4, 0.4, 0.4, 1.0 }, "(Show/Hide press key F1)");
						ImGui::PopFont();
					}

					ImGui::PopFont();
					ImGui::EndTable();
				}

				ImGui::End();

				// END IMGUI

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}
		}
	}
}
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
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/BGSRenderWindow.h>
#include <Patches/CKPE.SkyrimSE.Patch.RenderWindow.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			extern ID3D11DeviceContext* pointer_d3d11DeviceContext;
			extern ImFont* imguiFonts[3];
			extern std::uintptr_t gGlobAddrDeviceContext;

			static float StepInRender = 15.f;

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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool RenderWindow::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto _interface = Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				StepInRender = _READ_OPTION_FLOAT("Graphics", "fStepInRender", 15.f);
				StepInRender = std::min(std::max(StepInRender, 15.f), 100.f);

				*(uintptr_t*)&_oldWndProc = Detours::DetourClassJump(__CKPE_OFFSET(0), (uintptr_t)&HKWndProc);
				Detours::DetourJump(__CKPE_OFFSET(1), (uintptr_t)&RenderWindow::setFlagLoadCell);
				EditorAPI::BGSRenderWindow::Singleton = __CKPE_OFFSET(2);

				auto rel = __CKPE_OFFSET(3);
				SafeWrite::WriteNop(rel, 0xB);
				Detours::DetourCall(rel, (uintptr_t)&DrawFrameEx);

				*(uintptr_t*)&EditorAPI::BGSRenderWindow::Pick::Update =
					Detours::DetourClassJump(__CKPE_OFFSET(6), (uintptr_t)&EditorAPI::BGSRenderWindow::Pick::HKUpdate);
				*(uintptr_t*)&EditorAPI::BGSRenderWindow::Pick::GetRefFromTriShape = __CKPE_OFFSET(5);
				Detours::DetourCall(__CKPE_OFFSET(4), 
					(uintptr_t)&EditorAPI::BGSRenderWindow::Pick::HKGetRefFromTriShape);

				// Enable drawing always
				if (VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_1130)
					SafeWrite::WriteNop(__CKPE_OFFSET(7), 0x69);

				return true;
			}

			INT_PTR CALLBACK RenderWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					RenderWindow::Singleton->m_hWnd = Hwnd;
					RenderWindow::Singleton->_BlockInputMessage = true;

					return CallWindowProc(RenderWindow::Singleton->GetOldWndProc(),
						Hwnd, Message, wParam, lParam);
				}
				////// Fix WHITE area (Eats my eyes at startup) (only 1.6.1130 or newer)
				/*else if (Message == WM_PAINT)
				{
					PAINTSTRUCT ps;
					HDC dc = BeginPaint(Hwnd, &ps);

					if (RenderWindow::Singleton->_BlockInputMessage)
					{
						Canvas canvas = dc;
						canvas.Fill(RGB(0x6A, 0x6A, 0x6A));
					}

					EndPaint(Hwnd, &ps);
					return S_OK;
				}*/
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
				else if (RenderWindow::Singleton->_BlockInputMessage)
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
				else
				{
					if (Message == WM_KEYDOWN)
					{
						if (Keyboard::IsShiftPressed())
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
				}

				return CallWindowProc(RenderWindow::Singleton->GetOldWndProc(), Hwnd, Message, wParam, lParam);
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

			void RenderWindow::setFlagLoadCell() noexcept(true)
			{
				RenderWindow::Singleton->_BlockInputMessage = false;
			}

			void RenderWindow::ImGuiDrawInfo() noexcept(true)
			{
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

					auto ii = EditorAPI::BGSRenderWindow::Singleton->Camera->Node->GetLocalTransform();

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Camera:");

					ImGui::TableNextColumn();
					ImGui::Text("%.0f %.0f %.0f", ii.m_Translate.x, ii.m_Translate.y, ii.m_Translate.z);

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
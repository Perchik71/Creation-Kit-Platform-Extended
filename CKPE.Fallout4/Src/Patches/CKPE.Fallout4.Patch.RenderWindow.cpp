// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <wrl/client.h>
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include <CKPE.Keyboard.h>
#include <CKPE.Graphics.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BGSRenderWindow.h>
#include <Patches/CKPE.Fallout4.Patch.MainWindow.h>
#include <Patches/CKPE.Fallout4.Patch.RenderWindow.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			extern std::vector<EditorAPI::CKPE_CoordRefr> data_FakeMoveLight_coord;
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
				StepInRender = _READ_OPTION_FLOAT("Graphics", "fStepInRender", 15.f);
				StepInRender = std::min(std::max(StepInRender, 15.f), 100.f);

				*(std::uintptr_t*)&_oldWndProc = Common::Relocation(Common::ID{ 234564, 1885140 }).WriteJump(&HKWndProc);
				_TempDrawArea = Common::Relocation<Area*>(Common::ID(171553)).Get();
			
				EditorAPI::BGSRenderWindow::Singleton = Common::ID(383337).Address();
				
				EditorAPI::BGSRenderWindow::Settings::Movement::FlagsSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(381092)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::SnapGridValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384553)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::SnapAngleValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384587)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::ArrowSnapValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384565)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::ObjectRotateValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384591)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::ObjectMoveValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384559)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::CameraRotateValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384592)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::CameraZoomValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384595)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::CameraZoomOrthoValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384599)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::CameraPanValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384600)).Get();
				EditorAPI::BGSRenderWindow::Settings::Movement::LandspaceMultValueSingleton = Common::Relocation<EditorAPI::Setting*>(Common::ID(384603)).Get();

				const auto rel1 = Common::Relocation(Common::ID{ 408886, 1938434 }, Common::Offset{ 0xC5, 0x1E2 });

				if (VersionLists::GetEditorVersion() < VersionLists::EDITOR_FALLOUT_C4_1_10_943_1)
					rel1.WriteFill(Common::NOP, 0x4B);
				else
					rel1.WriteFill(Common::NOP, 0x44);

				rel1.WriteCall(&DrawFrameEx);

				const auto rel2 = Common::Relocation(Common::ID{ 467263, 1638356 }, Common::Offset{ 0x1DC, 0x211 });
				rel2.WriteFill(Common::NOP, 0x14);
				rel2.WriteCall(&UpdateDrawInfo);

				*(std::uintptr_t*)&EditorAPI::BGSRenderWindow::Pick::GetRefFromNiNode = Common::Relocation(Common::ID(411210)).WriteJump
					(&EditorAPI::BGSRenderWindow::Pick::HKGetRefFromNiNode);

				return true;
			}

			INT_PTR CALLBACK RenderWindow::HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
			{
				if (Message == WM_INITDIALOG)
				{
					RenderWindow::Singleton->m_hWnd = Hwnd;

					Common::Interface::GetSingleton()->GetDockingManager()->AddWindow((std::uintptr_t)Hwnd);
					SetWindowPos(Hwnd, nullptr, 0, 0, 0, 0,
						SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

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
						auto lpMMI = (LPMINMAXINFO)lParam;
						lpMMI->ptMinTrackSize.x = 96;	// 96 min tex size
						lpMMI->ptMinTrackSize.y = 96;
					}

					return S_OK;
				}
				// Fix bug loss of window size
				else if (Message == WM_ACTIVATE)
				{
					if (LOWORD(wParam) == WA_INACTIVE)
						rcSafeDrawArea = *_TempDrawArea;
					else
						*_TempDrawArea = rcSafeDrawArea;

					return S_OK;
				}
				else
				{
					if (Message == WM_LBUTTONUP)
					{
						data_FakeMoveLight_coord.clear();
						//data_FakeMoveLight_coord = EditorAPI::NiAPI::ZERO_P3;
					}
					else if (Message == WM_KEYDOWN)
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

								local.m_Translate.x += local.m_Rotate.m_pEntry[1][0] * delta;
								local.m_Translate.y += local.m_Rotate.m_pEntry[1][1] * delta;
								local.m_Translate.z += local.m_Rotate.m_pEntry[1][2] * delta;
								return 0;
							}
							case 'S':
							{
								auto delta = -StepInRender;
								auto& local = const_cast<EditorAPI::NiAPI::NiTransform&>(
									EditorAPI::BGSRenderWindow::Singleton->Camera->Node->GetLocalTransform());

								local.m_Translate.x += local.m_Rotate.m_pEntry[1][0] * delta;
								local.m_Translate.y += local.m_Rotate.m_pEntry[1][1] * delta;
								local.m_Translate.z += local.m_Rotate.m_pEntry[1][2] * delta;
								return 0;
							}
							case 'A':
							{
								auto delta = -StepInRender;
								auto& local = const_cast<EditorAPI::NiAPI::NiTransform&>(
									EditorAPI::BGSRenderWindow::Singleton->Camera->Node->GetLocalTransform());

								local.m_Translate.x += local.m_Rotate.m_pEntry[0][0] * delta;
								local.m_Translate.y += local.m_Rotate.m_pEntry[0][1] * delta;
								local.m_Translate.z += local.m_Rotate.m_pEntry[0][2] * delta;
								return 0;
							}
							case 'D':
							{
								auto delta = StepInRender;
								auto& local = const_cast<EditorAPI::NiAPI::NiTransform&>(
									EditorAPI::BGSRenderWindow::Singleton->Camera->Node->GetLocalTransform());

								local.m_Translate.x += local.m_Rotate.m_pEntry[0][0] * delta;
								local.m_Translate.y += local.m_Rotate.m_pEntry[0][1] * delta;
								local.m_Translate.z += local.m_Rotate.m_pEntry[0][2] * delta;
								return 0;
							}
							default:
								break;
							}
						}
					}
					else if (Message == WM_KEYUP)
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
						ImGui::Text("FPS/DC/Polys:");
						ImGui::TableNextColumn();
						ImGui::Text("%u / ", EditorAPI::BGSRenderWindow::DrawInfo::FramePerSecond);
						ImGui::SameLine(0.0f, 0.0f);
						if (EditorAPI::BGSRenderWindow::DrawInfo::DrawCalls < 8000)
							ImGui::TextColored(gImGuiGreenColor, "%u", EditorAPI::BGSRenderWindow::DrawInfo::DrawCalls);
						else if (EditorAPI::BGSRenderWindow::DrawInfo::DrawCalls < 12000)
							ImGui::TextColored(gImGuiOrangeColor, "%u", EditorAPI::BGSRenderWindow::DrawInfo::DrawCalls);
						else
							ImGui::TextColored(gImGuiRedColor, "%u", EditorAPI::BGSRenderWindow::DrawInfo::DrawCalls);
						ImGui::SameLine(0.0f, 0.0f);
						ImGui::Text(" / %u", EditorAPI::BGSRenderWindow::DrawInfo::Polys);

						auto ActiveCell = Root->GetCurrentCell();
						if (ActiveCell)
						{
							ImGui::Dummy(ImVec2(1, 10));

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Current Cell: ");

							ImGui::TableNextColumn();
							ImGui::TextDisabled("[?]");

							// ImGui::IsItemHovered() -- no worked

							auto ip = ImGui::GetCursorScreenPos();
							auto is = ImGui::GetItemRectSize();
					
							std::uint32_t uCountNPCs, uCountLight, uCountObject, uCountSelNPCs = 0,
								uCountSelLight = 0, uCountSelObject = 0;

							// It looks like the coordinates of the origin are the lower left corner (so ip.y - is.y - 2)
							if ((ip.x <= p.x) && ((ip.y - is.y - 2) <= p.y) && ((ip.x + is.x) > p.x) && (ip.y > p.y))
							{
								auto Counter = [](EditorAPI::Forms::TESObjectREFR** refrs, std::uint32_t count,
									std::uint32_t& npcs, std::uint32_t& lights, std::uint32_t& objs)
									{
										npcs = 0;
										lights = 0;
										objs = 0;

										for (std::uint32_t uId = 0; uId < count; uId++)
										{
											auto form = refrs[uId];
											auto formType = form->Parent->GetFormType();

											if (!form->HasDeleted()) 
											{
												switch (formType) 
												{
												case EditorAPI::Forms::TESForm::ftActor:
													npcs++;
													break;
												case EditorAPI::Forms::TESForm::ftLight:
													lights++;
													break;
												default:
													objs++;
													break;
												}
											}
										}
									};

								Counter(ActiveCell->GetItems(), ActiveCell->GetItemCount(), uCountNPCs, uCountLight, uCountObject);

								auto TotalSel = Root->PickHandler->Count;
								if (TotalSel > 0)
								{
									std::uint32_t i = 0;
									auto Items = Root->PickHandler->Items;
									for (auto It = Items->First; i < TotalSel; It = It->Next, i++)
									{
										auto form = It->Ref;
										if (form->HasDeleted()) continue;
										switch (form->GetParent()->GetFormType())
										{
										case EditorAPI::Forms::TESForm::ftActor:
											uCountSelNPCs++;
											break;
										case EditorAPI::Forms::TESForm::ftLight:
											uCountSelLight++;
											break;
										default:
											uCountSelObject++;
											break;
										}
									}

									ImGui::SetTooltip("Geometry:\n\tObjects: %d\n\tLights: %d\n\tNPCs: %d\nSelected:"
										"\n\tObjects: %d\n\tLights: %d\n\tNPCs: %d", uCountObject, uCountLight,
										uCountNPCs, uCountSelObject, uCountSelLight, uCountSelNPCs);
								}
								else
									ImGui::SetTooltip("Geometry:\n\tObjects: %u\n\tLights: %u\n\tNPCs: %u",
										uCountObject, uCountLight, uCountNPCs);
							}


							auto Name = ActiveCell->EditorID;
							if (!Name) Name = "";

							ImGui::SameLine(0, 5);
							if (ActiveCell->HasInterior())
								ImGui::Text("%s (%08X)", StringUtils::WinCPToUtf8(Name).c_str(), ActiveCell->FormID);
							else
								ImGui::Text("%s (%i, %i) (%08X)", StringUtils::WinCPToUtf8(Name).c_str(),
									ActiveCell->GridX, ActiveCell->GridY, ActiveCell->FormID);

							auto& ii = Root->Camera->Node->GetLocalTransform();

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Camera:");
							ImGui::TableNextColumn();
							ImGui::Text("p(%.3f, %.3f, %.3f)", ii.m_Translate.x, ii.m_Translate.y, ii.m_Translate.z);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TableNextColumn();
							ImGui::Text("r(%.3f, %.3f, %.3f)", Math::Rad2Deg(ii.m_Rotate.m_pEntry[1][0]),
								Math::Rad2Deg(ii.m_Rotate.m_pEntry[1][1]), Math::Rad2Deg(ii.m_Rotate.m_pEntry[1][2]));
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TableNextColumn();
							ImGui::PushFont(imguiFonts[2]);
							ImGui::TextColored(gImGuiGreyColor, "(Show/Hide press key F1)");
							ImGui::PopFont();
						}

						ImGui::PopFont();
						ImGui::EndTable();
					}
				}

				ImGui::End();

				// END IMGUI

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}
		}
	}
}
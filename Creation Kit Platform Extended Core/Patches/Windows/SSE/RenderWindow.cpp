// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/RegistratorWindow.h"
#include <Core/Timer.h>
#include "RenderWindow.h"
#include "MainWindow.h"
#include "Editor API/SSE/BGSRenderWindow.h"
#include "Editor API/SSE/BSGraphicsTypes.h"
#include "Editor API/SSE/TESObjectREFR.h"
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

		float RAD2DEG = 57.29577951308232;

		namespace SkyrimSpectialEdition
		{
			RenderWindow* GlobalRenderWindowPtr = nullptr;
			Core::Timer ImGuiTimer;

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
						voltek::detours_function_class_jump(_RELDATA_ADDR(0), &RenderWindow::HKWndProc);
					lpRelocator->DetourJump(_RELDATA_RAV(1), &RenderWindow::setFlagLoadCell);
					BGSRenderWindow::Singleton = _RELDATA_ADDR(2);
						
					if (GlobalEnginePtr->GetEditorVersion() >= EDITOR_SKYRIM_SE_1_6_1130)
					{
						auto rel = _RELDATA_RAV(3);
						lpRelocator->PatchNop(rel, 0xB);
						lpRelocator->DetourCall(rel, (uintptr_t)&DrawFrameEx);

						*(uintptr_t*)&BGSRenderWindow::Pick::Update =
							lpRelocator->DetourFunctionClass(_RELDATA_RAV(6), (uintptr_t)&BGSRenderWindow::Pick::HKUpdate);
						*(uintptr_t*)&BGSRenderWindow::Pick::GetRefFromTriShape = _RELDATA_ADDR(5);
						lpRelocator->DetourCall(_RELDATA_RAV(4), (uintptr_t)&BGSRenderWindow::Pick::HKGetRefFromTriShape);
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
					ImGuiTimer.Start();

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
				else if ((Message == WM_MOUSEMOVE) || (Message == WM_MOUSEWHEEL))
				{
					ImGuiTimer.Start();
				}

				return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
			}

			void RenderWindow::ImGuiDrawInfo()
			{
				// IMGUI
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				BGSRenderWindow* RenderWindow = BGSRenderWindow::Singleton.GetSingleton();
				auto& io = ImGui::GetIO();	
				if (RenderWindow)
				{
					// io.MousePos always { -1, -1 }, let's fix it
					auto p = RenderWindow->GetMousePos();
					io.MousePos = { (float)p.x, (float)p.y };
				}

				if (gImGuiShowDrawInfo)
				{
					// IMGUI DRAWINFO

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

						if (RenderWindow)
						{
							auto ActiveCell = RenderWindow->GetCurrentCell();
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
								auto rp = RenderWindow->GetMousePos();

								uint32_t uCountNPCs, uCountLight, uCountObject, uCountSelNPCs = 0, uCountSelLight = 0, uCountSelObject = 0;

								// It looks like the coordinates of the origin are the lower left corner (so ip.y - is.y - 2)
								if ((ip.x <= rp.x) && ((ip.y - is.y - 2) <= rp.y) && ((ip.x + is.x) > rp.x) && (ip.y > rp.y))
								{
									auto Counter = [](TESObjectCELL::Item* refrs, uint32_t count, uint32_t& npcs, uint32_t& lights, uint32_t& objs) 
									{
										uint32_t uId = 0;
										npcs = 0;
										lights = 0;
										objs = 0;

										if (!count)
											return;

										uint32_t total = 0;

										do
										{
											auto form = refrs[uId++].Refr;
											// empty (some kind of strange array, where there may be nothing between the elements, as if sparse)
											if (!form) continue;
											
											total++;

											if (form->IsDeleted()) continue;	
											switch (form->GetParent()->GetFormType())
											{
											case TESForm::ftNPC:
												npcs++;
												break;
											case TESForm::ftLight:
												lights++;
												break;
											default:
												objs++;
												break;
											}
										} while (uId != count);
									};

									Counter(ActiveCell->GetItems(), ActiveCell->GetItemCount(), uCountNPCs, uCountLight, uCountObject);

									auto TotalSel = RenderWindow->PickHandler->Count;
									if (TotalSel > 0)
									{
										uint32_t i = 0;
										auto Items = RenderWindow->PickHandler->Items;	
										for (auto It = Items->First; i < TotalSel; It = It->Next, i++)
										{
											auto form = It->Ref;
											if (form->IsDeleted()) continue;
											switch (form->GetParent()->GetFormType()) 
											{
											case TESForm::ftNPC:
												uCountSelNPCs++;
												break;
											case TESForm::ftLight:
												uCountSelLight++;
												break;
											default:
												uCountSelObject++;
												break;
											}
										}

										ImGui::SetTooltip("Geometry:\n\tObjects: %d\n\tLights: %d\n\tNPCs: %d\nSelected:\n\tObjects: %d\n\tLights: %d\n\tNPCs: %d", 
											uCountObject, uCountLight, uCountNPCs, uCountSelObject, uCountSelLight, uCountSelNPCs);
									}
									else
										ImGui::SetTooltip("Geometry:\n\tObjects: %u\n\tLights: %u\n\tNPCs: %u", uCountObject, uCountLight, uCountNPCs);
								}

								auto Name = ActiveCell->EditorID;
								if (!Name) Name = "";

								ImGui::SameLine(0, 5);
								if (ActiveCell->IsInterior())
									ImGui::Text("%s (%08X)", EditorAPI::BSString::Converts::AnsiToUtf8(Name).c_str(), 
										ActiveCell->FormID);
								else
									ImGui::Text("%s (%i, %i) (%08X)", EditorAPI::BSString::Converts::AnsiToUtf8(Name).c_str(),
										ActiveCell->GridX, ActiveCell->GridY, ActiveCell->FormID);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Camera:");

								ImGui::TableNextColumn();
								auto Cam = RenderWindow->Camera;
								auto CamPos = Cam->Position;
								ImGui::Text("%.3f, %.3f, %.3f", CamPos.x, CamPos.y, CamPos.z);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::TableNextColumn();
								ImGui::PushFont(imguiFonts[2]);
								ImGui::TextColored(gImGuiGreyColor, "(Show/Hide press key F1)");
								ImGui::PopFont();
							}
						}

						ImGui::PopFont();
						ImGui::EndTable();
					}
					
					ImGui::End();
				}

				if (RenderWindow && BGSRenderWindow::Pick::Result)
				{
					POINT pt;
					RECT rcWnd;

					GetCursorPos(&pt);
					GetWindowRect(RenderWindow->GetWindowHandle(), &rcWnd);

					if (gImGuiShowDrawInfo)
					{
						RECT rcWndToolInfo = { rcWnd.left, rcWnd.top, 330 + rcWnd.left, 160 + rcWnd.top };
						if (PtInRect(&rcWndToolInfo, pt))
							goto Skips;
					}

					if (PtInRect(&rcWnd, pt) && (ImGuiTimer.Get() > 1.5))
					{
						auto Ref = BGSRenderWindow::Pick::Result;
						auto FormParent = Ref->GetParent();
						auto MousePos = RenderWindow->GetMousePos();

						if (FormParent)
						{
							if (MousePos.y - 115 < 0)
								ImGui::SetNextWindowPos({ (float)MousePos.x, (float)(MousePos.y) });
							else
								ImGui::SetNextWindowPos({ (float)MousePos.x, (float)(MousePos.y - 115) });

							ImGui::Begin("Ref Info", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDecoration | 
								ImGuiWindowFlags_AlwaysAutoResize);

							if (ImGui::BeginTable("##Tooltip Contents", 2))
							{
								ImGui::PushFont(imguiFonts[1]);
								ImGui::TableSetupColumn("First", ImGuiTableColumnFlags_WidthStretch, 100);
								ImGui::TableSetupColumn("Second", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Object Reference:");
								ImGui::TableNextColumn();

								auto EDID = Ref->GetEditorID_NoVTable();
								if (!EDID) EDID = "<EMPTY>";

								ImGui::Text("%s%s (%08X)", EDID, (Ref->Active ? "*" : ""), Ref->FormID);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Base Form:");
								ImGui::TableNextColumn();

								EDID = FormParent->GetEditorID_NoVTable();
								if (!EDID) EDID = "<EMPTY>";

								ImGui::Text("%s%s (%08X)", EDID, (FormParent->Active ? "*" : ""), FormParent->FormID);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::TableNextColumn();
								ImGui::TextColored(gImGuiGreyColor, TESForm::GetFormTypeStr(FormParent->Type));

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Position:");
								ImGui::TableNextColumn();
								auto Vec3 = Ref->GetPosition();
								ImGui::Text("%.03f, %.03f, %.03f", Vec3.x, Vec3.y, Vec3.z);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Rotation:");
								ImGui::TableNextColumn();
								Vec3 = Ref->GetRotate();
								ImGui::Text("%.03f, %.03f, %.03f", Vec3.x * RAD2DEG, Vec3.y * RAD2DEG, Vec3.z * RAD2DEG);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Scale:");

								ImGui::TableNextColumn();
								ImGui::Text("%.03f", Ref->GetScale());

								ImGui::PopFont();
								ImGui::EndTable();
							}

							ImGui::End();
						}
					}
				Skips:
					{ /* nope : need compiler, lmao syntax error */ }
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
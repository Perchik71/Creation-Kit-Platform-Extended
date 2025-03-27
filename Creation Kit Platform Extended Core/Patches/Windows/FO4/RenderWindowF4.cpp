// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/RegistratorWindow.h"
#include <Core/Timer.h>
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

		bool gImGuiShowDrawInfo = true;
		extern float RAD2DEG;

		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::EditorAPI::Fallout4;

			RenderWindow* GlobalRenderWindowPtr = nullptr;
			RenderWindow::Area rcSafeDrawArea;
			Core::Timer ImGuiTimer;

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

					lpRelocator->DetourCall(rel, (uintptr_t)&DrawFrameEx);

					rel = _RELDATA_RAV(16);
					lpRelocator->PatchNop(rel, 0x14);
					lpRelocator->DetourCall(rel, (uintptr_t)&UpdateDrawInfo);

					*(uintptr_t*)&BGSRenderWindow::Pick::GetRefFromNiNode =
						lpRelocator->DetourFunctionClass(_RELDATA_RAV(17), (uintptr_t)&BGSRenderWindow::Pick::HKGetRefFromNiNode);

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
						ImGuiTimer.Start();
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
					case WM_MOUSEMOVE:
					case WM_MOUSEWHEEL:
						ImGuiTimer.Start();
					break;
				}

				return CallWindowProc(GlobalRenderWindowPtr->GetOldWndProc(), Hwnd, Message, wParam, lParam);
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
					if (RenderTarget)
					{
						ComPtr<ID3D11RenderTargetView> pRenderTargetViews;
						pointer_d3d11DeviceContext->OMGetRenderTargets(1, pRenderTargetViews.GetAddressOf(), nullptr);

						if (pRenderTargetViews.Get() == *RenderTarget)
						{
							GlobalRenderWindowPtr->_ImagespaceAA->Draw(This);
							ImGuiDrawInfo();
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

			void RenderWindow::ImGuiDrawInfo()
			{
				// IMGUI
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				BGSRenderWindow* RenderWindow = BGSRenderWindow::Singleton.GetSingleton();
				if (RenderWindow)
				{
					// io.MousePos always { -1, -1 }, let's fix it
					auto& io = ImGui::GetIO();
					auto p = RenderWindow->GetMousePos();
					io.MousePos = { (float)p.x, (float)p.y };

					auto ActiveCell = RenderWindow->GetCurrentCell();
					if (ActiveCell)
					{
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
								ImGui::Text("%u", BGSRenderWindow::DrawInfo::FramePerSecond);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Draw calls: ");
								ImGui::TableNextColumn();
								if (BGSRenderWindow::DrawInfo::DrawCalls < 8000)
									ImGui::TextColored(gImGuiGreenColor, "%u", BGSRenderWindow::DrawInfo::DrawCalls);
								else if (BGSRenderWindow::DrawInfo::DrawCalls < 12000)
									ImGui::TextColored(gImGuiOrangeColor, "%u", BGSRenderWindow::DrawInfo::DrawCalls);
								else
									ImGui::TextColored(gImGuiRedColor, "%u", BGSRenderWindow::DrawInfo::DrawCalls);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Polygons:");
								ImGui::TableNextColumn();
								ImGui::Text("%u", BGSRenderWindow::DrawInfo::Polys);

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
									auto Counter = [](TESObjectREFR** refrs, DWORD count, uint32_t& npcs, uint32_t& lights, uint32_t& objs)
										{
											uint32_t uId = 0;
											npcs = 0;
											lights = 0;
											objs = 0;

											for (uId = 0; uId < count; uId++)
											{
												auto form = refrs[uId];
												auto formType = form->Parent->GetFormType();

												if (!form->IsDeleted()) {
													switch (formType) {
													case TESForm::ftActor:
														npcs++;
														break;
													case TESForm::ftLight:
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
												case TESForm::ftActor:
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

								ImGui::PopFont();
								ImGui::EndTable();
							}

							ImGui::End();
						}

						if (RenderWindow && BGSRenderWindow::Pick::Result)
						{
							POINT pt;
							RECT rcWnd;
							double Tick;

							GetCursorPos(&pt);
							GetWindowRect(RenderWindow->GetWindowHandle(), &rcWnd);

							if (gImGuiShowDrawInfo)
							{
								RECT rcWndToolInfo = { rcWnd.left, rcWnd.top, 330 + rcWnd.left, 210 + rcWnd.top };
								if (PtInRect(&rcWndToolInfo, pt))
									goto Skips;
							}

							Tick = ImGuiTimer.Get();

							if (PtInRect(&rcWnd, pt) && (Tick > 1.5) && (Tick <= 7.5))
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
										ImGui::TextColored(gImGuiGreyColor, FormParent->GetFormTypeShortStr());

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
										ImGui::Text("%.03f", Ref->GetScaleFloat());

										ImGui::PopFont();
										ImGui::EndTable();
									}

									ImGui::End();
								}
							}
						Skips:
							{ /* nope : need compiler, lmao syntax error */ }
						}
					}
				}

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}
		}
	}
}
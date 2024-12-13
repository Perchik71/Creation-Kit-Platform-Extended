// jdiashow.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "../../Dependencies/jDialogs/include/jdialogs.h"

namespace jDialogs = perchik71::jDialogs;

#include <iostream>
#include <filesystem>
#include <memory>

static bool get_file_version(const char* a_filename, char* a_buffer, size_t a_buffersize)
{
    uint32_t dwSize = 0;
    VS_FIXEDFILEINFO* pFileInfo = nullptr;
    uint32_t puLenFileInfo = 0;

    dwSize = GetFileVersionInfoSizeA(a_filename, nullptr);
    if (dwSize == 0)
        return false;

    auto pbVersionInfo = std::make_unique<uint8_t[]>(dwSize);
    if (!pbVersionInfo)
        return false;

    if (!GetFileVersionInfoA(a_filename, 0, dwSize, pbVersionInfo.get()))
        return false;

    if (!VerQueryValueA(pbVersionInfo.get(), "\\", (LPVOID*)&pFileInfo, &puLenFileInfo))
        return false;

    sprintf_s(a_buffer, a_buffersize, "n%u.%u-%u", (pFileInfo->dwFileVersionMS >> 16) & 0xFFFF,
        (pFileInfo->dwFileVersionMS) & 0xFFFF, pFileInfo->dwFileVersionLS);

    return true;
}

static void hello()
{
    char modulePath[MAX_PATH];
    GetModuleFileNameA((HMODULE)GetModuleHandleA(nullptr), modulePath, MAX_PATH);

    char szBuffer[100];
    if (get_file_version(modulePath, szBuffer, _ARRAYSIZE(szBuffer)))
    {
        std::cout << "jdiashow version " << szBuffer << " copyright (c) 2024 the CKPE developers.\n";
        std::cout << "Show dialog from .json files.\n\n\n";
    }
}

static void example()
{
    std::cout << "usage: jdiashow [infile]...\n";
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        break;
    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    return 0;
}

static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        break;
    case WM_CLOSE:
    case WM_DESTROY:
        EndDialog(hWnd, 0);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            EndDialog(hWnd, 0);
            break;
        }
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

static void run_unsafe(int a_argc, char* a_argv[])
{
    WNDCLASSA wc = { 0 };
    wc.hInstance = GetModuleHandleA(0);
    wc.lpszClassName = "jWndTEST";
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    wc.lpfnWndProc = WndProc;
    if (!RegisterClassA(&wc)) 
        return;

    auto wnd = CreateWindowA(wc.lpszClassName, "", WS_POPUP, 
        0, 0, 10, 10, NULL, NULL, wc.hInstance, NULL);
    if (!wnd)
        return;

    for (int i = 1; i < a_argc; ++i)
    {
        if (!std::filesystem::exists(a_argv[i]))
        {
            std::cout << "File \"" << a_argv[i] <<"\" no found!\n";
            continue;
        }

        auto dialog = new jDialogs::jDialogA();
        if (!dialog)
        {
            std::cout << "File \"" << a_argv[i] << "\" init failed!\n";
            continue;
        }

        if (dialog->LoadFromFile(a_argv[i]) && !dialog->GenerateBinary())
        {
            delete dialog;
            std::cout << "File \"" << a_argv[i] << "\" no dialog!\n";
            continue;
        }

        dialog->ShowModal(wnd, DlgProc, NULL);
        delete dialog;
    }

    DestroyWindow(wnd);
}

int main(int a_argc, char* a_argv[])
{
    if (a_argc == 1)
    {
        hello();
        example();

        return 0;
    }

    hello();

    __try
    {
        run_unsafe(a_argc, a_argv);

        return 0;
    }
    __except (1)
    {
        std::cout << "FATAL ERROR!!!\n";
        return -1;
    }
}
#pragma once

#include "MainWindow.g.h"

#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/microsoft.ui.interop.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>

namespace winrt::LyWinUI3Samples::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        Windows::Foundation::IAsyncAction SetIconAsync(Microsoft::UI::Windowing::AppWindow const& window)
        {
            using namespace winrt;
            using namespace Windows::Storage;
            using namespace Windows::Foundation;
            using namespace Microsoft::UI::Windowing;

            Uri uri{ L"ms-appx:///Resources/Icon5.ico" };
            try
            {
                StorageFile storageFile = co_await StorageFile::GetFileFromApplicationUriAsync(uri);
                if (storageFile)
                {
                    AppWindow().SetIcon(storageFile.Path().c_str());
                }
            }
            catch (...)
            {
                // Failed to load icon, use default or ignore
            }
        }

        MainWindow()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

            /*auto hIcon = LoadImageW(NULL, L"G:\\WinUIProg\\Icon5.ico", IMAGE_ICON, 0,
                0, LR_LOADFROMFILE);
            if (hIcon == NULL)
            {
                DWORD err = GetLastError();
                wchar_t buf[256];
                swprintf_s(buf, L"Failed to load icon. Error: %lu\n", err);
                OutputDebugString(buf);
            }
            Microsoft::UI::IconId iconID = Microsoft::UI::GetIconIdFromIcon((HICON)hIcon);
            AppWindow().SetIcon(iconID);*/

            SetIconAsync(this->AppWindow());
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::LyWinUI3Samples::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}

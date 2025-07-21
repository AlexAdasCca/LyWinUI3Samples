#pragma once

#include "App.xaml.g.h"

namespace winrt::ToastNotifyUnpackaged::implementation
{
    struct App : AppT<App>
    {
        App();

        static std::wstring GetFullPathToExe();
        static std::wstring GetFullPathToAsset(std::wstring const& assetName);
        static void ToForeground();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
    };
}

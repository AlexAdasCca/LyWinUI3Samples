#include "pch.h"
#include "App.xaml.h"

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);
    auto myLambda = [](auto&&) {
        ::winrt::make<::winrt::ToastNotifyUnpackaged::implementation::App>();
        };
    ::winrt::Microsoft::UI::Xaml::Application::Start(
        myLambda);
    return 0;
}
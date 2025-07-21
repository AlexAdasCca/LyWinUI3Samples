#pragma once

#include "Pages/MainPage.g.h"

namespace winrt::ToastNotifyUnpackaged::Pages::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();
        ~MainPage();

        static ToastNotifyUnpackaged::Pages::MainPage Current() { return s_current.get(); }
        void NotificationReceived(Notification const& notification);

        void Page_Loaded(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void SendToastWithTextBox_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void SendToastWithAvatar_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void SendToastWithImage_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void SendToastWithAudio_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        static winrt::weak_ref<ToastNotifyUnpackaged::Pages::MainPage> s_current;
        Windows::Foundation::Collections::IVector<IInspectable> messages;
    };
}

namespace winrt::ToastNotifyUnpackaged::Pages::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}

#pragma once

#include "MainWindow.g.h"


namespace winrt::ToastNotifyUnpackaged::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

            InitializeComponent();
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::ToastNotifyUnpackaged::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}

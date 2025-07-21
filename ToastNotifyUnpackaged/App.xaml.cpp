#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"

#include "Notifications\NotificationManager.h"
#include <wil/result.h>
#include <Microsoft.UI.Xaml.Window.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/microsoft.ui.interop.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Microsoft.Windows.AppNotifications.h>

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::Windows::AppLifecycle;
    using namespace winrt::Microsoft::Windows::AppNotifications;
}

// NotificationManager is responsible for registering and unregistering the Sample for App Notifications as well as
// dispatching actioned notifications to the appropriate scenario.
// Registration will happen when Init() is called and Unregistration will happen when this
// instance variable goes out of scope, i.e.: when the App is terminated.
static NotificationManager g_notificationManager;


// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::ToastNotifyUnpackaged::implementation
{
    static App* app{ nullptr };

    /// <summary>
    /// Initializes the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    App::App()
    {
        g_notificationManager.Init();
        InitializeComponent();
        app = this;

        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    void App::ToForeground()
    {
        assert(app != nullptr);

        HWND hwnd;
        auto windowNative{ app->window.as<IWindowNative>() };
        if (windowNative && SUCCEEDED(windowNative->get_WindowHandle(&hwnd)))
        {
            SwitchToThisWindow(hwnd, TRUE);
        }
    }

    std::wstring App::GetFullPathToExe()
    {
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
        return std::wstring(buffer).substr(0, pos);
    }

    std::wstring App::GetFullPathToAsset(std::wstring const& assetName)
    {
        return GetFullPathToExe() + L"\\Assets\\" + assetName;
    }

    /// <summary>
    /// Invoked when the application is launched.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        

        window = make<MainWindow>();


        // NOTE: AppInstance is ambiguous between
        // Microsoft.Windows.AppLifecycle.AppInstance and
        // Windows.ApplicationModel.AppInstance
        auto currentInstance{ winrt::AppInstance::GetCurrent() };
        if (currentInstance)
        {
            // AppInstance.GetActivatedEventArgs will report the correct ActivationKind,
            // even in WinUI's OnLaunched.
            winrt::AppActivationArguments activationArgs{ currentInstance.GetActivatedEventArgs() };
            if (activationArgs)
            {
                winrt::ExtendedActivationKind extendedKind{ activationArgs.Kind() };
                if (extendedKind == winrt::Microsoft::Windows::AppLifecycle::ExtendedActivationKind::AppNotification)
                {
                    winrt::AppNotificationActivatedEventArgs notificationActivatedEventArgs{ activationArgs.Data().as<winrt::AppNotificationActivatedEventArgs>() };
                    g_notificationManager.ProcessLaunchActivationArgs(notificationActivatedEventArgs);
                }
            }
        }

        // Set the window's icon.

        auto hIcon = LoadImageW(NULL, L"G:\\WinUIProg\\Icon5.ico", IMAGE_ICON, 0,
            0, LR_LOADFROMFILE);
        if (hIcon == NULL)
        {
            DWORD err = GetLastError();
            wchar_t buf[256];
            swprintf_s(buf, L"Failed to load icon. Error: %lu\n", err);
            OutputDebugString(buf);
        }
        Microsoft::UI::IconId iconID = Microsoft::UI::GetIconIdFromIcon((HICON)hIcon);
        window.AppWindow().SetIcon(iconID);
        window.Activate();
    }
}

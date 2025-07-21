// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "pch.h"
#include "NotificationManager.h"
#include "App.xaml.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include "ToastWithAvatar.h"
#include "ToastWithTextBox.h"
#include "Common.h"
#include <map>
#include <functional>

namespace winrt
{
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::Windows::AppNotifications;
    /*using namespace ToastNotifyUnpackaged::implementation;*/
}

static const std::map<unsigned, std::function<void (winrt::AppNotificationActivatedEventArgs const&)>> c_notificationHandlers
{
    // When adding new a scenario, be sure to add its notification handler here.
    { ToastWithAvatar::ScenarioId, ToastWithAvatar::NotificationReceived },
    { ToastWithTextBox::ScenarioId, ToastWithTextBox::NotificationReceived }
};

NotificationManager::NotificationManager():m_isRegistered(false){}

NotificationManager::~NotificationManager()
{
    if (m_isRegistered)
    {
        winrt::AppNotificationManager::Default().Unregister();
    }
}

void NotificationManager::Init()
{
    // NotificationManager::Init() 在内部尝试申请缓冲区，这可能导致失败，
    // 但是 WIL 使用快速失败机制来处理这些情况，所以不会导致程序崩溃，这是一个可忽略的内部异常.
    auto notificationManager{ winrt::AppNotificationManager::Default() };

    // To ensure all Notification handling happens in this process instance, register for
    // NotificationInvoked before calling Register(). Without this a new process will
    // be launched to handle the notification.
    const auto token{ notificationManager.NotificationInvoked([&](const auto&, winrt::AppNotificationActivatedEventArgs  const& notificationActivatedEventArgs)
        {
            //NotifyUser::NotificationReceived();

            if (!DispatchNotification(notificationActivatedEventArgs))
            {
                //NotifyUser::UnrecognizedToastOriginator();
            }
        }) };

    // NotificationManager::Register() 引发的“未指定的错误”异常，是由于尝试 RetrieveAssetsFromShortcut 时引起的错误，所以可以忽略
    winrt::AppNotificationManager::Default().Register();
    m_isRegistered = true;
}

void NotificationManager::ProcessLaunchActivationArgs(winrt::AppNotificationActivatedEventArgs const& notificationActivatedEventArgs)
{
    DispatchNotification(notificationActivatedEventArgs);
}

bool NotificationManager::DispatchNotification(winrt::AppNotificationActivatedEventArgs const& notificationActivatedEventArgs)
{
    auto scenarioId{ notificationActivatedEventArgs.Arguments().Lookup(Common::scenarioTag) };
    if (!scenarioId.empty())
    {
        try
        {
            c_notificationHandlers.at(std::stoul(std::wstring(scenarioId.c_str())))(notificationActivatedEventArgs); // Will throw if out of range
            return true;
        }
        catch (...)
        {
            return false; // Couldn't find a NotificationHandler for scenarioId.
        }
    }
    else
    {
        return false; // No scenario specified in the notification
    }
}

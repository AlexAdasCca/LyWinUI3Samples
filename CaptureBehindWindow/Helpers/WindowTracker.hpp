#pragma once

#include "Helpers/WindowMessageController.hpp"
#include <windows.h>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Hosting;

class WindowTracker {
public:
    WindowTracker();
    ~WindowTracker();

    // 跟踪目标窗口
    void TrackWindow(HWND hWnd);

    // 绑定源窗口
    void BindSourceWindow(HWND sourceHwnd);

    // 解除绑定
    void UnbindSourceWindow();

    // 启用/禁用消息转发
    void EnableMessageForwarding(bool enable);

private:
    // 窗口状态结构
    struct TrackedWindow {
        HWND hWnd = nullptr;
        std::unique_ptr<WindowMessageMonitor> monitor;
        bool isActive = false;
    };

    // 绑定状态
    struct SourceBinding {
        HWND sourceHwnd = nullptr;
        HWND targetHwnd = nullptr;
        bool forwardingEnabled = false;
    };

    // 线程安全访问
    std::mutex m_mutex;

    // 被跟踪的窗口
    std::unordered_map<HWND, TrackedWindow> m_trackedWindows;

    // 当前绑定
    SourceBinding m_currentBinding;

    // 窗口消息处理
    void HandleTrackedWindowMessage(WindowMessageEventArgs& args, HWND trackedHwnd);
    /*void HandleSourceWindowMessage(WindowMessageEventArgs& args);*/

    // 消息转发逻辑
    void ForwardMessageToTarget(HWND targetHwnd, const MSG& msg);
    bool ShouldForwardMessage(const MSG& msg);

    // 窗口状态检查
    bool IsWindowActive(HWND hwnd);
    bool IsWindowInForeground(HWND hwnd);
    bool IsSourceWindowActive();

    // 窗口关系检查
    bool IsWindowAbove(HWND topHwnd, HWND bottomHwnd);
    bool IsWindowVisibleInHierarchy(HWND hwnd);

    // 工具函数
    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
};

// 窗口消息转发管理器
class WindowMessageForwarder {
public:
    static WindowMessageForwarder& GetInstance();

    void RegisterWindow(HWND hwnd, std::function<void(WindowMessageEventArgs&)> callback);
    void UnregisterWindow(HWND hwnd);

private:
    std::mutex m_mutex;
    std::unordered_map<HWND, std::unique_ptr<WindowMessageMonitor>> m_monitors;

    WindowMessageForwarder() = default;
    ~WindowMessageForwarder() = default;
};
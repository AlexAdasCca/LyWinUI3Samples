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

    // ����Ŀ�괰��
    void TrackWindow(HWND hWnd);

    // ��Դ����
    void BindSourceWindow(HWND sourceHwnd);

    // �����
    void UnbindSourceWindow();

    // ����/������Ϣת��
    void EnableMessageForwarding(bool enable);

private:
    // ����״̬�ṹ
    struct TrackedWindow {
        HWND hWnd = nullptr;
        std::unique_ptr<WindowMessageMonitor> monitor;
        bool isActive = false;
    };

    // ��״̬
    struct SourceBinding {
        HWND sourceHwnd = nullptr;
        HWND targetHwnd = nullptr;
        bool forwardingEnabled = false;
    };

    // �̰߳�ȫ����
    std::mutex m_mutex;

    // �����ٵĴ���
    std::unordered_map<HWND, TrackedWindow> m_trackedWindows;

    // ��ǰ��
    SourceBinding m_currentBinding;

    // ������Ϣ����
    void HandleTrackedWindowMessage(WindowMessageEventArgs& args, HWND trackedHwnd);
    /*void HandleSourceWindowMessage(WindowMessageEventArgs& args);*/

    // ��Ϣת���߼�
    void ForwardMessageToTarget(HWND targetHwnd, const MSG& msg);
    bool ShouldForwardMessage(const MSG& msg);

    // ����״̬���
    bool IsWindowActive(HWND hwnd);
    bool IsWindowInForeground(HWND hwnd);
    bool IsSourceWindowActive();

    // ���ڹ�ϵ���
    bool IsWindowAbove(HWND topHwnd, HWND bottomHwnd);
    bool IsWindowVisibleInHierarchy(HWND hwnd);

    // ���ߺ���
    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
};

// ������Ϣת��������
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
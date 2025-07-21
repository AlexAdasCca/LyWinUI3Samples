#pragma once
// C++/WinRT implementation of a WindowMessageMonitor equivalent to the C# version using SetWindowSubclass

#include <windows.h>
#include <commctrl.h>
#include <functional>
#include <unordered_map>
#include <memory>

#pragma comment(lib, "comctl32.lib")

class WindowMessageEventArgs {
public:
    MSG message;
    bool handled = false;
    LRESULT result = 0;

    WindowMessageEventArgs(MSG const& msg) : message(msg) {}
};

using MessageCallback = std::function<void(WindowMessageEventArgs&)>;

class WindowMessageMonitor {
public:
    WindowMessageMonitor(HWND hwnd)
        : m_hwnd(hwnd)
    {
        m_classId = ++s_classCounter;
    }

    ~WindowMessageMonitor()
    {
        RemoveSubclass();
    }

    void AddMessageHandler(MessageCallback callback)
    {
        if (!m_active)
        {
            SetSubclass();
            m_active = true;
        }
        m_callback = std::move(callback);
    }

    void RemoveMessageHandler()
    {
        m_callback = nullptr;
        RemoveSubclass();
        m_active = false;
    }

private:
    HWND m_hwnd;
    UINT_PTR m_classId = 0;
    bool m_active = false;
    MessageCallback m_callback;

    inline static UINT_PTR s_classCounter = 101;

    static LRESULT CALLBACK SubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
        UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
    {
        auto monitor = reinterpret_cast<WindowMessageMonitor*>(dwRefData);
        if (monitor && monitor->m_callback)
        {
            MSG message = { hwnd, msg, wParam, lParam, 0, {0, 0} };
            WindowMessageEventArgs args(message);
            monitor->m_callback(args);
            if (args.handled)
                return args.result;
        }
        return DefSubclassProc(hwnd, msg, wParam, lParam);
    }

    void SetSubclass()
    {
        SetWindowSubclass(m_hwnd, SubclassProc, m_classId, reinterpret_cast<DWORD_PTR>(this));
    }

    void RemoveSubclass()
    {
        RemoveWindowSubclass(m_hwnd, SubclassProc, m_classId);
    }
};

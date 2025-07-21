#include "pch.h"
#include "MainPage.xaml.h"
#if __has_include("Pages/MainPage.g.cpp")
#include "Pages/MainPage.g.cpp"
#endif

#include "Notifications\ToastWithTextBox.h"
#include "Notifications\ToastWithAvatar.h"

namespace winrt
{
	using namespace winrt::Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::ToastNotifyUnpackaged::Pages::implementation
{
	winrt::weak_ref<ToastNotifyUnpackaged::Pages::MainPage> winrt::ToastNotifyUnpackaged::Pages::implementation::MainPage::s_current{};

	MainPage::MainPage() :  messages(winrt::single_threaded_observable_vector<IInspectable>())
	{
		// Xaml objects should not call InitializeComponent during construction.
		// See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
		if (!MainPage::s_current)
		{
			MainPage::s_current = *this;
		}
	}

	MainPage::~MainPage()
	{
		MainPage::s_current = nullptr;
	}

	void MainPage::NotificationReceived(Notification const& notification)
	{
		std::wstring text{ notification.Originator };

		text.append(L"\t- Action: " + notification.Action);

		if (notification.HasInput)
		{
			if (notification.Input.empty())
			{
				text.append(L"\t- No input received");
			}
			else
			{
				text.append(L"\t- Input received: " + notification.Input);
			}
		}
		messages.InsertAt(0, PropertyValue::CreateString(text));
	}

	void MainPage::Page_Loaded(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
	{
		notificationBox().ItemsSource(messages);
	}

	void MainPage::SendToastWithTextBox_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
	{
		if (ToastWithTextBox::SendToast())
		{
			//NotifyUser::ToastSentSuccessfully();
			OutputDebugString(L"Toast sent successfully");
		}
		else
		{
			//NotifyUser::CouldNotSendToast();
			OutputDebugString(L"Could not send toast");
		}
	}

	void MainPage::SendToastWithAvatar_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
	{
		if (ToastWithAvatar::SendToast())
		{
			//NotifyUser::ToastSentSuccessfully();
			OutputDebugString(L"Toast sent successfully");
		}
		else
		{
			//NotifyUser::CouldNotSendToast();
			OutputDebugString(L"Could not send toast");
		}
	}

	void MainPage::SendToastWithImage_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
	{

	}

	void MainPage::SendToastWithAudio_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
	{

	}
}

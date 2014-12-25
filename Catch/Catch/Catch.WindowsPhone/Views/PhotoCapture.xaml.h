//
// PhotoCapture.xaml.h
// PhotoCapture 类的声明
//

#pragma once

#include "Views\PhotoCapture.g.h"
#include "Common\NavigationHelper.h"

namespace Catch
{
	/// <summary>
	/// 基本页，提供大多数应用程序通用的特性。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PhotoCapture sealed
	{
	public:
		PhotoCapture();

		/// <summary>
		/// 获取此 <see cref="Page"/> 的视图模型。
		/// 可将其更改为强类型视图模型。
		/// </summary>
		property Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ DefaultViewModel
		{
			Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^  get();
		}

		/// <summary>
		/// 获取与此 <see cref="Page"/> 关联的 <see cref="NavigationHelper"/>。
		/// </summary>
		property Common::NavigationHelper^ NavigationHelper
		{
			Common::NavigationHelper^ get();
		}

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void LoadState(Platform::Object^ sender, Common::LoadStateEventArgs^ e);
		void SaveState(Platform::Object^ sender, Common::SaveStateEventArgs^ e);
		void OnResume();

		static Windows::UI::Xaml::DependencyProperty^ _defaultViewModelProperty;
		static Windows::UI::Xaml::DependencyProperty^ _navigationHelperProperty;
		void AcceptBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CaptureButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void SystemMediaControlsPropertyChanged(Windows::Media::SystemMediaTransportControls^ sender, Windows::Media::SystemMediaTransportControlsPropertyChangedEventArgs^ e);

		concurrency::task<Windows::Devices::Enumeration::DeviceInformation^> GetCamera(Windows::Devices::Enumeration::Panel camera);
		concurrency::task<void> InitPreview(Windows::Devices::Enumeration::DeviceInformation^ device);
		concurrency::task<void> StartPreview();

		void ScenarioInit();
		void ScenarioClose();

		//Platform::Agile<Windows::Media::Capture::MediaCapture> mediaCaptureMgr;
		Windows::Media::Capture::MediaCapture^ mediaCapture;
		bool isPreviewing = false;
		Windows::Foundation::EventRegistrationToken m_eventRegistrationToken;
	};

}

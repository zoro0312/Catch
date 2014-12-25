//
// PhotoCapture.xaml.cpp
// PhotoCapture 类的实现
//

#include "pch.h"
#include "PhotoCapture.xaml.h"
using namespace Catch;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Media::Capture;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Media;
using namespace concurrency;
using namespace Windows::ApplicationModel::Core;
// “基本页”项模板在 http://go.microsoft.com/fwlink/?LinkID=390556 上有介绍

PhotoCapture::PhotoCapture()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Platform::Collections::Map<String^, Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &PhotoCapture::LoadState);
	navigationHelper->SaveState += ref new Common::SaveStateEventHandler(this, &PhotoCapture::SaveState);
	ScenarioInit();
	//SystemMediaTransportControls^ systemMediaControls = SystemMediaTransportControls::GetForCurrentView();
	//m_eventRegistrationToken = systemMediaControls->PropertyChanged += 
	//	ref new TypedEventHandler<SystemMediaTransportControls^, 
	//	SystemMediaTransportControlsPropertyChangedEventArgs^>(this, &PhotoCapture::SystemMediaControlsPropertyChanged);
}

DependencyProperty^ PhotoCapture::_defaultViewModelProperty =
DependencyProperty::Register("DefaultViewModel",
TypeName(IObservableMap<String^, Object^>::typeid), TypeName(PhotoCapture::typeid), nullptr);

/// <summary>
/// 用作普通视图模型。
/// </summary>
IObservableMap<String^, Object^>^ PhotoCapture::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ PhotoCapture::_navigationHelperProperty =
DependencyProperty::Register("NavigationHelper",
TypeName(Common::NavigationHelper::typeid), TypeName(PhotoCapture::typeid), nullptr);

/// <summary>
/// 获取 <see cref="NavigationHelper"/> 的实现，该实现旨在
/// 用作普通视图模型。
/// </summary>
Common::NavigationHelper^ PhotoCapture::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

#pragma region Navigation support

/// 此部分中提供的方法只是用于使
/// NavigationHelper 可响应页面的导航方法。
/// 
/// 应将页面特有的逻辑放入用于  
/// <see cref="NavigationHelper::LoadState"/>
/// 和 <see cref="NavigationHelper::SaveState"/> 的事件处理程序中。
/// 除了在会话期间保留的页面状态之外 
/// LoadState 方法中还提供导航参数。

void PhotoCapture::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
	
}

void PhotoCapture::OnNavigatedFrom(NavigationEventArgs^ e)
{	
	/*SystemMediaTransportControls^ systemMediaControls = SystemMediaTransportControls::GetForCurrentView();
	systemMediaControls->PropertyChanged -= m_eventRegistrationToken;*/
	ScenarioClose();
	NavigationHelper->OnNavigatedFrom(e);
}

#pragma endregion

/// <summary>
/// 使用在导航过程中传递的内容填充页。在从以前的会话
/// 重新创建页时，也会提供任何已保存状态。
/// </summary>
/// <param name="sender">
/// 事件的来源; 通常为 <see cref="NavigationHelper"/>
/// </param>
/// <param name="e">事件数据，其中既提供在最初请求此页时传递给
/// <see cref="Frame::Navigate(Type, Object)"/> 的导航参数，又提供
/// 此页在以前会话期间保留的状态的
/// 的字典。首次访问页面时，该状态将为 null。</param>
void PhotoCapture::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	(void) sender;	// 未使用的参数
	(void) e;	// 未使用的参数
	if (App::Current->mediaCaptureMgr == nullptr)
	{
		ScenarioInit();
	}
}

/// <summary>
/// 保留与此页关联的状态，以防挂起应用程序或
/// 从导航缓存中放弃此页。值必须符合
/// <see cref="SuspensionManager::SessionState"/> 的序列化要求。
/// </summary>
/// <param name="sender">事件的来源；通常为 <see cref="NavigationHelper"/></param>
/// <param name="e">提供要使用可序列化状态填充的空字典
/// 的事件数据。</param>
void PhotoCapture::SaveState(Object^ sender, Common::SaveStateEventArgs^ e){
	(void) sender;	// 未使用的参数
	(void) e; // 未使用的参数
}

void PhotoCapture::OnResume()
{
	ScenarioInit();
}

void PhotoCapture::SystemMediaControlsPropertyChanged(Windows::Media::SystemMediaTransportControls^ sender,
	Windows::Media::SystemMediaTransportControlsPropertyChangedEventArgs^ e)
{
	switch (e->Property)
	{
	case SystemMediaTransportControlsProperty::SoundLevel:
		create_task(Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([this, sender]()
		{
			if (sender->SoundLevel != SoundLevel::Muted)
			{
				ScenarioInit();
			}
		})));
		break;

	default:
		break;
	}
}

concurrency::task<Windows::Devices::Enumeration::DeviceInformation^> PhotoCapture::GetCamera(Windows::Devices::Enumeration::Panel camera)
{
	return create_task(DeviceInformation::FindAllAsync(DeviceClass::VideoCapture))
		.then([this, camera](DeviceInformationCollection^ devices)
	{
		DeviceInformation^ device = nullptr;
		for (int i = 0; i < devices->Size; i++)
		{
			device = devices->GetAt(i);
			if (device->EnclosureLocation != nullptr  && device->EnclosureLocation->Panel == camera)
				break;
		}
		return device;
	});
}
concurrency::task<void> PhotoCapture::InitPreview(Windows::Devices::Enumeration::DeviceInformation^ device)
{
	MediaCaptureInitializationSettings^ settings = ref new MediaCaptureInitializationSettings();
	settings->StreamingCaptureMode = StreamingCaptureMode::Video;
	settings->PhotoCaptureSource = PhotoCaptureSource::Photo;
	settings->AudioDeviceId = "";
	settings->VideoDeviceId = device->Id;
	return create_task(mediaCapture->InitializeAsync(settings));
}
concurrency::task<void> PhotoCapture::StartPreview()
{
	previewElement->Source = mediaCapture;
	App::Current->previewElement = previewElement;
	mediaCapture->SetPreviewRotation(VideoRotation::Clockwise90Degrees);
	return create_task(mediaCapture->StartPreviewAsync());
}

void PhotoCapture::ScenarioInit()
{
	if (App::Current->mediaCaptureMgr== nullptr) {
		mediaCapture = ref new MediaCapture();
		App::Current->mediaCaptureMgr = mediaCapture;
		isPreviewing = false;
		previewElement->Source = nullptr;

		GetCamera(Windows::Devices::Enumeration::Panel::Back).then([this](DeviceInformation^ device)
		{
			return InitPreview(device);
		}).then([this](void)
		{
			return StartPreview();
		}).then([this]()
		{
			isPreviewing = true;
			App::Current->isPreviewing = isPreviewing;
		});
	} 
	else 
	{
		mediaCapture = App::Current->mediaCaptureMgr;

		StartPreview().then([this]()
		{
			isPreviewing = true;
			App::Current->isPreviewing = isPreviewing;
		});
	}

}
void PhotoCapture::ScenarioClose()
{
	if (isPreviewing && mediaCapture != nullptr)
	{
		try {
			create_task(mediaCapture->StopPreviewAsync())
				.then([this](void)
			{
				isPreviewing = false;
				App::Current->isPreviewing = isPreviewing;
			});
		}
		catch (Exception^ e)
		{
			isPreviewing = false;
		}
	}
}

void Catch::PhotoCapture::AcceptBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void Catch::PhotoCapture::CaptureButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


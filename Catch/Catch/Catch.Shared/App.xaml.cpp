//
// App.xaml.cpp
// App 类的实现。
//

#include "pch.h"
#include "HubPage.xaml.h"
using namespace Catch;
using namespace Catch::Common;

using namespace concurrency;
using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Media::Animation;
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

// “通用中心应用程序”项目模板在 http://go.microsoft.com/fwlink/?LinkID=391958 上有介绍

/// <summary>
/// 初始化单一实例应用程序对象。这是创作的代码的第一行
/// 逻辑上等同于 main() 或 WinMain()。
/// </summary>
App::App()
{
	InitializeComponent();
	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
	//Resuming +=
	//	ref new EventHandler<Platform::Object^>(this, &App::OnResume);
	Current = this;
}

/// <summary>
/// 在最终用户正常启动应用程序时调用。	将在启动应用程序
/// 当启动应用程序以打开特定的文件或显示搜索结果等操作时，
/// 将使用其他入口点。
/// </summary>
/// <param name="e">有关启动请求和过程的详细信息。</param>
void App::OnLaunched(LaunchActivatedEventArgs^ e)
{

#if _DEBUG
	// 调试时显示图形分析信息。
	if (IsDebuggerPresent())
	{
		// 显示当前帧速率计数器
		DebugSettings->EnableFrameRateCounter = true;
	}
#endif

	auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

	// 不要在窗口已包含内容时重复应用程序初始化，
	// 只需确保窗口处于活动状态
	if (rootFrame == nullptr)
	{
		// 创建一个 Frame 以用作导航上下文并将其与
		// SuspensionManager 键关联
		rootFrame = ref new Frame();

		// 将框架与 SuspensionManager 键关联。
		SuspensionManager::RegisterFrame(rootFrame, "AppFrame");

		// TODO: 将此值更改为适合您的应用程序的缓存大小。
		rootFrame->CacheSize = 1;

		auto prerequisite = task<void>([](){});
		if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
		{
			// 适当时，仅还原已保存的会话状态，计划
			// 还原完成后的最终启动步骤
			prerequisite = SuspensionManager::RestoreAsync();
		}

		prerequisite.then([=](task<void> prerequisite)
		{
			try
			{
				prerequisite.get();
			}
			catch (Platform::Exception^)
			{
				// 还原状态时出现问题。
				// 假定没有状态并继续。
				__debugbreak();
			}

			if (rootFrame->Content == nullptr)
			{

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
				// 删除用于启动的旋转门导航。
				if (rootFrame->ContentTransitions != nullptr)
				{
					_transitions = ref new TransitionCollection();
					for (auto transition : rootFrame->ContentTransitions)
					{
						_transitions->Append(transition);
					}
				}

				rootFrame->ContentTransitions = nullptr;
				_firstNavigatedToken = rootFrame->Navigated += ref new NavigatedEventHandler(this, &App::RootFrame_FirstNavigated);
#endif

				// 当导航堆栈尚未还原时，导航到第一页，
				// 并通过将所需信息作为导航参数传入来配置
				// 新页面
				rootFrame->Navigate(HubPage::typeid, e->Arguments);
			}

			// 将框架放在当前窗口中
			Window::Current->Content = rootFrame;

			// 确保当前窗口处于活动状态
			Window::Current->Activate();

		}, task_continuation_context::use_current());
	}
	else
	{
		if (rootFrame->Content == nullptr)
		{
			// 当导航堆栈尚未还原时，导航到第一页，
			// 并通过将所需信息作为导航参数传入来配置
			// 新页面
			if (!rootFrame->Navigate(HubPage::typeid, e->Arguments))
			{
				throw ref new FailureException("Failed to create initial page");
			}
		}

		// 确保当前窗口处于活动状态
		Window::Current->Activate();
	}
}

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
/// <summary>
/// 启动应用程序后还原内容转换。
/// </summary>
void App::RootFrame_FirstNavigated(Object^ sender, NavigationEventArgs^ e)
{
	auto rootFrame = safe_cast<Frame^>(sender);

	TransitionCollection^ newTransitions;
	if (_transitions == nullptr)
	{
		newTransitions = ref new TransitionCollection();
		newTransitions->Append(ref new NavigationThemeTransition());
	}
	else
	{
		newTransitions = _transitions;
	}

	rootFrame->ContentTransitions = newTransitions;

	rootFrame->Navigated -= _firstNavigatedToken;
}

#endif

/// <summary>
/// 在将要挂起应用程序执行时调用。将保存应用程序状态
/// 将被终止还是恢复的情况下保存应用程序状态，
/// 并让内存内容保持不变。
/// </summary>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
	(void) sender;	// 未使用的参数
	(void) e;		// 未使用的参数

	auto deferral = e->SuspendingOperation->GetDeferral();
	SuspensionManager::SaveAsync().then([=]()
	{
		CleanMediaCapture().then([deferral](void)
		{
			deferral->Complete();
		});
		
	});
}

concurrency::task<Windows::Devices::Enumeration::DeviceInformation^> App::GetCamera(Windows::Devices::Enumeration::Panel camera)
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
concurrency::task<void> App::InitPreview(Windows::Devices::Enumeration::DeviceInformation^ device)
{
	MediaCaptureInitializationSettings^ settings = ref new MediaCaptureInitializationSettings();
	settings->StreamingCaptureMode = StreamingCaptureMode::Video;
	settings->PhotoCaptureSource = PhotoCaptureSource::Photo;
	settings->AudioDeviceId = "";
	settings->VideoDeviceId = device->Id;
	return create_task(mediaCaptureMgr->InitializeAsync(settings));
}
concurrency::task<void> App::StartPreview()
{
	previewElement->Source = mediaCaptureMgr;
	mediaCaptureMgr->SetPreviewRotation(VideoRotation::Clockwise90Degrees);
	return create_task(mediaCaptureMgr->StartPreviewAsync());
}

void App::ScenarioInit()
{
	mediaCaptureMgr = ref new MediaCapture();
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
	});
}

void App::OnResume(Platform::Object^ sender, Platform::Object^  e)
{
	//ScenarioInit();
}

concurrency::task<void> App::CleanMediaCapture()
{
	task<void> op([](){});
	if (mediaCaptureMgr != nullptr)
	{
		if (isPreviewing) 
		{
			try 
			{
				op =  create_task(mediaCaptureMgr->StopPreviewAsync())
					.then([this](void)
				{
					if (previewElement != nullptr)
						previewElement->Source = nullptr;

					mediaCaptureMgr = nullptr;
					isPreviewing = false;
				});
			}
			catch (Exception^ e)
			{

			}
		}
		else {
			mediaCaptureMgr = nullptr;
			isPreviewing = false;
		}

	}
	return op;
}

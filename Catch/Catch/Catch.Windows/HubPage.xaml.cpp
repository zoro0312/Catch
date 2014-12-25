//
// HubPage.xaml.cpp
// HubPage 类的实现
//

#include "pch.h"
#include "HubPage.xaml.h"
#include "SectionPage.xaml.h"
#include "ItemPage.xaml.h"

using namespace Catch;
using namespace Catch::Common;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// “通用中心应用程序”项目模板在 http://go.microsoft.com/fwlink/?LinkID=391958 上有介绍

HubPage::HubPage()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Map<String^,Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &HubPage::LoadState);
}

DependencyProperty^ HubPage::_defaultViewModelProperty = DependencyProperty::Register(
	"DefaultViewModel",
	TypeName(IObservableMap<String^,Object^>::typeid),
	TypeName(HubPage::typeid), nullptr);

/// <summary>
/// 获取此 <see cref="Page"/> 的视图模型。
/// 可将其更改为强类型视图模型。
/// </summary>
IObservableMap<String^, Object^>^ HubPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ HubPage::_navigationHelperProperty = DependencyProperty::Register(
	"NavigationHelper",
	TypeName(NavigationHelper::typeid),
	TypeName(HubPage::typeid),
	nullptr);

/// <summary>
/// 获取 <see cref="NavigationHelper"/> 的实现，该实现旨在
/// 用作普通视图模型。
/// </summary>
NavigationHelper^ HubPage::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

/// 此部分中提供的方法只是用于使
/// NavigationHelper 可响应页面的导航方法。
/// 
/// 应将页面特有的逻辑放入用于
/// <see cref="NavigationHelper::LoadState"/>
/// 和 <see cref="NavigationHelper::SaveState"/> 的事件处理程序中。
/// 除了在早期会话期间保留的页面状态之外
/// LoadState 方法中还提供导航参数。

void HubPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void HubPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);
}

/// <summary>
/// 使用在导航过程中传递的内容填充页。在从以前的会话
/// 重新创建页时，也会提供任何已保存状态。
/// </summary>
/// <param name="sender">
/// 事件的来源；通常为 <see cref="NavigationHelper"/>
/// </param>
/// <see cref="Frame::Navigate(Type, Object)"/> 的导航参数，又提供
/// 此页在以前会话期间保留的状态的
/// 字典。首次访问页面时，该状态将为 null。</param>
void HubPage::LoadState(Object^ sender, LoadStateEventArgs^ e)
{
	(void) sender;	// 未使用的参数
	(void) e;	// 未使用的参数

	// TODO: 创建适用于问题域的合适数据模型以替换示例数据
	Data::SampleDataSource::GetGroup("Group-4")
		.then([this](Data::SampleDataGroup^ Section3Items)
	{
		DefaultViewModel->Insert("Section3Items", Section3Items);
	}, task_continuation_context::use_current());
}

/// <summary>
/// 在单击 HubSection 标题时调用。
/// </summary>
/// <param name="sender">包含单击了其标题的 HubSection 的中心。</param>
/// <param name="e">描述如何启动单击的事件数据。</param>
void HubPage::Hub_SectionHeaderClick(Object^ sender, HubSectionHeaderClickEventArgs^ e)
{
	HubSection^ section = e->Section;
	auto group = section->DataContext;

	// 导航至相应的目标页，并
	// 通过将所需信息作为导航参数传入来配置新页
	Frame->Navigate(TypeName(SectionPage::typeid), safe_cast<Data::SampleDataGroup^>(group)->UniqueId);
}

/// <summary>
/// 在单击节内的项时调用。
/// </summary>
/// <param name="sender">GridView 或 ListView
/// 为 ListView)。</param>
/// <param name="e">描述所单击项的事件数据。</param>
void HubPage::ItemView_ItemClick(Object^ sender, ItemClickEventArgs^ e)
{
	(void) sender;	// 未使用的参数

	// 导航至相应的目标页，并
	// 通过将所需信息作为导航参数传入来配置新页
	auto itemId = safe_cast<Data::SampleDataItem^>(e->ClickedItem)->UniqueId;
	Frame->Navigate(TypeName(ItemPage::typeid), itemId);
}

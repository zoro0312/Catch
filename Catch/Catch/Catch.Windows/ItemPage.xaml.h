//
// ItemPage.xaml.h
// ItemPage 类的声明
//

#pragma once

#include "ItemPage.g.h"

namespace Catch
{
	/// <summary>
	/// 在显示组内单个项的详细信息同时允许使用手势
	/// 浏览同一组的其他项的页。
	/// </summary>
	[Windows::UI::Xaml::Data::Bindable]
	public ref class ItemPage sealed
	{
	public:
		ItemPage();
		/// <summary>
		/// 可将其更改为强类型视图模型。
		/// </summary>
		property Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ DefaultViewModel
		{
			Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^  get();
		}
		/// <summary>
		/// NavigationHelper 在每页上用于协助导航和
		/// 进程生命期管理
		/// </summary>
		property Catch::Common::NavigationHelper^ NavigationHelper
		{
			Catch::Common::NavigationHelper^ get();
		}

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void LoadState(Platform::Object^ sender, Catch::Common::LoadStateEventArgs^ e);

		static Windows::UI::Xaml::DependencyProperty^ _defaultViewModelProperty;
		static Windows::UI::Xaml::DependencyProperty^ _navigationHelperProperty;
	};
}

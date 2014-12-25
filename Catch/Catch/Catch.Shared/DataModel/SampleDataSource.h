//
// SampleDataSource.h
// SampleDataSource、SampleDataGroup、SampleDataItem 和 SampleDataCommon 类的声明
//

#pragma once

// 此文件定义的数据模型可充当在添加、移除或修改成员时
// 。所选属性名称与标准项模板中的数据绑定一致。
//
// 应用程序可以使用此模型作为起始点并以它为基础构建，或完全放弃它并
// 替换为适合其需求的其他内容。如果使用此模式，则可提高应用程序
// 响应速度，途径是首次启动应用程序时启动 App.xaml 隐藏代码中的数据加载任务
//。

namespace Catch
{
	namespace Data
	{
		ref class SampleDataGroup; // 解决 SampleDataItem 和 SampleDataGroup 之间的循环关系

		/// <summary>
		/// 泛型项数据模型。
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class SampleDataItem sealed : public Windows::UI::Xaml::Data::ICustomPropertyProvider
		{
		public:
			property Platform::String^ UniqueId { Platform::String^ get(); }
			property Platform::String^ Title { Platform::String^ get(); }
			property Platform::String^ Subtitle { Platform::String^ get(); }
			property Platform::String^ Description { Platform::String^ get(); }
			property Platform::String^ Content { Platform::String^ get(); }
			property Platform::String^ ImagePath { Platform::String^ get(); }

			//ICustomPropertyProvider 的实现提供了要用作辅助功能的自动化名称的对象的字符串表示形式
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetCustomProperty(Platform::String^ name);
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type);
			virtual Platform::String^ GetStringRepresentation();
			property Windows::UI::Xaml::Interop::TypeName Type { virtual Windows::UI::Xaml::Interop::TypeName get(); }

		internal:
			SampleDataItem(Platform::String^ uniqueId, Platform::String^ title, Platform::String^ subtitle, Platform::String^ imagePath,
				Platform::String^ description, Platform::String^ content);

		private:
			Platform::String^ _uniqueId;
			Platform::String^ _title;
			Platform::String^ _subtitle;
			Platform::String^ _imagePath;
			Platform::String^ _description;
			Platform::String^ _content;
		};

		/// <summary>
		/// 泛型组数据模型。
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class SampleDataGroup sealed : public Windows::UI::Xaml::Data::ICustomPropertyProvider
		{
		public:
			property Platform::String^ UniqueId { Platform::String^ get(); }
			property Platform::String^ Title { Platform::String^ get(); }
			property Platform::String^ Subtitle { Platform::String^ get(); }
			property Platform::String^ Description { Platform::String^ get(); }
			property Windows::Foundation::Collections::IObservableVector<SampleDataItem^>^ Items
			{
				Windows::Foundation::Collections::IObservableVector<SampleDataItem^>^ get();
			}
			property Platform::String^ ImagePath { Platform::String^ get(); }

			//ICustomPropertyProvider 的实现提供了要用作辅助功能的自动化名称的对象的字符串表示形式
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetCustomProperty(Platform::String^ name);
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type);
			virtual Platform::String^ GetStringRepresentation();
			property Windows::UI::Xaml::Interop::TypeName Type { virtual Windows::UI::Xaml::Interop::TypeName get(); }

		internal:
			SampleDataGroup(Platform::String^ uniqueId, Platform::String^ title, Platform::String^ subtitle, Platform::String^ imagePath,
				Platform::String^ description);

		private:
			Platform::String^ _uniqueId;
			Platform::String^ _title;
			Platform::String^ _subtitle;
			Platform::String^ _imagePath;
			Platform::String^ _description;
			Platform::Collections::Vector<SampleDataItem^>^ _items;
		};

		/// <summary>
		/// 创建包含从静态 json 文件读取内容的组和项的集合。
		/// 
		/// SampleDataSource 通过从项目中包括的静态 json 文件读取的数据进行
		/// 初始化。这样在设计时和运行时均可提供示例数据。
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class SampleDataSource sealed
		{
		public:
			property Windows::Foundation::Collections::IObservableVector<SampleDataGroup^>^ Groups
			{
				Windows::Foundation::Collections::IObservableVector<SampleDataGroup^>^ get();
			}

		internal:
			SampleDataSource();
			static concurrency::task<Windows::Foundation::Collections::IIterable<SampleDataGroup^>^> GetGroups();
			static concurrency::task<SampleDataGroup^> GetGroup(Platform::String^ uniqueId);
			static concurrency::task<SampleDataItem^> GetItem(Platform::String^ uniqueId);

		private:
			concurrency::task_completion_event<void> _loadCompletionEvent;
			Platform::Collections::Vector<SampleDataGroup^>^ _groups;
			static concurrency::task<void> Init();

			static SampleDataSource^ _sampleDataSource;
		};
	}
}

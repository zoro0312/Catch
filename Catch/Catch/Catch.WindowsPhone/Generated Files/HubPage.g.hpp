﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "HubPage.xaml.h"




void ::Catch::HubPage::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///HubPage.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///HubPage.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the Grid named 'LayoutRoot'
    LayoutRoot = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"LayoutRoot"));
    // Get the Hub named 'Hub'
    Hub = safe_cast<::Windows::UI::Xaml::Controls::Hub^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"Hub"));
}

void ::Catch::HubPage::Connect(int connectionId, Platform::Object^ target)
{
    switch (connectionId)
    {
    case 1:
        (safe_cast<::Windows::UI::Xaml::Controls::ListViewBase^>(target))->ItemClick +=
            ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::Catch::HubPage::*)(Platform::Object^, Windows::UI::Xaml::Controls::ItemClickEventArgs^))&HubPage::ItemView_ItemClick);
        break;
    case 2:
        (safe_cast<::Windows::UI::Xaml::Controls::ListViewBase^>(target))->ItemClick +=
            ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::Catch::HubPage::*)(Platform::Object^, Windows::UI::Xaml::Controls::ItemClickEventArgs^))&HubPage::ItemView_ItemClick);
        break;
    case 3:
        (safe_cast<::Windows::UI::Xaml::Controls::ListViewBase^>(target))->ItemClick +=
            ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::Catch::HubPage::*)(Platform::Object^, Windows::UI::Xaml::Controls::ItemClickEventArgs^))&HubPage::ItemView_ItemClick);
        break;
    case 4:
        (safe_cast<::Windows::UI::Xaml::Controls::ListViewBase^>(target))->ItemClick +=
            ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::Catch::HubPage::*)(Platform::Object^, Windows::UI::Xaml::Controls::ItemClickEventArgs^))&HubPage::ItemView_ItemClick);
        break;
    case 5:
        (safe_cast<::Windows::UI::Xaml::Controls::ListViewBase^>(target))->ItemClick +=
            ref new ::Windows::UI::Xaml::Controls::ItemClickEventHandler(this, (void (::Catch::HubPage::*)(Platform::Object^, Windows::UI::Xaml::Controls::ItemClickEventArgs^))&HubPage::GroupSection_ItemClick);
        break;
    }
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}


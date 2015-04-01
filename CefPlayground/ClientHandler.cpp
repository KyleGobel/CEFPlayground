#include "ClientHandler.h"

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"


#include <iostream>

namespace {
	ClientHandler* g_instance = NULL;

	// Custom menu command Ids.
	enum client_menu_ids {
		CLIENT_ID_SHOW_DEVTOOLS = MENU_ID_USER_FIRST,
		CLIENT_ID_CLOSE_DEVTOOLS,
		CLIENT_ID_INSPECT_ELEMENT,
		CLIENT_ID_TESTMENU_SUBMENU,
		CLIENT_ID_TESTMENU_CHECKITEM,
		CLIENT_ID_TESTMENU_RADIOITEM1,
		CLIENT_ID_TESTMENU_RADIOITEM2,
		CLIENT_ID_TESTMENU_RADIOITEM3,
	};
}

int ClientHandler::browser_count_ = 0;

ClientHandler::ClientHandler() : is_closing_(false), browser_id_(0)
{
	DCHECK(!g_instance);
	g_instance = this;
}

ClientHandler::~ClientHandler() {
	g_instance = NULL;
}

//static
ClientHandler* ClientHandler::GetInstance() {
	return g_instance;
}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	m_Browser = browser;
	browser_id_ = browser->GetIdentifier();
	
	//increment the static browser count (Dev Tool Windows are browsers)
	browser_count_ = browser_count_ + 1;
}

bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	int id = browser->GetIdentifier();

	if (GetBrowserId() == browser->GetIdentifier())
	{
		// Set a flag to indicate that the window close is allowed
		is_closing_ = true;
	}


	// Allow the close.
	return false;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (GetBrowserId() == browser->GetIdentifier()) 
	{
		base::AutoLock lock_scope(lock_);
		// Free browser pointer so browser can be destroyed
		m_Browser = NULL;

	}

	if (browser_count_ - 1 == 0)
	{
		// Browser is closed, quit the app and message loop
		CefQuitMessageLoop();
	}

	//Decrement the static browser count
	browser_count_ = browser_count_ - 1;
}

void ClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl) {
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED)
		return;

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</h2></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

void ClientHandler::CloseBrowser(bool force_close) {
	if (!CefCurrentlyOn(TID_UI)){
		//Execute on the UI thread.
		CefPostTask(TID_UI,
			base::Bind(&ClientHandler::CloseBrowser, this, force_close));
		return;
	}
}


//Context Menu
void ClientHandler::OnBeforeContextMenu(
	CefRefPtr<CefBrowser> browser, 
	CefRefPtr<CefFrame> frame, 
	CefRefPtr<CefContextMenuParams> params, 
	CefRefPtr<CefMenuModel> model) 
{
	CEF_REQUIRE_UI_THREAD();

	if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0)
	{
		// Add a separator if the menu has items
		if (model->GetCount() > 0)
			model->AddSeparator();

		//  Add DevTools to all context menus
		model->AddItem(CLIENT_ID_SHOW_DEVTOOLS, "&Show Dev Tools");
		model->AddItem(CLIENT_ID_CLOSE_DEVTOOLS, "Close Dev Tools");
		model->AddSeparator();
		model->AddItem(CLIENT_ID_INSPECT_ELEMENT, "Inspect Element");
	}
}

bool ClientHandler::OnContextMenuCommand(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	int command_id,
	EventFlags event_flags)
{
	CEF_REQUIRE_UI_THREAD();

	switch (command_id)
	{
	case CLIENT_ID_SHOW_DEVTOOLS:
		ShowDevTools(browser, CefPoint());
		return true;
	case CLIENT_ID_CLOSE_DEVTOOLS:
		CloseDevTools(browser);
		return true;
	case CLIENT_ID_INSPECT_ELEMENT:
		ShowDevTools(browser, CefPoint(params->GetXCoord(), params->GetYCoord()));
		return true;
	default: //allow default handling to proceed
		return false;
	}

}

void ClientHandler::ShowDevTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at) 
{
	CefWindowInfo windowInfo;
	CefBrowserSettings settings;

#if defined(OS_WIN)
	windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), "Dev Tools");
#endif

	browser->GetHost()->ShowDevTools(windowInfo, this, settings, inspect_element_at);
}

void ClientHandler::CloseDevTools(CefRefPtr<CefBrowser> browser)
{
	browser->GetHost()->CloseDevTools();
}

int ClientHandler::GetBrowserId() const 
{
	base::AutoLock lock_scope(lock_);
	return browser_id_;
}
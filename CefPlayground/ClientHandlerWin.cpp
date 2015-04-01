#include "ClientHandler.h"

#include <string>
#include <windows.h>

#include "cef_browser.h"
#include "wrapper/cef_helpers.h"


void ClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	 CEF_REQUIRE_UI_THREAD();

	 CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();

	 SetWindowText(hwnd, std::wstring(title).c_str());
}
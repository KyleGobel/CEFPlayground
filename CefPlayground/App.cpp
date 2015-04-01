#include "App.h";

#include <string>
#include "ClientHandler.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"


App::App() {}

void App::OnContextInitialized()
{
	CEF_REQUIRE_UI_THREAD();

	CefWindowInfo window_info;

#if defined(OS_WIN)
	window_info.SetAsPopup(NULL, "My Application Name");
#endif

	CefRefPtr<ClientHandler> handler(new ClientHandler());

	CefBrowserSettings browser_settings;

	std::string url;
	url = "file:///E:/github.com/KyleGobel/CefPlayground/Debug/index.html";

	CefBrowserHost::CreateBrowser(window_info, handler.get(), url, browser_settings, NULL);


}

void App::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	// Retrieve the context's window object
	CefRefPtr<CefV8Value> object = context->GetGlobal();

	// Create a new V8 string value
	CefRefPtr<CefV8Value> str = CefV8Value::CreateString("A Value!!!");


	// Add the string to the window object as window.myval
	object->SetValue("myval", str, V8_PROPERTY_ATTRIBUTE_NONE);
}
void App::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{
	//if (!javascript_handler)
	//	javascript_handler = new JavascriptHandler();

	//CefRegisterExtension("extensionTest", "alert('hi');", javascript_handler);

}
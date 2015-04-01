#ifndef _APP_H_
#define _APP_H_

#include "include/cef_app.h"
#include "JavascriptHandler.h"

class App : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler
{
public:
	App();


	// CefApp Methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE{ return this; }

	// Cef BrowserProcessHandler Methods:
	virtual void OnContextInitialized() OVERRIDE;

	// Cef RenderProcessHandler Methods:
	virtual void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE; 
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;

private:
	CefRefPtr<JavascriptHandler> javascript_handler;
	IMPLEMENT_REFCOUNTING(App);

};

#endif
#include <string>
#include <windows.h>


#include "include/cef_app.h"
#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_web_plugin.h"
#include "include/cef_sandbox_win.h"

#include "ClientHandler.h"
#include "App.h"

using namespace std;


#if defined(CEF_USE_SANDBOX)
// The cef_sandbox.lib static library is currently built with VS2013. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		break;

	case WM_ERASEBKGND:
		break;

	case WM_CLOSE:
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND RegisterWindow(HINSTANCE hInstance, int nCmdShow)
{
	const wchar_t CLASS_NAME [] = L"CefSimpleSample";
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,							// Optional window styles
		CLASS_NAME,					// window class
		L"CEF Simple Sample",		// window text
		WS_OVERLAPPEDWINDOW,		// window style

		//Size and Position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,						// parent window
		NULL,						// menu
		hInstance,					// instance handle
		NULL						// Additional app data
		);

	if (hwnd == NULL)
		return 0;

	ShowWindow(hwnd, nCmdShow);

	return hwnd;

}
ClientHandler* g_handler;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	void * sandbox_info = NULL;

	CefMainArgs main_args(hInstance);

	CefRefPtr<App> app(new App());

	CefSettings appSettings;

#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif


#if !defined(CEF_USE_SANDBOX)
	appSettings.no_sandbox = true;
#endif

	// Initalize CEF
	CefInitialize(main_args,appSettings, app.get(), sandbox_info);


	// Run the CEF message loop.  Will block until CefQuitMessageLoop() is called
	CefRunMessageLoop();

	// Shutdown CEF
	CefShutdown();

	return 0;

}

#ifndef _CLLIENTHANDLER_H_
#define _CLLIENTHANDLER_H_

#include "include/cef_client.h"
#include "include/base/cef_lock.h"

class ClientHandler : public CefClient, 
	public CefLifeSpanHandler, 
	public CefDisplayHandler, 
	public CefLoadHandler,
	public CefContextMenuHandler
{
public:
	ClientHandler();
	~ClientHandler();

	//provide access to single global instance of this object
	static ClientHandler* GetInstance();

	CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }


	//CefClient Methods
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE{
		return this;
	}
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE{
		return this;
	}
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE{
		return this;
	}
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE{
		return this;
	}



	// CefDisplayHandler Methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;

	// CefLifeSpanHandler Methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefLoadHandler Methods:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) OVERRIDE;

	// CefContextMenuHandler Methods:
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;
	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) OVERRIDE;

	// Close browser window
	void CloseBrowser(bool force_close);

	//Main Browser Id
	int GetBrowserId() const;

	//Dev Tools
	void ShowDevTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at);
	void CloseDevTools(CefRefPtr<CefBrowser> browser);

	bool IsClose() const { return is_closing_; }

	

protected:
	// The child browser window
	CefRefPtr<CefBrowser> m_Browser;

	bool is_closing_;

	// Lock used to protect members accessed on multiple threads,
	// Make it mutable so it can be used from const methods
	mutable base::Lock lock_;


	//the child browser id
	int browser_id_;

	static int browser_count_;

	//keep this last
	IMPLEMENT_REFCOUNTING(ClientHandler);
};

#endif
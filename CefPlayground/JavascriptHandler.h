#ifndef _JAVASCRIPT_HANDLER_H_
#define _JAVASCRIPT_HANDLER_H_

#include "include/cef_client.h"
#include "include/cef_v8.h"

class JavascriptHandler : public CefV8Handler
{
public:
	virtual bool Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception) OVERRIDE;

private:
	IMPLEMENT_REFCOUNTING(JavascriptHandler);
};

#endif
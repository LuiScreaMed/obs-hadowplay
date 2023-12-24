#include "Notifications.h"
#include "plugin-support.h"
#include "wintoastlib.h"

using namespace WinToastLib;

class WinToastHandlerExample : public IWinToastHandler {
public:
	WinToastHandlerExample(){};
	// Public interfaces
	void toastActivated() const override{};
	void toastActivated(int actionIndex) const override{};
	void toastDismissed(WinToastDismissalReason state) const override{};
	void toastFailed() const override{};
};

std::wstring obs_hadowplay_app_id = std::wstring(L"EZ64cool.Hadowplay");

bool obs_hadowplay_windows_show_saved_notif()
{
	WinToastHandlerExample *handler = new WinToastHandlerExample();
	WinToastTemplate templ = WinToastTemplate(WinToastTemplate::Text01);
	templ.setTextField(L"title", WinToastTemplate::FirstLine);

	return WinToast::instance()->showToast(templ, handler);
}

extern bool obs_hadowplay_init_notifications()
{
	WinToast::instance()->setAppName(L"OBS Studio");
	size_t newsize = strlen(PLUGIN_VERSION) + 1;
	wchar_t *wversion = new wchar_t[newsize]{};
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wversion, newsize, PLUGIN_VERSION,
		   _TRUNCATE);
	const auto aumi = WinToast::configureAUMI(L"ez64cool", L"obs",
						  L"hadowplay", wversion);
	delete[] wversion;
	WinToast::instance()->setAppUserModelId(aumi);

	if (!WinToast::instance()->initialize()) {
		return false;
	}

	return true;
}

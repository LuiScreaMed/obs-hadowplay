// See: https://github.com/mohabouje/WinToast

#include "Notifications.h"
#include <util/base.h>
#include <util/dstr.h>
#include <obs-module.h>
#include "plugin-support.h"
#include "wintoastlib.h"

#pragma comment(lib, "Winmm.lib")

using namespace WinToastLib;

class FilepathToastHandler : public IWinToastHandler {
private:
	std::wstring m_filepath;

public:
	FilepathToastHandler(std::wstring filepath) { m_filepath = filepath; }
	// Public interfaces
	void toastActivated() const override
	{
		std::wstring folder_path =
			m_filepath.substr(0, m_filepath.find_last_of(L'/') + 1);
		INT_PTR ret = (INT_PTR)ShellExecuteW(NULL, L"explore",
						     folder_path.c_str(), NULL,
						     NULL, SW_SHOWNORMAL);
	};
	void toastActivated(int actionIndex) const override{};
	void toastDismissed(WinToastDismissalReason state) const override{};
	void toastFailed() const override{};
};

std::wstring obs_hadowplay_char_to_wchar(const char *input)
{
	size_t len = strlen(input);
	std::wstring wc(len, L'#');
	mbstowcs(&wc[0], input, len);
	return wc;
}

WinToast *obs_hadowplay_toast_inst = nullptr;

bool obs_hadowplay_windows_show_saved_notif(wchar_t *filepath)
{
	std::wstring w_filepath = filepath;

	FilepathToastHandler *handler = new FilepathToastHandler(w_filepath);
	WinToastTemplate templ = WinToastTemplate(WinToastTemplate::Text02);
	templ.setTextField(L"Recording organised", WinToastTemplate::FirstLine);
	templ.setTextField(w_filepath, WinToastTemplate::SecondLine);
	templ.setAudioOption(WinToastTemplate::Silent);

	char *notif_filepath = obs_module_file("notification.wav");

	if (notif_filepath != nullptr) {
		PlaySoundA(notif_filepath, NULL, SND_FILENAME | SND_ASYNC);
	} else {
		PlaySound((LPWSTR)SND_ALIAS_SYSTEMDEFAULT, NULL,
			  SND_ALIAS_ID | SND_ASYNC);
	}

	bfree(notif_filepath);

	return obs_hadowplay_toast_inst->showToast(templ, handler);
}

extern bool obs_hadowplay_init_notifications()
{
	obs_hadowplay_toast_inst = new WinToast();
	obs_hadowplay_toast_inst->setAppName(L"OBS Studio");

	const auto aumi = WinToast::configureAUMI(
		L"ez64cool", L"obs", L"hadowplay",
		obs_hadowplay_char_to_wchar(PLUGIN_VERSION));
	obs_hadowplay_toast_inst->setAppUserModelId(aumi);

	if (!obs_hadowplay_toast_inst->initialize()) {
		return false;
	}

	return true;
}

extern bool obs_hadowplay_uninit_notifications()
{
	delete obs_hadowplay_toast_inst;
	obs_hadowplay_toast_inst = nullptr;

	return true;
}

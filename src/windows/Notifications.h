#pragma once

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC bool obs_hadowplay_windows_show_saved_notif(wchar_t *filepath);

EXTERNC bool obs_hadowplay_init_notifications();

EXTERNC bool obs_hadowplay_uninit_notifications();

#undef EXTERNC
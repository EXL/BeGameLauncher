#ifndef BELAUNCHERBASE_H
#define BELAUNCHERBASE_H

#include "BeMainWindow.h"
#include "BeDirectoryFilter.h"
#include "BeSettings.h"
#include "BeDirectoryFilePanel.h"

#include <String.h>
#include <View.h>
#include <TextControl.h>
#include <StringView.h>

class BeLauncherBase : public BeMainWindow
{
	const char *sWindowTitle;
	const char *sButtonBrowseToolTip;
	const char *sStringViewData;
	const char *sTextControlToolTip;
	const char *sPackageName;
	const char *sExecutableFileName;
	const char *sSettingsFileName;
	const char *sDataPath;
	const char *sFilePanelTitle;

	const bool sShowIcon;
	const bool sUseExecVe;

	BString fExecutableFilePath;

	BeSettings *fSettings;
	BeDirectoryFilter *fDirectotyFilter;
	BeDirectoryFilePanel *fDirectoryFilePanel;

	BView *fMainView;
	BStringView *fStatusString;
	BTextControl *fDataTextControl;

	void DirectorySelected(void);
	void SelectDirectory(void);

protected:
	enum
	{
		MSG_BUTTON_RUN_CLICKED        = 'btrn',
		MSG_BUTTON_BROWSE_CLICKED     = 'btbr',
		MSG_BUTTON_ABOUT_CLICKED      = 'btab',
		MSG_FILE_PANEL_FILE_SELECTED  = 'fpsc'
	};
	enum color_msg_t
	{
		B_COLOR_RED,
		B_COLOR_GREEN,
		B_COLOR_BLUE,
		B_COLOR_BLACK
	};

public:
	BeLauncherBase(const char *windowTitle, const char *packageName,
	               const char *executableFileName, const char *settingsFileName,
	               const char *dataPath, bool showIcon = false, bool useExecVe = false);
	virtual void InitParameters(const char *stringViewData, const char *textControlToolTip,
	                            const char *buttonBrowseToolTip, const char* filePanelTitle);
	virtual void CreateForm();
	virtual bool ReadSettings();
	virtual void SaveSettings(bool def);
	virtual void MessageReceived(BMessage *msg);
	virtual void SetStatusString(color_msg_t type, const BString &str);

	virtual bool CheckCache();
	virtual bool CheckExecutable();

	BView *GetMainView() const;
	BeSettings *GetSettings() const;
	BTextControl *GetTextControl() const;

	virtual bool RunGameViaRoster();
	virtual bool RunGameViaExecVe();

	virtual const char* SetDefaultDir();

	static float Gap();
	static float BannerWidth();
	static float StatusGapHack();

	virtual void ShowErrorCacheAlert();
	virtual void ShowExecutableCacheAlert();
	virtual void ShowWarnWriteSettingsAlert();

	virtual bool QuitRequested();
	virtual bool QuitRequestedSub();

	virtual void ShowAboutDialog();
};

#endif // BELAUNCHERBASE_H

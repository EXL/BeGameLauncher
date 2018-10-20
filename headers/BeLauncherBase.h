#ifndef BELAUNCHERBASE_H
#define BELAUNCHERBASE_H

#include "BeMainWindow.h"
#include "BeDirectoryFilter.h"
#include "BeSettings.h"
#include "BeDirectoryFilePanel.h"
#include "BeLauncherView.h"

#include <String.h>
#include <TextControl.h>
#include <StringView.h>
#include <Message.h>
#include <Box.h>

class BeLauncherBase : public BeMainWindow
{
	const char *sWindowTitle;
	const char *sSettingsFileName;
	const char *sDataPath;
	const char *sStartPath;

	const bool sShowIcon;
	const bool sUseExecVe;

	BString fExecutableFilePath;

	BeSettings *fSettings;
	BeDirectoryFilter *fDirectoryFilter;
	BeDirectoryFilePanel *fDirectoryFilePanel;
	BeLauncherView *fLauncherView;

	BStringView *fStatusString;
	BTextControl *fDataTextControl;
	BBox *fAdditionalBox;

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

	virtual void MessageReceived(BMessage *msg);

	virtual void CreateForm(void);
	virtual bool ReadSettings(void);
	virtual void SaveSettings(bool def);

	virtual void SetStatusString(color_msg_t type, const BString &str);

	virtual bool CheckCache(void);
	virtual bool CheckExecutable(void);

	virtual bool RunGameViaRoster(void);
	virtual bool RunGameViaExecVe(void);

	virtual void ShowErrorCacheAlert(void);
	virtual void ShowExecutableCacheAlert(void);
	virtual void ShowWarnWriteSettingsAlert(void);

	virtual bool QuitRequested(void);
	virtual bool QuitRequestedSub(void);

	virtual void ShowAboutDialog(void);

public:
	explicit BeLauncherBase(const char *windowTitle,
	                        const char *packageName,
	                        const char *executableFileName,
	                        const char *settingsFileName,
	                        const char *dataPath,
	                        const char *startPath,
	                        bool showIcon = false,
	                        bool readSettings = true,
	                        bool useExecVe = false);
	virtual ~BeLauncherBase();

	BeSettings *GetSettings(void) const;
	BTextControl *GetTextControl(void) const;
	BBox *GetAdditionalBox(void) const;

	static float Gap(void);
	static float BannerWidth(void);
};

#endif // BELAUNCHERBASE_H

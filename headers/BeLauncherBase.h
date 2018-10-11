#ifndef BELAUNCHERBASE_H
#define BELAUNCHERBASE_H

#include "BeMainWindow.h"
#include "BeDirectoryFilter.h"
#include "BeSettings.h"

#include <String.h>
#include <View.h>
#include <TextControl.h>

class BeLauncherBase : public BeMainWindow
{
	const char *sSettingsFileName;
	const char *sDataPath;
	const char *sButtonBrowseToolTip;
	const char *sStringViewData;
	const char *sTextControlToolTip;
	const char *sFilePanelTitle;
	const char *sExecutableFileName;
	const char *sPackageName;

	BString fExecutableFilePath;

	BTextControl *fDataTextControl;
	BFilePanel *fFilePanel;
	BeDirectoryFilter *fDirectotyFilter;
	BeSettings *fSettings;

	BView *fMainView;

	void DirectorySelected(void);
	void SelectDirectory(void);
	void ShowAboutDialog();
	void RunGameViaRoaster();
	void RunGameViaExecVe();

protected:
	enum
	{
		MSG_BUTTON_RUN_CLICKED        = 'btrn',
		MSG_BUTTON_EXIT_CLICKED       = 'btex',
		MSG_BUTTON_BROWSE_CLICKED     = 'btbr',
		MSG_BUTTON_ABOUT_CLICKED      = 'btab',
		MSG_FILE_PANEL_FILE_SELECTED  = 'fpsc'
	};

public:
	BeLauncherBase(const char *windowTitle, const char *packageName,
	               const char *executableFileName, const char *settingFileName, const char*dataPath);
	virtual void InitParameters(const char *stringViewData, const char *textControlToolTip,
	                            const char *buttonBrowseToolTip, const char* filePanelTitle);
	virtual void CreateForm();
	virtual bool ReadSettings();
	virtual void SaveSettings(bool def);
	virtual void MessageReceived(BMessage *msg);

	virtual bool CheckCache();
	virtual bool CheckExecutable();

	BView *GetMainView() const;
	BeSettings *GetSettings() const;
	BTextControl *GetTextControl() const;

	static float Gap();
	static float BannerWidth();
};

#endif // BELAUNCHERBASE_H

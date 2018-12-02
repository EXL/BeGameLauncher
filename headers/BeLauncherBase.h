/************************************************************************************
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2018 EXL
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy
 ** of this software and associated documentation files (the "Software"), to deal
 ** in the Software without restriction, including without limitation the rights
 ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ** copies of the Software, and to permit persons to whom the Software is
 ** furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 ** SOFTWARE.
 ************************************************************************************/

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

#include <vector>

class BeLauncherBase : public BeMainWindow
{
	const char *sWindowTitle;
	const char *sSettingsFileName;
	const char *sDataPathArg;
	const char *sStartPath;

	const bool sShowIcon;

	bool fAdditionalEndingSlash;
	bool fUseArgvZeroForRoster;

	std::vector<const char*> fCustomArgs;

	BString fExecutableFilePath;

	BeSettings *fSettings;
	BeDirectoryFilter *fDirectoryFilter;
	BeDirectoryFilePanel *fDirectoryFilePanel;
	BeLauncherView *fLauncherView;

	BStringView *fStatusString;
	BTextControl *fDataTextControl;
	BBox *fAdditionalBox;

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

	virtual void DirectorySelected(const BeDirectoryFilePanel *filePanel,
	                               BTextControl *textControl);

	virtual void MessageReceived(BMessage *msg);

	virtual void CreateForm(void);
	virtual bool ReadSettings(void);
	virtual void SaveSettings(bool def);

	virtual void SetStatusString(color_msg_t type, const BString &str);

	virtual bool CheckCache(void);
	virtual bool CheckExecutable(void);
	virtual bool CheckAll(void);

	virtual bool RunGame(void);
	virtual bool RunGameViaRoster(bool useEnviron, bool customArgs);
	virtual bool RunGameViaExecVe(bool useEnviron, bool customArgs);

	virtual void ShowErrorCacheAlert(void);
	virtual void ShowExecutableCacheAlert(void);
	virtual void ShowWarnWriteSettingsAlert(void);

	virtual bool QuitRequested(void);

	virtual void ShowAboutDialog(void);

	virtual void SetCustomArgs(const BString &str);

public:
	explicit BeLauncherBase(const char *windowTitle,
	                        const char *packageName,
	                        const char *executableFileName,
	                        const char *settingsFileName,
	                        const char *dataPathArg,
	                        const char *startPath,
	                        bool showIcon = false,
	                        bool readSettings = true);
	virtual ~BeLauncherBase(void);

	BeDirectoryFilePanel *GetDirectoryFilePanel(void) const;

	BeSettings *GetSettings(void) const;
	BTextControl *GetTextControl(void) const;
	BBox *GetAdditionalBox(void) const;

	void SetExecutablePath(const BString &path);

	void SetAdditionalEndingSlash(bool state);
	void SetUseArgvZeroForRoster(bool state);

	static float Gap(void);
	static float BannerWidth(void);
	static float GetStartPointX(void);
	static float GetStartPointY(void);
	static float GetWindowWidth(void);
	static float GetWindowHeight(void);
	static float GetOffsetForSize(void);
};

#endif // BELAUNCHERBASE_H
